#ifndef CALC_MATH_EXPR_UC_IMPL_H
#define CALC_MATH_EXPR_UC_IMPL_H

#include <memory>

#include "domain/use_case/use_cases.h"

#include "domain/service/math_service.h"
#include "adapters/infrastructure/storage/uc_log/use_case_log_storage.h"

namespace lez::domain::use_case::impl
{
    using namespace service::contract;
    using namespace adapters::infrastructure;

    class Calc_math_expr_uc_impl : public Calc_math_expr_uc
    {
    public:
        using Sp = std::shared_ptr<Calc_math_expr_uc_impl>;
        static Sp create(Math_service::Sp math_service,
                         Use_case_log_storage::Sp uc_log_storage);

    public:
        Calc_math_expr_uc_impl(Math_service::Sp math_service,
                               Use_case_log_storage::Sp uc_log_storage);
        ~Calc_math_expr_uc_impl() override = default;

    public:
        void execute(const std::string& expression) override;
        double get_result() const override;

    public:
        Math_service::Sp m_math_service;
        Use_case_log_storage::Sp m_uc_log_storage;

    public:
        double m_result{ 0.0 };
    };
}

#endif // CALC_MATH_EXPR_UC_IMPL_H
