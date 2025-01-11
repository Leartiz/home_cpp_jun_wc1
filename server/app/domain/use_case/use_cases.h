#ifndef USE_CASES_H
#define USE_CASES_H

#include <string>
#include <memory>

namespace lez::domain::use_case
{
    // base uc!
    // -------------------------------------------------------------------

    class Use_case
    {
    public:
        virtual ~Use_case() = default;
    };

    // Can be divided into groups!

    // auth (for example!)
    // -------------------------------------------------------------------

    class Sign_up_uc : public Use_case
    {
    public:
        static constexpr char NAME[] = "sign_up";

    public:
        using Sp = std::shared_ptr<Sign_up_uc>;

    public:
        virtual ~Sign_up_uc() = default;

    public:
        virtual void execute(const std::string& username,
                             const std::string& password) = 0;
        virtual std::string get_token() const = 0;
    };

    class Sign_in_uc : public Use_case
    {
    public:
        static constexpr char NAME[] = "sign_in";

    public:
        using Sp = std::shared_ptr<Sign_in_uc>;

    public:
        virtual ~Sign_in_uc() = default;

    public:
        virtual void execute(const std::string& username,
                             const std::string& password) = 0;
        virtual std::string get_token() const = 0;
    };

    // math
    // -------------------------------------------------------------------

    class Calc_math_expr_uc : public Use_case
    {
    public:
        static constexpr char NAME[] = "calculate_math_expression";

    public:
        using Sp = std::shared_ptr<Calc_math_expr_uc>;

    public:
        virtual ~Calc_math_expr_uc() = default;

    public:
        virtual void execute(const std::string& expression) = 0;
        virtual double get_result() const = 0;
    };

    // collection
    // -------------------------------------------------------------------

    struct Use_cases final // pointer set!
    {
        Calc_math_expr_uc::Sp calc_math_expr_uc;
        Sign_up_uc::Sp sign_up_uc;
        Sign_in_uc::Sp sign_in_uc;
        //...
    };
}

#endif // USE_CASES_H
