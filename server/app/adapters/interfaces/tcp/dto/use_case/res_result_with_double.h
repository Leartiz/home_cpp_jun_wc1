#ifndef RES_RESULT_WITH_DOUBLE_H
#define RES_RESULT_WITH_DOUBLE_H

#include <memory>

#include <nlohmann/json.hpp>

#include "adapters/interfaces/tcp/dto/response_result.h"

namespace lez::adapters::interfaces::tcp::dto::use_case
{
    class Res_result_with_double : public Response_result
    {
    public:
        struct Json_key
        {
            static constexpr char VALUE[] = "value";
        };

    public:
        using Sp = std::shared_ptr<Res_result_with_double>;
        static Sp create(double value);

    public:
        explicit Res_result_with_double(double value);
        ~Res_result_with_double() = default;

        const nlohmann::json to_json() const override;

    private:
        double m_value;
    };
}

#endif // RES_RESULT_WITH_DOUBLE_H
