#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <sstream>
#include <memory>
#include <format>

namespace lez
{
    namespace logging
    {
        enum class Level {
            trace = 0,
            debug,

            info,
            warning,

            error,
            fatal
        };

        class Level_converter
        {
        public:
            static std::string to_str(const Level level);
            static Level to_level(const std::string& str);
        };

        // ---------------------------------------------------------------

        class Logger
        {
        public:
            static constexpr char DEFAULT_NAME[] = "main";
            explicit Logger(const std::string& name = DEFAULT_NAME);

        public:
            virtual void to(const std::string& message, Level level) const;

            virtual void trace(const std::string& message) const = 0;
            virtual void debug(const std::string& message) const = 0;

            virtual void info(const std::string& message) const = 0;
            virtual void warning(const std::string& message) const = 0;

            virtual void error(const std::string& message) const = 0;
            virtual void fatal(const std::string& message) const = 0;

        public:
            virtual ~Logger() = default;

        protected:
            std::string m_name; // module?
        };

        // ---------------------------------------------------------------

        bool is_empty();
        void set_global_logger(std::shared_ptr<Logger> logger);
        std::shared_ptr<Logger> get_logger();

        // global mt unsafe!?
        // ---------------------------------------------------------------

        void to(const Level level, const std::string& message);
        void to(const Level level, const std::ostringstream& sout);

        void trace(const std::string& message);
        void debug(const std::string& message);

        void info(const std::string& message);
        void warning(const std::string& message);

        void error(const std::string& message);
        void fatal(const std::string& message);

        // ---------------------------------------------------------------

        template <class... Args>
        void to_f(const Level level, const std::string& format,
                  Args&&... args)
        {
            const std::string message = std::vformat(
                format, std::make_format_args(args...));

            to(level, message);
        }

        template <class... Args>
        void trace_f(const std::string& format, Args&&... args)
        {
            to_f(Level::trace, format, args...);
        }
        template <class... Args>
        void debug_f(const std::string& format, Args&&... args)
        {
            to_f(Level::debug, format, args...);
        }

        template <class... Args>
        void info_f(const std::string& format, Args&&... args)
        {
            to_f(Level::info, format, args...);
        }
        template <class... Args>
        void warning_f(const std::string& format, Args&&... args)
        {
            to_f(Level::warning, format, args...);
        }

        template <class... Args>
        void error_f(const std::string& format, Args&&... args)
        {
            to_f(Level::error, format, args...);
        }
        template <class... Args>
        void fatal_f(const std::string& format, Args&&... args)
        {
            to_f(Level::fatal, format, args...);
        }

        // ---------------------------------------------------------------

        template <class... Args>
        void logf(const Level level, const std::string& module_name,
                  const std::string& format, Args&&... args)
        {
            std::string message{ module_name + " | " };

            message += std::vformat(format,
                std::make_format_args(args...));

            to(level, message);
        }
    }
}

#endif // LOGGING_H
