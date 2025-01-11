#ifndef ERROR_UTILS_H
#define ERROR_UTILS_H

#include <string>
#include <exception>

namespace lez::error_utils
{
    std::string combine_exceptions(const std::string& text,
                                   const std::exception& e);
}

#endif // ERROR_UTILS_H
