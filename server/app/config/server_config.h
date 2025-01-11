#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <cstdint>
#include <string>

#include "logging/logging.h"

namespace lez
{
    class Config final
    {
    public:
        struct Key final {
            static constexpr char port[] = "PORT";
            static constexpr char log_level[] = "LOG_LEVEL";
            static constexpr char enable_logging_to_file[] = "ENABLE_LOGGING_TO_FILE";
        };

        struct Default final {
            static const std::uint16_t port;
            static const std::string log_level;
            static const bool enable_logging_to_file;
        };

    public:
        static const Config& instance();
        void log(std::shared_ptr<logging::Logger> logger,
                 logging::Level level) const;

    public:
        std::uint16_t port;
        std::string log_level;
        bool enable_logging_to_file;

    private:
        Config();
    };
}

#endif // SERVER_CONFIG_H
