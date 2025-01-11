#include <sstream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "uuid_utils.h"

namespace lez::uuid_utils
{
    std::string gen()
    {
        static boost::uuids::random_generator gen;
        boost::uuids::uuid uuid = gen();

        std::ostringstream sout; sout << uuid;
        return sout.str();
    }
}
