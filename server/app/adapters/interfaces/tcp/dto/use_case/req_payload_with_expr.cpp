#include <format>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "req_payload_with_expr.h"

namespace lez::adapters::interfaces::tcp::dto::use_case
{
    const std::string& Req_payload_with_expr::get_expr() const
    {
        return m_expression;
    }

    void Req_payload_with_expr::set_expr(const std::string& expr)
    {
        if (expr.empty()) {
            throw std::runtime_error("expression is empty");
        }

        m_expression = expr;
    }

    /*
        {
            "payload": {
                "expression": "<string>"
            }
        }
    */
    const nlohmann::json Req_payload_with_expr::to_json() const
    {
        using nlohmann::json;
        const json payload = { { Json_key::EXPRESSION, m_expression } };
        return json { { Request_payload::Json_key::PAYLOAD, payload } };
    }

    void Req_payload_with_expr::from_json(const nlohmann::json& j)
    {
        if (j.contains(Request_payload::Json_key::PAYLOAD) &&
            j[Request_payload::Json_key::PAYLOAD].contains(Json_key::EXPRESSION)) {

            m_expression = j[Request_payload::Json_key::PAYLOAD][Json_key::EXPRESSION]
                .get<std::string>();
        }
        else {
            throw std::runtime_error(
                std::format("missing `{}` or `{}` in JSON",
                            Request_payload::Json_key::PAYLOAD,
                            Json_key::EXPRESSION));
        }
    }
}

