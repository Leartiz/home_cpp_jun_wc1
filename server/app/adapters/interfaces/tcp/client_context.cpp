#include "client_context.h"

namespace lez::adapters::interfaces::tcp
{
    Client_context::Sp Client_context::create(const std::string& client_id)
    {
        return std::make_shared<Client_context>(client_id);
    }

    Client_context::Client_context(const std::string& client_id)
        : m_client_id{ client_id }
    {}
}

