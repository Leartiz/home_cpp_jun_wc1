#include <stdexcept>

#include "calc_math_expr_uc_impl.h"
#include "logging/logging.h"

namespace lez::domain::use_case::impl
{
    Calc_math_expr_uc_impl::Sp Calc_math_expr_uc_impl::create(
            Math_service::Sp math_service,
            Use_case_log_storage::Sp uc_log_storage)
    {
        return std::make_shared<Calc_math_expr_uc_impl>(
            math_service, uc_log_storage);
    }

    // -------------------------------------------------------------------

    Calc_math_expr_uc_impl::Calc_math_expr_uc_impl(
            Math_service::Sp math_service,
            Use_case_log_storage::Sp uc_log_storage)

        : m_math_service{ math_service }
        , m_uc_log_storage{ uc_log_storage }
    {
        if (!m_math_service) {
            throw std::runtime_error{ "mathematical service is not provided" };
        }
        if (!m_uc_log_storage) {
            throw std::runtime_error{ "uc log storage is not provided" };
        }
    }

    void Calc_math_expr_uc_impl::execute(const std::string& expression)
    {
        try {
            m_result = m_math_service->calculate_expression(expression);

            logging::logf(logging::Level::info, NAME,
                          "successfully calculated expression: '{}', result: {}",
                          expression, m_result);
        }
        catch (const std::runtime_error& e) {

            // may throw an exception!
            m_uc_log_storage->insert_simple(NAME, expression, e.what(),
                                            Use_case_log_storage::FAILURE);

            // ***

            throw e;
        }

        m_uc_log_storage->insert_simple(NAME, expression, std::to_string(m_result),
                                        Use_case_log_storage::SUCCESS); // ok!
    }

    double Calc_math_expr_uc_impl::get_result() const
    {
        return m_result;
    }
}

