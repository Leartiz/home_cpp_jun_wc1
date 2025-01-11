# ------------------------------------------------------------------------

INCLUDEPATH += \
    $$PWD/../dependency \ # json, strutil
    $$PWD/../dependency/clickhouse \
    $$PWD/../dependency/clickhouse/absl \
    $$PWD/../dependency/clickhouse/cityhash \
    $$PWD/../dependency/clickhouse/lz4 \
    $$PWD/../dependency/clickhouse/zstd \
    $$PWD/../dependency/laserpants \
    $$PWD/../dependency/clickhouse/contrib/absl

# ------------------------------------------------------------------------

win32: { # msvc!

    LUA_VERSION = 54
    LUA_PATH = $$getenv(LUA_PATH)
    BOOST_PATH = $$getenv(BOOST_PATH)

# ------------------------------------------------------------------------

    CLICKHOUSE_BUILD_PATH = $$PWD/../dependency/clickhouse/build
    CLICKHOUSE_LIBRARY_PATH = $$PWD/../dependency/clickhouse/build/clickhouse
    CLICKHOUSE_CONTRIB_PATH = $$CLICKHOUSE_BUILD_PATH/contrib

    ABSL_BUILD_PATH =     $$CLICKHOUSE_CONTRIB_PATH/absl/absl
    CITYHASH_BUILD_PATH = $$CLICKHOUSE_CONTRIB_PATH/cityhash/cityhash
    LZ4_BUILD_PATH =      $$CLICKHOUSE_CONTRIB_PATH/lz4/lz4
    ZSTD_BUILD_PATH =     $$CLICKHOUSE_CONTRIB_PATH/zstd/zstd

    message("ABSL_BUILD_PATH: $$ABSL_BUILD_PATH")
    message("CITYHASH_BUILD_PATH: $$CITYHASH_BUILD_PATH")
    message("LZ4_BUILD_PATH: $$LZ4_BUILD_PATH")
    message("ZSTD_BUILD_PATH: $$ZSTD_BUILD_PATH")
    #...

# ------------------------------------------------------------------------

    INCLUDEPATH += \
        $$BOOST_PATH \
        $$LUA_PATH\include

    LIBS += -lws2_32 -lmswsock

    LIBS += -L"$$LUA_PATH" -llua$$LUA_VERSION
    LIBS += -L"$$BOOST_PATH/stage/lib"

    CONFIG(debug, debug|release) {
        message("Building in Debug mode")

        LIBS += -L"$$CLICKHOUSE_LIBRARY_PATH/Debug"
        LIBS += -lclickhouse-cpp-lib

        LIBS += -L"$$ABSL_BUILD_PATH/Debug"
        LIBS += -labsl_int128

        LIBS += -L"$$CITYHASH_BUILD_PATH/Debug"
        LIBS += -lcityhash

        LIBS += -L"$$LZ4_BUILD_PATH/Debug"
        LIBS += -llz4

        LIBS += -L"$$ZSTD_BUILD_PATH/Debug"
        LIBS += -lzstdstatic
    } else {
        message("Building in Release mode")

        LIBS += -L"$$CLICKHOUSE_LIBRARY_PATH/Release"
        LIBS += -lclickhouse-cpp-lib

        LIBS += -L"$$ABSL_BUILD_PATH/Release"
        LIBS += -labsl_int128

        LIBS += -L"$$CITYHASH_BUILD_PATH/Release"
        LIBS += -lcityhash

        LIBS += -L"$$LZ4_BUILD_PATH/Release"
        LIBS += -llz4

        LIBS += -L"$$ZSTD_BUILD_PATH/Release"
        LIBS += -lzstdstatic
    }

# only release!

} else: unix: {
    INCLUDEPATH += \
        /usr/include

    LIBS += -L/usr/lib64 -llua-5.4

    BOOST_INCLUDE_DIR=$$getenv(BOOST_INCLUDE_DIR)
    BOOST_LIBRARY_DIR=$$getenv(BOOST_LIBRARY_DIR)

    message("BOOST_INCLUDE_DIR: $$BOOST_INCLUDE_DIR")
    message("BOOST_LIBRARY_DIR: $$BOOST_LIBRARY_DIR")

    INCLUDEPATH += $$BOOST_INCLUDE_DIR

    # Why doesn't this work?
    # LIBS += -L"$$BOOST_LIBRARY_DIR"
    LIBS += \
        $$BOOST_LIBRARY_DIR/libboost_system.a \
        $$BOOST_LIBRARY_DIR/libboost_thread.a \
        $$BOOST_LIBRARY_DIR/libboost_log_setup.a \
        $$BOOST_LIBRARY_DIR/libboost_log.a \
        $$BOOST_LIBRARY_DIR/libboost_filesystem.a

    LIBS += -lpthread

    # clickhouse (and it dependencies)

    CLICKHOUSE_BUILD_PATH = $$PWD/../dependency/clickhouse/build
    CLICKHOUSE_LIBRARY_PATH = $$PWD/../dependency/clickhouse/build/clickhouse
    CLICKHOUSE_CONTRIB_PATH = $$CLICKHOUSE_BUILD_PATH/contrib

    LIBS += \
        $$CLICKHOUSE_LIBRARY_PATH/libclickhouse-cpp-lib.a

    ABSL_LIBRARY_PATH =     $$CLICKHOUSE_CONTRIB_PATH/absl/absl
    CITYHASH_LIBRARY_PATH = $$CLICKHOUSE_CONTRIB_PATH/cityhash/cityhash
    LZ4_LIBRARY_PATH =      $$CLICKHOUSE_CONTRIB_PATH/lz4/lz4
    ZSTD_LIBRARY_PATH =     $$CLICKHOUSE_CONTRIB_PATH/zstd/zstd

    LIBS += \
        $$ABSL_LIBRARY_PATH/libabsl_int128.a \
        $$CITYHASH_LIBRARY_PATH/libcityhash.a \
        $$LZ4_LIBRARY_PATH/liblz4.a \
        $$ZSTD_LIBRARY_PATH/libzstdstatic.a
}

# ------------------------------------------------------------------------
