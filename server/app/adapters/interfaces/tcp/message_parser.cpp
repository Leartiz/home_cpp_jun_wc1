#include <format>
#include <algorithm>

#include <nlohmann/json.hpp>

#include "message_parser.h"
#include "utils/error/error_utils.h"

namespace lez::adapters::interfaces::tcp
{

    namespace
    {
        dto::Sp_request str_to_request(const std::string& str)
        {
            dto::Sp_request request{ nullptr };

            // usafe!
            {
                const auto json_body = nlohmann::json::parse(str);
                request = dto::Request::from_json(json_body);
            }
            return request;
        }
    }

// -----------------------------------------------------------------------

Message_parser::Message_parser(
    const std::uint64_t max_buffer_size)
    : m_max_buffer_size{ max_buffer_size }
{}

// -----------------------------------------------------------------------

void Message_parser::reset()
{
    m_buffer.clear();
}

void Message_parser::add_to_buffer(std::string part)
{
    m_buffer += std::move(part);
    if (m_buffer.size() > m_max_buffer_size) {
        throw std::overflow_error("buffer exceeded allowed size");
    }
}

std::size_t Message_parser::get_buffer_size() const
{
    return m_buffer.size();
}

// -----------------------------------------------------------------------

dto::Sp_request Message_parser::parse_request()
{
    /*
        Exceptions slow things down.
        Is it possible to use return codes?
    */

    using dto::Request;
    using namespace error_utils;

    if (m_buffer.size() > 4) {
        auto first_four = m_buffer.substr(0, 4); // header!
        std::reverse(first_four.begin(), first_four.end());

        std::uint32_t body_size = 0;
        std::memcpy(&body_size, first_four.data(), sizeof(body_size));

        // ***

        const auto got_buffer_size = m_buffer.size() - 4;
        if (body_size > m_buffer.size() - 4) {
            const auto text = std::format("body is not complete. Got: {}, want: {}",
                                          got_buffer_size, body_size);
            throw std::length_error(text);
        }

        // impossible?
        if (body_size > m_max_buffer_size - 4) {
            throw std::overflow_error(
                "body size may exceed allowed size");
        }
    }
    else {
        throw std::length_error("header is not complete");
    }

    // ***

    try {
        return str_to_request(m_buffer.substr(4));
    }
    catch (const std::exception& e) {
        throw std::invalid_argument(
            combine_exceptions("invalid json", e));
    }
}

}
