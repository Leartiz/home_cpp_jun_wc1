#include "message_creator.h"

namespace lez::adapters::interfaces::tcp
{

Message_creator::Message_creator() {}

std::string Message_creator::create(dto::Response::Sp response) const
{
    const std::string body = response->to_json().dump();
    uint32_t body_length = static_cast<uint32_t>(body.size());

    std::array<char, 4> length_bytes;
    length_bytes[0] = static_cast<char>((body_length >> 24) & 0xFF);
    length_bytes[1] = static_cast<char>((body_length >> 16) & 0xFF);
    length_bytes[2] = static_cast<char>((body_length >> 8)  & 0xFF);
    length_bytes[3] = static_cast<char>((body_length >> 0)  & 0xFF);

    return std::string(length_bytes.begin(), length_bytes.end()) +
        body;
}

}
