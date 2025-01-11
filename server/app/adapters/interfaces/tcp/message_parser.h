#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <string>

#include "dto/request.h"

namespace lez::adapters::interfaces::tcp
{

class Message_parser final
{
public:
    explicit Message_parser(
        const std::uint64_t max_buffer_size = 8192);

public:
    void reset();
    void add_to_buffer(std::string part);
    std::size_t get_buffer_size() const;

public:
    dto::Sp_request parse_request();

private:
    std::uint64_t m_max_buffer_size;

private:
    std::string m_buffer;
};

}

#endif // MESSAGE_PARSER_H
