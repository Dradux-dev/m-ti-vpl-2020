TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/release/ -lbeuth-profiling
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/debug/ -lbeuth-profiling
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/ -lbeuth-profiling

INCLUDEPATH += $$PWD/../../libs/beuth-profiling
DEPENDPATH += $$PWD/../../libs/beuth-profiling
