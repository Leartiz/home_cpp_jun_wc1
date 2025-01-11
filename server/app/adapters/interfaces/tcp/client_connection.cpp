#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "tcp_common.h"
#include "request_context.h"
#include "client_connection.h"

#include "utils/uuid/uuid_utils.h"
#include "logging/logging.h"

#include "dto/response.h"
#include "dto/request.h"

namespace lez
{
	namespace adapters
	{
		namespace interfaces 
		{
			namespace tcp
            {
                namespace
                {
                    std::string ep_to_full_addr(const Client_connection::endpoint& ep)
                    {
                        const auto addr = ep.address();
                        const auto port = ep.port();

                        std::ostringstream sout;
                        sout << addr << ":" << port;

                        return sout.str();
                    }
                }

                // -------------------------------------------------------

                Client_connection::Client_connection(io_context& ioc, Request_handler& handler)
                    : m_ioc{ ioc }, m_tcp_socket{ ioc }, m_deadline_timer{ ioc }
                    , m_client_ctx{ Client_context::create(uuid_utils::gen()) } // has id?
                    , m_handler{ handler }
                {}

                Client_connection::ptr Client_connection::create(io_context& ioc,
                                                                 Request_handler& handler)
				{
                    return std::shared_ptr<Client_connection>(
                        new Client_connection(ioc, handler));
				}

                void Client_connection::correct_close(const error_code& err)
                {
                    m_deadline_timer.cancel(); // !?
                    if (err) {
                        logging::logf(logging::Level::warning, Common::MODULE_NAME,
                                      "client [{}] is or will be disconnected due to an error: {}",
                                      get_remote_addr(), err.to_string());
                    }

                    if (!m_tcp_socket.is_open()) {
                        return;
                    }

                    // ***

                    boost::system::error_code shutdown_err;
                    m_tcp_socket.shutdown(
                                // Shutdown both send and receive on the socket.
                                boost::asio::ip::tcp::socket::shutdown_both,
                                shutdown_err);

                    if (shutdown_err)
                    {
                        logging::logf(logging::Level::warning, Common::MODULE_NAME,
                                      "failed to shutdown socket for client [{}]: {}",
                                      get_remote_addr(), shutdown_err.message());
                    }

                    boost::system::error_code close_err;
                    m_tcp_socket.close(close_err);
                    if (close_err)
                    {
                        logging::logf(logging::Level::warning, Common::MODULE_NAME,
                                      "failed to close socket for client [{}]: {}",
                                      get_remote_addr(), close_err.message());
                    }

                    return;
                }

                // -----------------------------------------------------------------

                const std::string Client_connection::get_local_addr() const
                {
                    const auto remote_ep = m_tcp_socket.local_endpoint();
                    return ep_to_full_addr(remote_ep);
                }

                const std::string Client_connection::get_remote_addr() const
                {
                    const auto remote_ep = m_tcp_socket.remote_endpoint();
                    return ep_to_full_addr(remote_ep);
                }

				Client_connection::tcp_socket&
					Client_connection::get_tcp_socket()
				{
					return m_tcp_socket;
				}

				void Client_connection::start()
				{
                    async_read_next_request();
				}

                // read/write
                // -----------------------------------------------------------------

                void Client_connection::async_read_next_request()
				{
                    using namespace boost::posix_time;
					using namespace boost::asio::placeholders;

                    m_message_parser.reset(); // to empty!
                    m_read_message = std::string(m_max_read_msg_size, 0);

                    m_deadline_timer.expires_from_now(seconds(30000));
					m_deadline_timer.async_wait(
						boost::bind(&Client_connection::deadline_handler,
							shared_from_this(), error));

                    m_tcp_socket.async_read_some(boost::asio::buffer(m_read_message),
						boost::bind(&Client_connection::read_handler,
							shared_from_this(), error, bytes_transferred));
				}

                void Client_connection::async_read_part_request()
                {
                    using namespace boost::asio::placeholders;

                    m_read_message = std::string(m_max_read_msg_size, 0);
                    m_tcp_socket.async_read_some(boost::asio::buffer(m_read_message),
                        boost::bind(&Client_connection::read_handler,
                            shared_from_this(), error, bytes_transferred));
                }

                // -----------------------------------------------------------------

				void Client_connection::async_write(std::string w_message)
				{
					using namespace boost::asio::placeholders;
                    m_write_message = std::move(w_message);
                    m_tcp_socket.async_write_some(boost::asio::buffer(m_write_message),
						boost::bind(&Client_connection::write_handler,
							shared_from_this(), error, bytes_transferred));
				}

				// handlers
                // -----------------------------------------------------------------

				void Client_connection::read_handler(const error_code& err,
					size_t bytes_transferred)
				{                    
                    if (err) {
                        correct_close(err); // transfer!
                        return;
					}

                    logging::logf(logging::Level::trace, Common::MODULE_NAME,
                                  "{} bytes read from client [{}]",
                                  bytes_transferred, get_remote_addr());

                    // work with byte array!

                    dto::Sp_request request;
                    try {
                        m_message_parser.add_to_buffer(
                            m_read_message.substr(0, bytes_transferred));

                        logging::logf(logging::Level::trace, Common::MODULE_NAME,
                                      "client [{}] has full message size {}",
                                      get_remote_addr(), m_message_parser.get_buffer_size());

                        request = m_message_parser.parse_request();
                        m_deadline_timer.cancel(); // !?
                    }
                    catch(const std::length_error& e) {

                        logging::logf(logging::Level::trace, Common::MODULE_NAME,
                                      "client [{}] message was not read completely. Details: {}",
                                      get_remote_addr(), e.what());

                        async_read_part_request();
                        return;
                    }
                    catch(const std::overflow_error& e) { /* critical error */

                        logging::logf(logging::Level::warning, Common::MODULE_NAME,
                                      "client [{}] sent a message that was too long. Details: {}",
                                      get_remote_addr(), e.what());

                        const auto code = dto::Response::Status_code::PayloadTooLarge;
                        const auto response = dto::Response::client_error(0, code);
                        async_write(m_message_creator.create(response));

                        // ?

                        return;
                    }

                    logging::logf(logging::Level::trace, Common::MODULE_NAME,
                                  "client [{}] sent a request {}",
                                  get_remote_addr(), request->to_json().dump());

                    // ***

                    auto req_ctx = \
                        Request_context::create(m_client_ctx, request); // ?

                    const auto response = m_handler.handle(req_ctx);
                    async_write(m_message_creator.create(response));
				}

				void Client_connection::write_handler(const error_code& err,
                    size_t)
				{
                    if (err) {
                        correct_close(err);
                        return;
                    }

                    async_read_next_request();
				}

                void Client_connection::deadline_handler(const error_code& err)
				{
                    if (err == boost::asio::error::operation_aborted) {
                        logging::logf(logging::Level::trace, Common::MODULE_NAME,
                                      "client [{}] deadline timer canceled. Details: {}",
                                      get_remote_addr(), err.to_string());
                        return;
                    }

                    correct_close();
				}
			}
		}
	}
}
