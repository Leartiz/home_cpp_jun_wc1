#ifndef TCP_COMMON_H
#define TCP_COMMON_H

#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>

namespace lez::adapters::interfaces::tcp
{
    struct Common
    {
        static constexpr char MODULE_NAME[] = "TCP";

    public:
        using tcp_endpoint = boost::asio::ip::tcp::endpoint;

        static tcp_endpoint make_tcp_endpoint(const uint16_t port);
        static tcp_endpoint make_tcp_endpoint(const std::string& str_ip,
                                              const uint16_t port);
    };
}

#endif // TCP_COMMON_H
