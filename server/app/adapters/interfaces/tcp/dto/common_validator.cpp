#include <stdexcept>

#include "common_validator.h"

namespace lez::adapters::interfaces::tcp::dto
{

    void Common_validator::string_not_empty(const std::string& str,
                                            const std::string& err_message)
    {
        if (str.empty()) {
            throw std::runtime_error(err_message);
        }
    }
}
