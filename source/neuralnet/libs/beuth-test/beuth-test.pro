TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        math/matrix.cpp

INCLUDEPATH += \
    $$PWD/dependencies/Catch2/single_include \
    $$PWD/../../dependencies/Catch2/single_include \
    $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../beuth-math/release/ -lbeuth-math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../beuth-math/debug/ -lbeuth-math
else:unix: LIBS += -L$$OUT_PWD/../beuth-math/ -lbeuth-math

INCLUDEPATH += $$PWD/../beuth-math
DEPENDPATH += $$PWD/../beuth-math
