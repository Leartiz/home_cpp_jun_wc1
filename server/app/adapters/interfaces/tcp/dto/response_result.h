#ifndef RESPONSE_RESULT_H
#define RESPONSE_RESULT_H

#include <memory>

#include <nlohmann/json.hpp>

namespace lez::adapters::interfaces::tcp::dto
{
    class Response_result
    {
    public:
        Response_result();
        virtual ~Response_result() = default;

    public:
        virtual const nlohmann::json to_json() const = 0;
    };

    using Sp_res_result = \
        std::shared_ptr<Response_result>;
}

#endif // RESPONSE_RESULT_H
