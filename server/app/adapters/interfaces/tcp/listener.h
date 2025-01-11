#pragma once

#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>

#include "client_connection.h"
#include "domain/use_case/use_cases.h"

namespace lez
{
    namespace adapters
    {
        namespace interfaces
        {
            namespace tcp
            {
                class Listener final
                {
                    // short names!
                public:
                    using io_context = boost::asio::io_context;
                    using tcp_acceptor = boost::asio::ip::tcp::acceptor;
                    using tcp_endpoint = boost::asio::ip::tcp::endpoint;
                    using error_code = boost::system::error_code;

                public:
                    using Use_cases = domain::use_case::Use_cases;

                public:
                    Listener(io_context&, const Use_cases& use_cases, const uint16_t port);
                    Listener(io_context&, const Use_cases& use_cases, const tcp_endpoint&);
                    Listener(io_context&, const Use_cases& use_cases,
                        const std::string& ip, const uint16_t port);

                private:
                    void async_accept();
                    void accept_handler(Client_connection::ptr new_connection,
                        const error_code& error);

                private:
                    io_context& m_ioc;
                    tcp_acceptor m_tcp_acceptor;

                private:
                    Request_handler m_handler;
                };
            }
        }        
    }
}



