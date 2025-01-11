#include "request_context.h"

namespace lez::adapters::interfaces::tcp
{
    Request_context::Sp Request_context::create(Client_context::Sp client_ctx,
                                                dto::Sp_request req)
    {
        return std::make_shared<Request_context>(client_ctx, req);
    }

    Request_context::Request_context(Client_context::Sp client_ctx,
                                     dto::Sp_request req)
        : m_client_ctx{ client_ctx }
        , m_request{ req }
    {}

    Client_context::Sp Request_context::client_ctx() const
    {
        return m_client_ctx;
    }

    dto::Sp_request Request_context::req() const
    {
        return m_request;
    }
}
