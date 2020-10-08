CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../dependencies/jsoncpp/src/lib_json/json_reader.cpp \
    ../../dependencies/jsoncpp/src/lib_json/json_value.cpp \
    ../../dependencies/jsoncpp/src/lib_json/json_writer.cpp

HEADERS += \
    ../../dependencies/jsoncpp/include/json/allocator.h \
    ../../dependencies/jsoncpp/include/json/assertions.h \
    ../../dependencies/jsoncpp/include/json/config.h \
    ../../dependencies/jsoncpp/include/json/forwards.h \
    ../../dependencies/jsoncpp/include/json/json.h \
    ../../dependencies/jsoncpp/include/json/json_features.h \
    ../../dependencies/jsoncpp/include/json/reader.h \
    ../../dependencies/jsoncpp/include/json/value.h \
    ../../dependencies/jsoncpp/include/json/version.h \
    ../../dependencies/jsoncpp/include/json/writer.h \
    ../../dependencies/jsoncpp/src/lib_json/json_tool.h \
    ../../dependencies/jsoncpp/src/lib_json/json_valueiterator.inl

INCLUDEPATH += ../../dependencies/jsoncpp/include

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
