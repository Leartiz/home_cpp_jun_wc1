message("`app_sources.pri` has PWD: $$PWD")

HEADERS += \
    $$PWD/../app/adapters/infrastructure/storage/uc_log/impl/clickhouse/uc_log_clickhouse_storage.h \
    $$PWD/../app/adapters/infrastructure/storage/uc_log/use_case_log_storage.h \
    $$PWD/../app/adapters/interfaces/tcp/client_connection.h \
    $$PWD/../app/adapters/interfaces/tcp/client_context.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/use_case/auth_req_payload.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/use_case/req_payload_with_expr.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/use_case/res_result_with_double.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/request_payload.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/response_result.h \
    $$PWD/../app/adapters/interfaces/tcp/listener.h \
    $$PWD/../app/adapters/interfaces/tcp/message_creator.h \
    $$PWD/../app/adapters/interfaces/tcp/request_context.h \
    $$PWD/../app/adapters/interfaces/tcp/request_handler.h \
    $$PWD/../app/adapters/interfaces/tcp/tcp_common.h \
    $$PWD/../app/config/server_config.h \
    $$PWD/../app/domain/service/impl/lua_math_service.h \
    $$PWD/../app/domain/use_case/impl/calc_math_expr_uc_impl.h \
    $$PWD/../app/logging/impl/boost/main_boost_logger.h \
    $$PWD/../app/domain/service/math_service.h \
    $$PWD/../app/logging/logging.h \
    $$PWD/../app/domain/use_case/use_cases.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/request.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/common_validator.h \
    $$PWD/../app/adapters/interfaces/tcp/message_parser.h \
    $$PWD/../app/utils/error/error_utils.h \
    $$PWD/../app/utils/uuid/uuid_utils.h \
    $$PWD/../app/adapters/interfaces/tcp/dto/response.h

SOURCES += \
    $$PWD/../app/adapters/infrastructure/storage/uc_log/impl/clickhouse/uc_log_clickhouse_storage.cpp \
    $$PWD/../app/adapters/interfaces/tcp/client_context.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/use_case/auth_req_payload.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/use_case/req_payload_with_expr.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/use_case/res_result_with_double.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/response_result.cpp \
    $$PWD/../app/adapters/interfaces/tcp/message_creator.cpp \
    $$PWD/../app/adapters/interfaces/tcp/request_context.cpp \
    $$PWD/../app/adapters/interfaces/tcp/request_handler.cpp \
    $$PWD/../app/adapters/interfaces/tcp/tcp_common.cpp \
    $$PWD/../app/config/server_config.cpp \
    $$PWD/../app/domain/service/impl/lua_math_service.cpp \
    $$PWD/../app/domain/use_case/impl/calc_math_expr_uc_impl.cpp \
    $$PWD/../app/logging/impl/boost/main_boost_logger.cpp \
    $$PWD/../app/logging/logging.cpp \
    $$PWD/../app/adapters/interfaces/tcp/client_connection.cpp \
    $$PWD/../app/adapters/interfaces/tcp/listener.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/common_validator.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/request.cpp \
    $$PWD/../app/adapters/interfaces/tcp/message_parser.cpp \
    $$PWD/../app/utils/error/error_utils.cpp \
    $$PWD/../app/utils/uuid/uuid_utils.cpp \
    $$PWD/../app/adapters/interfaces/tcp/dto/response.cpp
