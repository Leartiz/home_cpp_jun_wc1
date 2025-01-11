#include <mutex>
#include <stdexcept>

#include <strutil/strutil.h>

#include "logging.h"

namespace lez
{
    namespace logging
    {
        std::string Level_converter::to_str(const Level level)
        {
            switch (level) {
            case Level::trace: return "trace";
            case Level::debug: return "debug";

            case Level::info: return "info";
            case Level::warning: return "warning";

            case Level::error: return "error";
            case Level::fatal: return "fatal";
            }
        }

        Level Level_converter::to_level(const std::string& str)
        {
            const auto lower_str = strutil::to_lower(str);
            if (to_str(Level::trace) == lower_str) return Level::trace;
            if (to_str(Level::debug) == lower_str) return Level::debug;

            if (to_str(Level::info) == lower_str) return Level::info;
            if (to_str(Level::warning) == lower_str) return Level::warning;

            if (to_str(Level::error) == lower_str) return Level::error;
            if (to_str(Level::fatal) == lower_str) return Level::fatal;

            return Level::trace; // !
        }


        // ---------------------------------------------------------------

        std::mutex mx;
        std::shared_ptr<Logger> global_logger = nullptr;

        // ---------------------------------------------------------------

        bool is_empty()
        {
            std::lock_guard<std::mutex> _{ mx };
            return global_logger == nullptr;
        }

        void set_global_logger(std::shared_ptr<Logger> logger)
        {
            if (!is_empty()) {
                throw std::runtime_error{ "global logger already set" };
            }
            if (!logger) {
                throw std::runtime_error{ "logger is nullptr" };
            }

            // ***

            std::lock_guard<std::mutex> _{ mx };
            global_logger = logger;
        }

        std::shared_ptr<Logger> get_logger() // unsafe?
        {
            std::lock_guard<std::mutex> _{ mx };
            return global_logger;
        }

        // ---------------------------------------------------------------

        Logger::Logger(const std::string& name)
            : m_name{ name }
        {}

        void Logger::to(const std::string& message, Level level) const
        {
            switch (level) {
            case Level::trace: trace(message); break;
            case Level::debug: debug(message); break;

            case Level::info: info(message); break;
            case Level::warning: warning(message); break;

            case Level::error: error(message); break;
            case Level::fatal: fatal(message); break;
            }
        }

        // to
        // ---------------------------------------------------------------

        void to(const Level level, const std::string& message)
        {
            global_logger->to(message, level);
        }

        void to(const Level level, const std::ostringstream& sout)
        {
            global_logger->to(sout.str(), level);
        }

        // ---------------------------------------------------------------

        void trace(const std::string& message)
        {
            // no nullptr check!?

            global_logger->trace(message);

            // cppcheck reports unused variable!?
            static_cast<void>(message);
        }
        void debug(const std::string& message)
        {
            global_logger->debug(message);
        }

        void info(const std::string& message)
        {
            global_logger->info(message);
        }
        void warning(const std::string& message)
        {
            global_logger->warning(message);
        }

        void error(const std::string& message)
        {
            global_logger->error(message);
        }
        void fatal(const std::string& message)
        {
            global_logger->fatal(message);
        }
    }
}
