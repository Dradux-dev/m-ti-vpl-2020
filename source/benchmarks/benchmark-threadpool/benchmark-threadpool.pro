TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        async.cpp \
        main.cpp \
        pool.cpp \
        single-pool.cpp

HEADERS += \
    async.h \
    config.h \
    pool.h \
    single-pool.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-thread/release/ -lbeuth-thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-thread/debug/ -lbeuth-thread
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-thread/ -lbeuth-thread

INCLUDEPATH += $$PWD/../../libs/beuth-thread
DEPENDPATH += $$PWD/../../libs/beuth-thread

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-benchmark/release/ -lbeuth-benchmark
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-benchmark/debug/ -lbeuth-benchmark
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-benchmark/ -lbeuth-benchmark

INCLUDEPATH += $$PWD/../../libs/beuth-benchmark
DEPENDPATH += $$PWD/../../libs/beuth-benchmark
