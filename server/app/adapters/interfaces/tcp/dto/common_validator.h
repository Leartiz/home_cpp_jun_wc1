#ifndef COMMON_VALIDATOR_H
#define COMMON_VALIDATOR_H

#include <string>

namespace lez::adapters::interfaces::tcp::dto
{
    class Common_validator final
    {
        // with exceptions!
    public:
        static void string_not_empty(const std::string& str,
                                     const std::string& err_message);
    };
}

#endif // COMMON_VALIDATOR_H
