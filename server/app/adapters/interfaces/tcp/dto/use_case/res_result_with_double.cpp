#include "res_result_with_double.h"

namespace lez::adapters::interfaces::tcp::dto::use_case
{
    Res_result_with_double::Sp Res_result_with_double::create(double value)
    {
        return std::make_shared<Res_result_with_double>(value);
    }

    Res_result_with_double::Res_result_with_double(double value)
        : m_value{ value }
    {}

    const nlohmann::json Res_result_with_double::to_json() const
    {
        nlohmann::json j;
        j[Json_key::VALUE] = m_value;
        return j;
    }
}
