TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS +=-msse4.1

SOURCES += \
        main.cpp \
        matrix-mt.cpp \
        matrix-plain.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-thread/release/ -lbeuth-thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-thread/debug/ -lbeuth-thread
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-thread/ -lbeuth-thread

INCLUDEPATH += $$PWD/../../libs/beuth-thread
DEPENDPATH += $$PWD/../../libs/beuth-thread

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/release/ -lbeuth-math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/debug/ -lbeuth-math
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-math/ -lbeuth-math

INCLUDEPATH += $$PWD/../../libs/beuth-math
DEPENDPATH += $$PWD/../../libs/beuth-math

INCLUDEPATH += $$PWD/../../dependencies/Catch2/single_include
