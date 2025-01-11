#include <sstream>

#include "error_utils.h"

namespace lez::error_utils
{
    std::string combine_exceptions(const std::string& text,
                                    const std::exception& e)
    {
        std::ostringstream sout;
        sout << text << "\n" << "Nested exception: "
             << e.what();
        return sout.str();
    }
}
