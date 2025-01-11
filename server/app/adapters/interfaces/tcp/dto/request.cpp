#include <format>

#include "request.h"
#include "common_validator.h"

#include "use_case/req_payload_with_expr.h"
#include "domain/use_case/use_cases.h"

namespace
{
    std::string parse_str_from_json(const nlohmann::json& j, const std::string& key,
                                    const std::string& err_message)
    {
        if (j.contains(key)) {
            return j[key].get<std::string>();
        }

        throw std::runtime_error{
            err_message
        };
    }
}

// -----------------------------------------------------------------------

namespace lez::adapters::interfaces::tcp::dto
{

    Request::Request()
        : m_request_id{ 0 }
        , m_use_case_name{}
        , m_payload{ nullptr }
    {}

// -----------------------------------------------------------------------

    void Request::set_payload(Sp_req_payload pd)
    {
        m_payload = pd;
    }

    void Request::set_use_case_name(const std::string& str)
    {
        Common_validator::string_not_empty(str, "use case name cannot be empty");
        m_use_case_name = str;
    }

    // -----------------------------------------------------------------------

    std::uint64_t Request::get_request_id() const
    {
        return m_request_id;
    }

    const std::string& Request::get_use_case_name() const
    {
        return m_use_case_name;
    }

    Sp_req_payload Request::get_payload() const
    {
        return m_payload;
    }

// -----------------------------------------------------------------------

    std::shared_ptr<Request> Request::from_json(const nlohmann::json& j)
    {
        Request request;
        if (j.contains(Json_key::REQUEST_ID)) {
            request.m_request_id = j[Json_key::REQUEST_ID].get<std::uint64_t>();
        }
        else {
            throw std::runtime_error(
                std::format("missing `{}` in JSON", Json_key::REQUEST_ID));
        }

        // ***

        const auto use_case_name = parse_str_from_json(j, Json_key::USE_CASE,
                    std::format("missing `{}` in JSON", Json_key::USE_CASE));
        Common_validator::string_not_empty(use_case_name,
                    std::format("`{}` is empty string", Json_key::USE_CASE));
        request.m_use_case_name = use_case_name;

        // ***

        using namespace domain::use_case;
        if (use_case_name == Calc_math_expr_uc::NAME) { // !
            const auto payload = std::make_shared<use_case::Req_payload_with_expr>();
            payload->from_json(j); // void!
            request.m_payload = payload;
        }
        // other ucs...
        else {
            request.m_payload = nullptr;

            //throw std::runtime_error("unsupported use case");
        }

        return std::make_shared<Request>(request);
    }

    /*
        ```json
        {
            "request_id": "<uint64>",

            "service": "math",
            "action": "calculate",

            "payload": {
                "expression": "<string>"
            }
        }
        ```
    */
    const nlohmann::json Request::to_json() const
    {
        nlohmann::json j;
        j[Json_key::REQUEST_ID] = m_request_id;
        j[Json_key::USE_CASE] = m_use_case_name;

        if (m_payload) {
           auto j_payload = m_payload->to_json(); // ?
           for (auto& element : j_payload.items()) {
               j[element.key()] = element.value();
           }
        }
        else {
            j[Request_payload::Json_key::PAYLOAD] = nullptr; // !
        }

        return j;
    }
}
