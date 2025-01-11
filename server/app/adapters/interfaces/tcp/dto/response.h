#ifndef RESPONSE_H
#define RESPONSE_H

#include <memory>
#include <string>
#include <cstdint>
#include <chrono>

#include <nlohmann/json.hpp>

#include "response_result.h"

namespace lez::adapters::interfaces::tcp::dto
{
    class Response final
    {
    public:
        using Sp = std::shared_ptr<Response>; // !

    public:
        enum class Status_code : int // as HTTP
        {
            OK = 200,

            BadRequest = 400,
            NotFound = 404,
            PayloadTooLarge = 413,

            InternalServerError = 500
            //...
        };

        static std::string status_code_to_str(Status_code code);

    public:
        struct Metadata final
        {
            struct Json_key final
            {
                static constexpr char DURATION[]  = "duration";
                static constexpr char TIMESTAMP[] = "timestamp";
            };

            const nlohmann::json to_json() const;
            std::string timestamp;
            std::string duration;
        };

        struct Error final
        {
            struct Json_key final
            {
                static constexpr char MESSAGE[] = "message";
            };

            Error() = default;
            explicit Error(const std::string& msg);

            const nlohmann::json to_json() const;
            std::string message;
        };

    public:
        struct Json_key final
        {
            static constexpr char REQUEST_ID[] = "request_id";
            static constexpr char STATUS_CODE[] = "status_code";

            static constexpr char ERR[] = "error";
            static constexpr char RESULT[] = "result";

            static constexpr char METADATA[] = "metadata";
        };

    public:
        static Sp sucess(std::uint64_t request_id, std::shared_ptr<Response_result> rr);

    public:
        static Sp client_error(std::uint64_t request_id, Status_code status_code);
        static Sp bad_request(std::uint64_t request_id, const std::string&);
        static Sp not_found(std::uint64_t request_id, const std::string&);
        static Sp internal_server_error(std::uint64_t request_id, const std::string&);
        //...

    public:
        Response(std::uint64_t request_id, int status_code, std::shared_ptr<Error> err);
        Response(std::uint64_t request_id, int status_code, std::shared_ptr<Response_result> rr);

        void set_metadata(std::shared_ptr<Metadata> metadata);
        void update_metada(std::chrono::system_clock::time_point);

        const nlohmann::json to_json() const;

    private:
        std::uint64_t m_request_id;

    private:
        int m_status_code{ (int)Status_code::OK };

        // or
        std::shared_ptr<Error> m_error;
        std::shared_ptr<Response_result> m_result;

        // optional?
    private:
        std::shared_ptr<Metadata> m_metadata;
    };

    using Sp_response = \
        std::shared_ptr<Response>;
}

#endif // RESPONSE_H
