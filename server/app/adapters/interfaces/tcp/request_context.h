#ifndef REQUEST_CONTEXT_H
#define REQUEST_CONTEXT_H

#include <memory>

#include "dto/request.h"
#include "client_context.h"

namespace lez::adapters::interfaces::tcp
{
    class Request_context final
    {
    public:
        using Sp = std::shared_ptr<Request_context>;
        static Sp create(Client_context::Sp client_ctx,
                         dto::Request::Sp req);

    public:
        Request_context(Client_context::Sp client_ctx,
                        dto::Request::Sp req);

        Client_context::Sp client_ctx() const;
        dto::Request::Sp req() const;

    private:
        Client_context::Sp m_client_ctx;
        dto::Request::Sp m_request;
    };
}

#endif // REQUEST_CONTEXT_H
