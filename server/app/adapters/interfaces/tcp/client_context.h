#ifndef CLIENT_CONTEXT_H
#define CLIENT_CONTEXT_H

#include <memory>
#include <string>

namespace lez::adapters::interfaces::tcp
{
    // Request_context separately for each request!

    class Client_context final
    {
    public:
        using Sp = std::shared_ptr<Client_context>;
        static Sp create(const std::string& client_id);

    public:
        explicit Client_context(
            const std::string& client_id);

    private:
        std::string m_client_id;
    };

    using Sp_client_ctx = \
        std::shared_ptr<Client_context>;
}

#endif // CLIENT_CONTEXT_H
