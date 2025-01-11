#include <sstream>

#include <boost/lexical_cast.hpp>

#include "server_config.h"

#include "laserpants/dotenv/dotenv.h"

namespace lez
{
    namespace
    {

    }

    // -------------------------------------------------------------------

    const std::uint16_t Config::Default::port = 24444;
    const std::string Config::Default::log_level = "trace";
    const bool Config::Default::enable_logging_to_file = true;

    // -------------------------------------------------------------------

    const Config& Config::instance()
    {
        static Config c;
        return c;
    }

    // -------------------------------------------------------------------

    Config::Config()
    {
        dotenv::init();

        {
            const std::string val = dotenv::getenv(Config::Key::port);
            if (!val.empty()) {
                port = static_cast<std::uint16_t>(std::stoi(val));
            }
            else {
                port = Config::Default::port;
            }
        }

        {
            const std::string val = dotenv::getenv(Config::Key::log_level);
            if (!val.empty()) {
                log_level = val;
            }
            else {
                log_level = Config::Default::log_level;
            }
        }

        {
            const auto val = dotenv::getenv(Config::Key::enable_logging_to_file);
            if (!val.empty()) {
                enable_logging_to_file = boost::lexical_cast<bool>(val);
            }
            else {
                enable_logging_to_file =
                        Config::Default::enable_logging_to_file;
            }
        }
    }

    void Config::log(std::shared_ptr<logging::Logger> logger,
                     logging::Level level) const
    {
        std::ostringstream sout;
        sout << Config::Key::port << ": " << port;
        logger->to(sout.str(), level);
        sout.str("");

        sout << Config::Key::log_level << ": " << log_level;
        logger->to(sout.str(), level);
        sout.str("");

        sout << Config::Key::enable_logging_to_file << ": "
             << enable_logging_to_file;
        logger->to(sout.str(), level);
        sout.str("");
    }
}


