#ifndef UC_LOG_CLICKHOUSE_STORAGE_H
#define UC_LOG_CLICKHOUSE_STORAGE_H

#include "../../use_case_log_storage.h"

namespace lez
{
    namespace adapters
    {
        namespace infrastructure
        {
            namespace impl
            {
                class Uc_log_clickhouse_storage final : public Use_case_log_storage
                {
                public:
                    Uc_log_clickhouse_storage();
                    ~Uc_log_clickhouse_storage() override;

                public:
                    void insert_simple(const std::string& uc_name,
                                       const std::string& input_data,
                                       const std::string& output_data,
                                       const std::string& status) override;
                };
            }
        }
    }
}

#endif // UC_LOG_CLICKHOUSE_STORAGE_H
