#include <sstream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions.hpp>

#include "main_boost_logger.h"

namespace lez
{
    namespace logging
    {
        namespace
        {
            using boost::log::trivial::severity_level;

            severity_level convert_level(const Level level)
            {
                switch (level) {
                case Level::trace: return severity_level::trace;
                case Level::debug: return severity_level::debug;

                case Level::info: return severity_level::info;
                case Level::warning: return severity_level::warning;

                case Level::error: return severity_level::error;
                case Level::fatal: return severity_level::fatal;
                }

                return severity_level::trace; // default?
            }
        }

        namespace impl
        {
            Main_boost_logger::Params::Params()
                : enable_file(true), dir_name("logs"), file_name("actions")
                , rotation_size(5 * 1024 * 1024), max_files(5)
                , level(Level::trace) {}

            // -----------------------------------------------------------

            Main_boost_logger::Main_boost_logger(const Params& options)
            {
                namespace attr = boost::log::attributes;
                namespace expr = boost::log::expressions;

                const auto format = expr::stream
                    << expr::format_date_time<boost::posix_time::ptime>(
                        "TimeStamp", "[%d.%m.%Y %H:%M:%S.%f]") << " "
                    << "[" << boost::log::trivial::severity << "] "

                    << "[" << expr::attr<attr::current_thread_id::value_type>("ThreadID") << "] "
                    << ": " << expr::smessage;

                // ***

                boost::log::add_console_log(
                    std::cout, boost::log::keywords::format = format); // ?

                if (options.enable_file) {
                    std::ostringstream full_file_name;
                    full_file_name << options.dir_name << "/"
                                   << options.file_name << "_%N.log";

                    auto file_sink = boost::log::add_file_log(
                        boost::log::keywords::open_mode = std::ios::app,
                        boost::log::keywords::file_name = full_file_name.str(),
                        boost::log::keywords::rotation_size = options.rotation_size,
                        boost::log::keywords::max_files = options.max_files,
                        boost::log::keywords::format = format);
                    file_sink->locked_backend()->auto_flush(); // !
                }

                // ***

                boost::log::add_common_attributes();

                boost::log::core::get()->set_filter(
                    boost::log::trivial::severity >= convert_level(options.level));
            }

            // -----------------------------------------------------------

            void Main_boost_logger::trace(const std::string& message) const
            {
                BOOST_LOG_TRIVIAL(trace) << message;
            }
            void Main_boost_logger::debug(const std::string& message) const
            {
                BOOST_LOG_TRIVIAL(debug) << message;
            }

            void Main_boost_logger::info(const std::string& message) const
            {
                BOOST_LOG_TRIVIAL(info) << message;
            }
            void Main_boost_logger::warning(const std::string& message) const
            {
                BOOST_LOG_TRIVIAL(warning) << message;
            }

            void Main_boost_logger::error(const std::string& message) const
            {
                BOOST_LOG_TRIVIAL(error) << message;
            }
            void Main_boost_logger::fatal(const std::string& message) const
            {
                BOOST_LOG_TRIVIAL(fatal) << message;
            }
        }
    }
}
