#ifndef REQUEST_PAYLOAD_H
#define REQUEST_PAYLOAD_H

#include <memory>

#include "nlohmann/json.hpp"

namespace lez::adapters::interfaces::tcp::dto
{
    class Request_payload
    {
    public:
        struct Json_key final
        {
            static constexpr char PAYLOAD[] = "payload";
        };

    public:
        virtual ~Request_payload() = default;

    public:
        virtual const nlohmann::json to_json() const = 0;
        virtual void from_json(const nlohmann::json& json) = 0;
    };

    using Sp_req_payload =
        std::shared_ptr<Request_payload>;
}

#endif // REQUEST_PAYLOAD_H
