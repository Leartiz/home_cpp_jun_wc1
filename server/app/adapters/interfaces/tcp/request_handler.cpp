#include <memory>

#include "request_handler.h"

#include "dto/use_case/req_payload_with_expr.h"
#include "dto/use_case/res_result_with_double.h"

namespace lez::adapters::interfaces::tcp
{
    Request_handler::Request_handler(const Use_cases& services)
        : m_ucs{ services }
    {
        m_route_map[Calc_math_expr_uc::NAME] =
            [this](Request_context::Sp ctx) -> dto::Response::Sp {

            const auto r = ctx->req();
            const auto pd = r->get_payload();

            if (!pd) {
                return dto::Response::bad_request(
                    r->get_request_id(), "payload is nullptr");
            }

            using namespace dto::use_case;
            auto cpd = std::dynamic_pointer_cast<Req_payload_with_expr>(pd);
            if (!cpd) {
                return dto::Response::bad_request(
                    r->get_request_id(), "payload is of a different type");
            }

            // ***

            try {
                m_ucs.calc_math_expr_uc->execute(cpd->get_expr());
                const auto uc_result = m_ucs.calc_math_expr_uc->get_result();


                return dto::Response::sucess(r->get_request_id(),
                                             Res_result_with_double::create(uc_result));
            }
            catch(const std::runtime_error& e) {

                return dto::Response::bad_request(
                    r->get_request_id(), e.what());
            }
        };
        //...
    }

    dto::Response::Sp Request_handler::handle(Request_context::Sp ctx)
    {
        auto r = ctx->req();
        const auto& uc_name = r->get_use_case_name();

        if (!m_route_map.contains(uc_name)) {

            // it is already detected at early stages!

            return dto::Response::not_found(
                r->get_request_id(), "not found");
        }
        // ?

        return m_route_map[uc_name](ctx);
    }
}
