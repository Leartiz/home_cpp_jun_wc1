#ifndef REQ_PAYLOAD_WITH_EXPR_H
#define REQ_PAYLOAD_WITH_EXPR_H

#include <nlohmann/json.hpp>

#include "../request_payload.h"

namespace lez::adapters::interfaces::tcp::dto::use_case
{
    class Req_payload_with_expr : public Request_payload
    {
    public:
        struct Json_key final
        {
            static constexpr char EXPRESSION[] = "expression";
        };

    public:
        ~Req_payload_with_expr() override = default;

    public:
        const std::string& get_expr() const;
        void set_expr(const std::string&);

        // Payload interface
    public:
        const nlohmann::json to_json() const override;
        void from_json(const nlohmann::json&) override;

    private:
        std::string m_expression;
    };
}

#endif // REQ_PAYLOAD_WITH_EXPR_H
