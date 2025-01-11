#include <memory>
#include <string>
#include <exception>

#include <boost/asio.hpp>

#include "config/server_config.h"

#include "logging/logging.h"
#include "logging/impl/boost/main_boost_logger.h"

#include "domain/use_case/use_cases.h"
#include "domain/use_case/impl/calc_math_expr_uc_impl.h"

#include "domain/service/math_service.h"
#include "domain/service/impl/lua_math_service.h"

#include "adapters/infrastructure/storage/uc_log/use_case_log_storage.h"
#include "adapters/infrastructure/storage/uc_log/impl/clickhouse/uc_log_clickhouse_storage.h"

#include "adapters/interfaces/tcp/listener.h"

using namespace lez;

// -----------------------------------------------------------------------

void initialize_logging(const Config& config)
{
    using Main_boost_logger =
        logging::impl::Main_boost_logger;

    Main_boost_logger::Params params;
    params.level = logging::Level_converter::to_level(config.log_level);

    //...

    auto logger = std::make_shared<Main_boost_logger>(params);
    logging::set_global_logger(logger);
}

void test_logging()
{
    logging::trace("trace");
    logging::debug("debug");

    logging::info("info");
    logging::warning("warning");

    logging::error("error");
    logging::fatal("fatal");

    // ***

    logging::trace_f("trace {}", "f");
    logging::debug_f("debug {}", "f");

    logging::info_f("info {}", "f");
    logging::warning_f("warning {}", "f");

    logging::error_f("error {}", "f");
    logging::fatal_f("fatal {}", "f");
}

// -----------------------------------------------------------------------

void run_io_context(boost::asio::io_context& ioc) {
    try {
        ioc.run();
    } catch (const std::exception& e) {
        logging::fatal_f("run io context failed. With err: {}", e.what());
        exit(-1);
    }
}


int main() /* or wrap in a class: App */ 
{
    using namespace domain::use_case;
    using namespace domain::use_case::impl;

    using namespace domain::service::contract;
    using namespace domain::service::impl;

    // ***

    using namespace adapters::infrastructure;
    using namespace adapters::infrastructure::impl;

    // ***

    using adapters::interfaces::tcp::Listener;

	try {

        /* config */

        const auto c = Config::instance();

        /* logger */

        initialize_logging(c);
        test_logging();

        c.log(logging::get_logger(),
              logging::Level::trace);

        /* infrastructure */

        const auto uc_log_storage = std::make_shared<Uc_log_clickhouse_storage>();

        /* services and ucs */

        const auto math_service = std::make_shared<Lua_math_service>();
        //...

        const domain::use_case::Use_cases ucs{
            .calc_math_expr_uc = Calc_math_expr_uc_impl::create(
                math_service, uc_log_storage),
            //...
        };


        /* interfaces */

        boost::asio::io_context ioc; // framework!?
        const Listener listener(ioc, ucs, c.port);

        // ***

        using executor_type = boost::asio::io_context::executor_type;
        boost::asio::executor_work_guard<executor_type> work_guard =
            boost::asio::make_work_guard(ioc);

        const size_t num_threads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back(run_io_context,
                                 std::ref(ioc));
        }

        // ***

        for (auto& thread : threads) {
            thread.join();
        }
	}
    catch (const std::exception& ex) {
        std::stringstream sout; sout << "err: " << ex.what();
        logging::error(sout.str());
	}
}
