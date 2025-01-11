#pragma once

#include <string>
#include <memory>

namespace lez
{
    namespace domain
    {
        namespace service
        {
            namespace contract
            {
                class Math_service
                {
                public:
                    using Sp = std::shared_ptr<Math_service>;

                public:
                    static constexpr char SERVICE_NAME[] = "math"; // !

                public:
                    virtual double calculate_expression(const std::string&) = 0;
                    virtual ~Math_service() = default;
                };

                using Sp_math_service = \
                    std::shared_ptr<Math_service>;
            }
        }
    }
}

