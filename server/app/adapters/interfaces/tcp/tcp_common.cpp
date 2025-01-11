#include "tcp_common.h"

namespace lez::adapters::interfaces::tcp
{
   Common::tcp_endpoint Common::make_tcp_endpoint(const uint16_t port)
    {
        return Common::tcp_endpoint{
            boost::asio::ip::tcp::v4(), port };
    }

    Common::tcp_endpoint Common::make_tcp_endpoint(const std::string& str_ip,
        const uint16_t port)
    {
        const auto ip = boost::asio::ip::address::from_string(str_ip);
        return Common::tcp_endpoint{ ip, port };
    }
}
