#pragma once

#include <string>

#include <lua.hpp>

#include "domain/service/math_service.h"

namespace lez
{
    namespace domain
    {
        namespace service
        {
            namespace impl
            {
                class Lua_math_service : public contract::Math_service
                {
                public:
                    Lua_math_service();
                    ~Lua_math_service() override;

                public:
                    double calculate_expression(const std::string&) override;

                private:
                    lua_State* m_lua_state;
                };
            }
        }
    }
}
