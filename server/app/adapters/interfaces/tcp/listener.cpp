#include <boost/bind.hpp>

#include "listener.h"
#include "tcp_common.h"

#include "logging/logging.h"

namespace lez
{
    namespace adapters
    {
        namespace interfaces
        {
            namespace tcp
            {
                using namespace std;

                Listener::Listener(io_context& ioc, const Use_cases& use_cases, const uint16_t port)
                    : m_ioc{ ioc }, m_tcp_acceptor{ ioc, Common::make_tcp_endpoint(port) }
                    , m_handler{ use_cases } // create one instance!
                {
                    async_accept();
                }

                Listener::Listener(io_context& ioc, const Use_cases& use_cases,
                    const std::string& ip, const uint16_t port)
                    : m_ioc{ ioc }, m_tcp_acceptor{ ioc, Common::make_tcp_endpoint(ip, port) }
                    , m_handler{ use_cases }
                {
                    async_accept();
                }

                Listener::Listener(io_context& ioc, const Use_cases& use_cases, const tcp_endpoint& ep)
                    : m_ioc{ ioc }, m_tcp_acceptor{ ioc, ep }
                    , m_handler{ use_cases }
                {
                    async_accept();
                }

                // -------------------------------------------------------

                void Listener::async_accept()
                {
                    logging::logf(logging::Level::trace, Common::MODULE_NAME,
                                  "start next async accept");

                    // ***

                    auto new_client_connection = Client_connection::create(m_ioc, m_handler);
                    m_tcp_acceptor.async_accept(new_client_connection->get_tcp_socket(),
                        boost::bind(&Listener::accept_handler, this, new_client_connection,
                            boost::asio::placeholders::error));
                }

                void Listener::accept_handler(Client_connection::ptr new_connection,
                                              const boost::system::error_code& err)
                {
                    if (err) {
                        logging::logf(logging::Level::warning, Common::MODULE_NAME,
                                      "accept connection failed. Err: {}", err.message());

                    } else {
                        const auto addr = new_connection->get_remote_addr();
                        logging::logf(logging::Level::info, Common::MODULE_NAME,
                                      "successfully accept connection. Client: {}", addr);

                        new_connection->start();
                    }

                    // next!

                    async_accept();
                }
            }
        }        
    }
}
