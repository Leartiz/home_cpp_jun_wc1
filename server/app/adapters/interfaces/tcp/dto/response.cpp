#include <map>
#include <stdexcept>

#include "response.h"

namespace lez::adapters::interfaces::tcp::dto
{
    std::string Response::status_code_to_str(Status_code code)
    {
        static std::map<Status_code, std::string> m{

            { Status_code::BadRequest, "bad request" },
            { Status_code::NotFound, "not found" },
            { Status_code::PayloadTooLarge, "payload too large" },

            { Status_code::InternalServerError, "internal server error" },
            //...
        };
        return m[code];
    }

    // -------------------------------------------------------------------

    const nlohmann::json Response::Metadata::to_json() const
    {
        nlohmann::json j; // object!
        j[Json_key::DURATION] = duration;
        j[Json_key::TIMESTAMP] = timestamp;
        return j;
    }

    // -------------------------------------------------------------------

    Response::Error::Error(const std::string& msg)
        : message(msg)
    {}

    const nlohmann::json Response::Error::to_json() const
    {
        nlohmann::json j;
        j[Json_key::MESSAGE] = message;
        return j;
    }

    // ok
    // -------------------------------------------------------------------

    Response::Sp Response::sucess(std::uint64_t request_id, std::shared_ptr<Response_result> rr)
    {
        // `rr` maybe nullptr!

        return std::make_shared<Response>(request_id, (int)Status_code::OK,
                                          rr);
    }

    // err
    // -------------------------------------------------------------------

    Response::Sp Response::client_error(std::uint64_t request_id, Status_code status_code)
    {
        return std::make_shared<Response>(request_id, (int)status_code,
                                          std::make_shared<Error>(status_code_to_str(status_code)));
    }

    Response::Sp Response::bad_request(std::uint64_t request_id, const std::string& extra_text)
    {
        return std::make_shared<Response>(request_id, (int)Status_code::BadRequest,
                                          std::make_shared<Error>(extra_text));
    }

    Response::Sp Response::not_found(std::uint64_t request_id, const std::string& extra_text)
    {
        return std::make_shared<Response>(request_id, (int)Status_code::NotFound,
                                          std::make_shared<Error>(extra_text));
    }

    Response::Sp Response::internal_server_error(std::uint64_t request_id, const std::string& extra_text)
    {
        return std::make_shared<Response>(request_id, (int)Status_code::InternalServerError,
                                          std::make_shared<Error>(extra_text));
    }
    //...

    // -------------------------------------------------------------------

    Response::Response(std::uint64_t request_id, int status_code, std::shared_ptr<Error> err)
        : m_request_id{ request_id }, m_status_code{ status_code }
        , m_error{ err }, m_result{ nullptr }
        , m_metadata{ nullptr }
    {
        if (!m_error)
            throw std::invalid_argument{ "error is nullptr" };

        // TODO: combinations check?
    }

    Response::Response(std::uint64_t request_id, int status_code, std::shared_ptr<Response_result> rr)
        : m_request_id{ request_id }, m_status_code{ status_code }
        , m_error{ nullptr }, m_result{ rr }
        , m_metadata{ nullptr }
    {
        if (!m_result)
            throw std::invalid_argument{ "result is nullptr" };
    }

    void Response::set_metadata(std::shared_ptr<Metadata> metadata)
    {
        m_metadata = metadata;
    }

    // -------------------------------------------------------------------

    const nlohmann::json Response::to_json() const
    {
        nlohmann::json j;
        j[Json_key::REQUEST_ID] = m_request_id;
        j[Json_key::STATUS_CODE] = m_status_code;

        j[Json_key::METADATA] = nullptr;
        j[Json_key::RESULT] = nullptr;
        j[Json_key::ERR] = nullptr;

        if (m_metadata) {
            j[Json_key::METADATA] = m_metadata->to_json();
        }

        // or
        if (m_error) {
            j[Json_key::ERR] = m_error->to_json();
        }
        else {
            j[Json_key::RESULT] = m_result->to_json();
        }

        return j;
    }
}
