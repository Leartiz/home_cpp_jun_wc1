#ifndef TST_COMMON_H
#define TST_COMMON_H

#include <QtTest>

// add necessary includes here

class Common : public QObject
{
    Q_OBJECT

public:
    Common();
    ~Common();

private slots:
    void test_Lua_math_calculate_expression_n0();
    void test_Lua_math_calculate_expression_n1();

    void test_Lua_math_calculate_expression_err_n0();
    void test_Lua_math_calculate_expression_err_n1();

    void test_Lua_math_calculate_expression_data();
    void test_Lua_math_calculate_expression();

private slots:
    void test_Payload_with_expr_to_json_n0();
    void test_Payload_with_expr_from_json_n0();

private slots:
    void test_Request_to_json_n0();
    void test_Request_to_json_n1();

    void test_Request_from_json_n0();

    // error_utils
private slots:
    void test_error_utils_combine_exceptions_n0();

    // uuid_utils
private slots:
    void test_uuid_utils_gen_n0();

    // Message_parser (tcp)
private slots:
    void test_Message_parser_parse_request_n0();
    void test_Message_parser_parse_request_n1();

    void test_Message_parser_parse_request_err_n0();
    void test_Message_parser_parse_request_err_n1();

    // Message_creator (tcp)
private slots:
    void test_Message_creator_create_n0();
    void test_Message_creator_create_n1();

    // std!

private slots:
    void test_std_any_to_string();
    void test_std_format();

    // some dependencies!

private slots:
    void test_lua_calculate_expression();

private slots:
    void test_nlohmann_json();

private slots:
    void test_boost_lexical_cast_data();
    void test_boost_lexical_cast();
};


#endif // TST_COMMON_H
