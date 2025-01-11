#ifndef MAIN_BOOST_LOGGER_H
#define MAIN_BOOST_LOGGER_H

#include <string>

#include "logging/logging.h"

namespace lez
{
    namespace logging
    {
        namespace impl
        {
            // or trivial?

            class Main_boost_logger final : public Logger
            {
            public:
                struct Params final
                {
                public:
                    Params();

                public:
                    bool enable_file;
                    std::string dir_name;
                    std::string file_name;

                    size_t rotation_size;
                    size_t max_files;

                    Level level;
                };

            public:
                explicit Main_boost_logger(const Params&);

            public:
                void trace(const std::string& message) const override;
                void debug(const std::string& message) const override;

                void info(const std::string& message) const override;
                void warning(const std::string& message) const override;

                void error(const std::string& message) const override;
                void fatal(const std::string& message) const override;

            public:
                ~Main_boost_logger() override = default;
            };
        }
    }
}

#endif // MAIN_BOOST_LOGGER_H
