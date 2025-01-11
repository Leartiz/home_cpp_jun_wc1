#ifndef USE_CASE_LOG_STORAGE_H
#define USE_CASE_LOG_STORAGE_H

#include <memory>
#include <string>

namespace lez
{
    namespace adapters
    {
        namespace infrastructure
        {
            class Use_case_log_storage
            {
            public:

                // as enum?
                static constexpr char SUCCESS[] = "success";
                static constexpr char FAILURE[] = "failure";

            public:
                using Sp = std::shared_ptr<Use_case_log_storage>;

            public:
                virtual ~Use_case_log_storage() = default;

            public:
                virtual void insert_simple(const std::string& uc_name,
                                           const std::string& input_data,
                                           const std::string& output_data,
                                           const std::string& status) = 0;
            };
        }
    }
}


#endif // USE_CASE_LOG_STORAGE_H
