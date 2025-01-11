TEMPLATE = app

# ------------------------------------------------------------------------

CONFIG += c++20
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# ------------------------------------------------------------------------

include(../pri_files/common_deps.pri)
include(../pri_files/app_codes.pri)

# ------------------------------------------------------------------------

SOURCES += \
    main.cpp

OTHER_FILES += \
    $$PWD/../.env
