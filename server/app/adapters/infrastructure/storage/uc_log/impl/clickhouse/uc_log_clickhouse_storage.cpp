#include <iostream>
#include <stdexcept>

#include "uc_log_clickhouse_storage.h"

#include "clickhouse/client.h"
#include "logging/logging.h"

namespace lez
{
    namespace adapters
    {
        namespace infrastructure
        {
            namespace impl
            {
                using namespace clickhouse;

                // ctor & dtor
                // -------------------------------------------------------

                Uc_log_clickhouse_storage::Uc_log_clickhouse_storage()
                {
                    try {
                        Client client(ClientOptions()
                                      .SetHost("localhost")
                                      .SetPassword("12345678"));

                        client.Ping();
                        logging::info("clickhouse is ready to use");
                    }
                    catch(const std::exception& e) {
                        logging::info(std::string("err: ") + e.what());
                    }
                }

                Uc_log_clickhouse_storage::~Uc_log_clickhouse_storage()
                {

                }

                // -------------------------------------------------------

                void Uc_log_clickhouse_storage::insert_simple(const std::string& uc_name,
                                                              const std::string& input_data,
                                                              const std::string& output_data,
                                                              const std::string& status)
                {

                }
            }
        }
    }
}
