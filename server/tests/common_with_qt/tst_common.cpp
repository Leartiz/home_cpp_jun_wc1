#include <any>
#include <string>
#include <sstream>
#include <memory>
#include <format>

#include <QDebug>

#include <lua.hpp>
#include <boost/lexical_cast.hpp>

#include "tst_common.h"

#include "domain/service/math_service.h"
#include "domain/service/impl/lua_math_service.h"

#include "domain/use_case/use_cases.h"
#include "domain/use_case/impl/calc_math_expr_uc_impl.h"

#include "adapters/interfaces/tcp/dto/use_case/req_payload_with_expr.h"
#include "adapters/interfaces/tcp/dto/request.h"
#include "adapters/interfaces/tcp/message_parser.h"

#include "utils/error/error_utils.h"
#include "utils/uuid/uuid_utils.h"

#include "nlohmann/json.hpp"

Q_DECLARE_METATYPE(std::string)

// -----------------------------------------------------------------------

Common::Common() {}
Common::~Common() {}

// Lua Math Service!
// -----------------------------------------------------------------------

void Common::test_Lua_math_calculate_expression_n0()
{
    using namespace lez::domain::service;
    using namespace lez::domain::service::impl;

    std::shared_ptr<contract::Math_service> m = \
        std::make_shared<Lua_math_service>();

    QCOMPARE(m->calculate_expression("5+5"), 10);
}

void Common::test_Lua_math_calculate_expression_n1()
{
    using namespace lez::domain::service;
    using namespace lez::domain::service::impl;

    std::shared_ptr<contract::Math_service> m = \
        std::make_shared<Lua_math_service>();

    QCOMPARE(m->calculate_expression("5*5*3"), 75);
}

// -----------------------------------------------------------------------

void Common::test_Lua_math_calculate_expression_err_n0()
{
    using namespace lez::domain::service;
    using namespace lez::domain::service::impl;

    std::shared_ptr<contract::Math_service> m = \
        std::make_shared<Lua_math_service>();

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             (m->calculate_expression("5 5 5 5")));

    //QVERIFY_THROWS_NO_EXCEPTION((m->calculate_expression("5 5 5 5")));
}

void Common::test_Lua_math_calculate_expression_err_n1()
{
    using namespace lez::domain::service;
    using namespace lez::domain::service::impl;

    std::shared_ptr<contract::Math_service> m = \
        std::make_shared<Lua_math_service>();

    QVERIFY_THROWS_EXCEPTION(std::runtime_error,
                             (m->calculate_expression("5+5; while true do end")));
}

// -----------------------------------------------------------------------

void Common::test_Lua_math_calculate_expression_data()
{
    QTest::addColumn<std::string>("expression");
    QTest::addColumn<double>("result");

    {
        const double result = 5;
        const std::string expression = "2+3";
        QTest::newRow(expression.c_str()) << expression << result;
    }
    {
        const double result = 22;
        const std::string expression = "11*2";
        QTest::newRow(expression.c_str()) << expression << result;
    }
    //...
}

void Common::test_Lua_math_calculate_expression()
{
    QFETCH(std::string, expression);
    QFETCH(double, result);

    // ***

    using namespace lez::domain::service;
    using namespace lez::domain::service::impl;

    std::shared_ptr<contract::Math_service> m = \
        std::make_shared<Lua_math_service>();

    QCOMPARE(m->calculate_expression(expression), result);
}

// -----------------------------------------------------------------------

void Common::test_Payload_with_expr_to_json_n0()
{
    using nlohmann::json;
    using namespace lez::adapters::interfaces::tcp::dto;

    use_case::Req_payload_with_expr pd;
    pd.set_expr("1 + 2 + 3 + 4 + 5");
    const auto j = pd.to_json();

    std::ostringstream sout; sout << j;
    qDebug() << sout.str();
}

void Common::test_Payload_with_expr_from_json_n0()
{
    using nlohmann::json;
    using namespace lez::adapters::interfaces::tcp::dto;

    const std::string expr = "1 + 2 + 3 + 4 + 5";

    use_case::Req_payload_with_expr pd;
    const json j = {
        { "payload", {
              { "expression", expr.c_str() }
          } }
    };
    pd.from_json(j);

    QCOMPARE_EQ(pd.get_expr(), expr);
}

// -----------------------------------------------------------------------

void Common::test_Request_to_json_n0()
{
    using namespace lez::adapters::interfaces::tcp::dto;

    Request r;
    const auto j = r.to_json();
    std::ostringstream sout; sout << j;
    qDebug() << sout.str();
}

void Common::test_Request_to_json_n1()
{
    using nlohmann::json;
    using namespace lez::adapters::interfaces::tcp::dto;
    using namespace lez::domain::use_case;

    Request r;
    r.set_use_case_name(Calc_math_expr_uc::NAME);

    use_case::Req_payload_with_expr pd;
    json j = { { "payload", { { "expression", "1 + 2 + 3 + 4 + 5" } } } };
    pd.from_json(j);
    r.set_payload(std::make_shared<use_case::Req_payload_with_expr>(pd));

    // ***

    j = r.to_json();
    std::ostringstream sout; sout << j;
    qDebug() << sout.str();
}

void Common::test_Request_from_json_n0()
{
    using namespace lez::adapters::interfaces::tcp::dto;

    const std::string json_str = R"({
            "request_id": 12345,
            "use_case": "calculate_math_expression",
            "payload": {
                "expression": "1 + 2 + 3 + 4 + 5"
            }
        }
    )";
    const auto input_j = nlohmann::json::parse(json_str);
    const auto r = Request::from_json(input_j);

    // ***

    const auto output_j = r->to_json();
    std::ostringstream sout; sout << output_j;
    qDebug() << sout.str();
}

// -----------------------------------------------------------------------

void Common::test_error_utils_combine_exceptions_n0()
{
    using namespace lez::error_utils;
    const auto ex = std::runtime_error("bottom some text for error");
    const auto ex1 = std::runtime_error(combine_exceptions("middle some text for error", ex));
    const auto ex2 = std::runtime_error(combine_exceptions("top some text for error", ex1));
    //...

    qDebug() << "An exception occurred:" << ex2.what();
}

void Common::test_uuid_utils_gen_n0()
{
    using namespace lez::uuid_utils;
    for (int i = 0; i < 3; ++i) {
        qDebug() << "uuid value:" << gen();
    }
}

// Message parser
// -----------------------------------------------------------------------

void Common::test_Message_parser_parse_request_n0()
{
    using namespace lez::adapters::interfaces::tcp;
    Message_parser mp;
    mp.reset();

    // ***

    std::string json_bytes = R"({
        "request_id": 12345,
        "use_case": "calculate_math_expression",
        "payload": {
            "expression": "1 + 2 + 3 + 4 + 5"
        }
    })";

    std::uint32_t json_size = std::uint32_t(json_bytes.size());
    std::string size_bytes(4, 0);
    std::memcpy(size_bytes.data(), &json_size, sizeof(json_size));
    std::reverse(size_bytes.begin(), size_bytes.end());

    // ***

    mp.add_to_buffer(std::move(size_bytes));
    mp.add_to_buffer(std::move(json_bytes));

    // ***

    const auto r = mp.parse_request(); // OK!
    qDebug() << r->to_json().dump();
}

void Common::test_Message_parser_parse_request_n1()
{
    // TODO: !
}

// -----------------------------------------------------------------------

void Common::test_Message_parser_parse_request_err_n0()
{
    using namespace lez::adapters::interfaces::tcp;
    Message_parser mp;
    mp.reset();

    // ***

    std::string json_bytes = R"({
        "use_case": "calculate_math_expression",
        "payload": {
            "expression": "1 + 2 + 3 + 4 + 5"
        }
    })";

    std::uint32_t json_size = std::uint32_t(json_bytes.size());
    std::string size_bytes(4, 0);
    std::memcpy(size_bytes.data(), &json_size, sizeof(json_size));
    std::reverse(size_bytes.begin(), size_bytes.end());

    // ***

    mp.add_to_buffer(std::move(size_bytes));
    mp.add_to_buffer(std::move(json_bytes));

    // ***

    QVERIFY_THROWS_EXCEPTION(std::invalid_argument,
        mp.parse_request());
}

void Common::test_Message_parser_parse_request_err_n1()
{
    // TODO: !
}

// Message creator
// -----------------------------------------------------------------------

void Common::test_Message_creator_create_n0()
{
    // TODO: !!!
}

void Common::test_Message_creator_create_n1()
{
    // TODO: !!!
}

// std library
// -----------------------------------------------------------------------

void Common::test_std_any_to_string()
{
    std::any a = std::string("any string");
    std::ostringstream sout;
    sout << std::any_cast<std::string>(a);
    qDebug() << sout.str();
}

void Common::test_std_format()
{
    qDebug() << std::format("str: `{}`", std::string("test_key")).c_str();
    qDebug() << std::format("int: `{}`", 123).c_str();

    qDebug() << std::format("double: `{}`", 123.123).c_str();
    qDebug() << std::format("nullptr: `{}`", nullptr).c_str();
    //...
}

// experiments with some dependencies
// -----------------------------------------------------------------------

void Common::test_lua_calculate_expression()
{
    lua_State* lua_state = luaL_newstate();
    luaL_openlibs(lua_state);

    // ***

    const std::string expression = "3 * (4 + 5) - 2 / 1";
    const std::string lua_code = "result = " + expression;

    if (luaL_dostring(lua_state, lua_code.c_str()) != LUA_OK) {
        std::ostringstream sout;
        sout << "Lua error: " << lua_tostring(lua_state, -1);
        lua_pop(lua_state, 1);
        lua_close(lua_state);

        QFAIL(sout.str().c_str());
    }

    // ***

    lua_getglobal(lua_state, "result");
    if (lua_isnumber(lua_state, -1)) {
        double result = lua_tonumber(lua_state, -1);
        qDebug() << "Result: " << result << '\n'; // !

    } else {
        QFAIL( "Error: result is not a number!");
    }

    lua_pop(lua_state, 1);
    lua_close(lua_state); // !
}

// -----------------------------------------------------------------------

void Common::test_nlohmann_json()
{
    nlohmann::json ex = nlohmann::json::parse(R"(
        {
            "pi": 3.141,
            "happy": true
        }
    )");

    const int pi_val = ex["pi"];
    const bool happy_val = ex["happy"];

    qDebug() << "pi_val:" << pi_val;
    qDebug() << "happy_val:" << happy_val;
}

// -----------------------------------------------------------------------

void Common::test_boost_lexical_cast_data()
{
    QTest::addColumn<std::string>("input");
    QTest::addColumn<bool>("expected");

    // ***

    QTest::newRow("true 1") << std::string("1") << true;
    QTest::newRow("false 0") << std::string("0") << false;

    /*
        QTest::newRow("true") << std::string("true") << true;
        QTest::newRow("false") << std::string("false") << false;
    */
}

void Common::test_boost_lexical_cast()
{
    QFETCH(std::string, input);
    QFETCH(bool, expected);

    QCOMPARE(boost::lexical_cast<bool>(input), expected);
}

QTEST_APPLESS_MAIN(Common)
