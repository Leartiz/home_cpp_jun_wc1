#pragma once

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "message_parser.h"
#include "message_creator.h"
#include "request_handler.h"

namespace lez
{
	namespace adapters
	{
		namespace interfaces
		{
			namespace tcp
			{
                // or `session`!?

                class Client_connection final :
                    public std::enable_shared_from_this<Client_connection>
				{
					// short names!
				public:
					using io_context = boost::asio::io_context;
					using tcp_socket = boost::asio::ip::tcp::socket;
					using deadline_timer = boost::asio::deadline_timer;
					using error_code = boost::system::error_code;
                    using endpoint = boost::asio::ip::tcp::endpoint;

				public:
					using ptr = std::shared_ptr<Client_connection>;
                    static ptr create(io_context&, Request_handler&);

				public:
                    const std::string get_local_addr() const;
                    const std::string get_remote_addr() const;

					tcp_socket& get_tcp_socket();
					void start();

				private:                    
                    Client_connection(io_context&, Request_handler&);
                    void correct_close(const error_code& err = {});

					void async_write(std::string);
                    void async_read_next_request();
                    void async_read_part_request();

				private:
					void read_handler(const error_code&, size_t);
					void write_handler(const error_code&, size_t);
					void deadline_handler(const error_code&);

				private:
					io_context& m_ioc;
					tcp_socket m_tcp_socket;
					deadline_timer m_deadline_timer;
                    Sp_client_ctx m_client_ctx;

                    // read
                private:
                    std::uint64_t m_max_read_msg_size = 1024;
                    std::string m_read_message;
                    Message_parser m_message_parser;

                    // write
                private:
                    std::string m_write_message;
                    Message_creator m_message_creator;

					// services!
				private:
                    Request_handler& m_handler; // see `Listener`
				};
			}
		}
	}
}

