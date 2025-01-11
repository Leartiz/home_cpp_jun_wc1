#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <map>
#include <string>
#include <functional>

#include "dto/response.h"
#include "request_context.h"
#include "domain/use_case/use_cases.h"

namespace lez::adapters::interfaces::tcp
{
    using namespace domain::use_case;

    class Request_handler final
    {
    public:
        explicit Request_handler(const Use_cases&);

    public:
        dto::Response::Sp handle(Request_context::Sp ctx);

        // services/use-cases!
    private:
        const Use_cases m_ucs; // as ptr collection!

    private:
        using route_handler = std::function<dto::Response::Sp(Request_context::Sp ctx)>;
        using route_map = std::map<std::string, route_handler>;
        route_map m_route_map;
    };
}

#endif // REQUEST_HANDLER_H
