#include <sstream>
#include <stdexcept>

#include <lua.hpp>

#include "domain/service/impl/lua_math_service.h"

namespace lez
{
    namespace domain
    {
        namespace service
        {
            namespace impl
            {
                namespace
                {
                    void validate_expression(const std::string& expression)
                    {
                        const size_t new_line_pos = expression.find('\n');
                        const size_t semicolon_pos = expression.find(';');

                        if (new_line_pos != std::string::npos || semicolon_pos != std::string::npos) {
                            throw std::runtime_error{
                                "invalid character found in expression"
                            };
                        }
                    }
                } // <anonymous>

                // -----------------------------------------------------------

                Lua_math_service::Lua_math_service()
                    : m_lua_state{ luaL_newstate() }
                {
                    if (!m_lua_state) {
                        throw std::runtime_error{ "failed to create lua state" };
                    }

                    luaL_openlibs(m_lua_state);
                }

                Lua_math_service::~Lua_math_service()
                {
                    lua_close(m_lua_state); // !
                }

                // -----------------------------------------------------------

                // if lua-err then `std::runtime_error` as client err?!

                double Lua_math_service::calculate_expression(const std::string& expression)
                {
                    validate_expression(expression);

                    const std::string lua_code = "result = " + expression;
                    const auto status_value = luaL_dostring(m_lua_state, lua_code.c_str());
                    if (status_value != LUA_OK) {
                        std::ostringstream sout;
                        sout << "lua code execution failed with error: " <<
                                lua_tostring(m_lua_state, -1);
                        lua_pop(m_lua_state, 1); // !
                        throw std::runtime_error{ sout.str() };
                    }

                    lua_getglobal(m_lua_state, "result");
                    if (lua_isnumber(m_lua_state, -1)) {
                        const double result = lua_tonumber(m_lua_state, -1); // ok!
                        lua_pop(m_lua_state, 1);
                        return result; // ok!
                    }

                    lua_pop(m_lua_state, 1);
                    throw std::runtime_error{
                        "expression/result is not a number"
                    };
                }
            }
        }
    }
}
