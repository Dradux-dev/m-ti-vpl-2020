TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ENABLE_BEUTH_PROFILING

QMAKE_CXXFLAGS +=-msse4.1

SOURCES += \
        main.cpp \
        nosimd.cpp \
        randomizer.cpp \
        simd.cpp

HEADERS += \
    config.h \
    nosimd.h \
    randomizer.h \
    simd.h \

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/release/ -lbeuth-math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/debug/ -lbeuth-math
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-math/ -lbeuth-math

INCLUDEPATH += $$PWD/../../libs/beuth-math
DEPENDPATH += $$PWD/../../libs/beuth-math

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/release/ -lbeuth-profiling
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/debug/ -lbeuth-profiling
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/ -lbeuth-profiling

INCLUDEPATH += $$PWD/../../libs/beuth-profiling
DEPENDPATH += $$PWD/../../libs/beuth-profiling

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-benchmark/release/ -lbeuth-benchmark
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-benchmark/debug/ -lbeuth-benchmark
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-benchmark/ -lbeuth-benchmark

INCLUDEPATH += $$PWD/../../libs/beuth-benchmark
DEPENDPATH += $$PWD/../../libs/beuth-benchmark
