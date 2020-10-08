TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ENABLE_BEUTH_PROFILING

QMAKE_CXXFLAGS +=-msse4.1

SOURCES += \
        main.cpp \
        measurement.cpp \
        nosimd.cpp \
        randomizer.cpp \
        simd.cpp \
        variant.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/release/ -lbeuth-math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/debug/ -lbeuth-math
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-math/ -lbeuth-math

INCLUDEPATH += $$PWD/../../libs/beuth-math
DEPENDPATH += $$PWD/../../libs/beuth-math

HEADERS += \
    benchmark.h \
    config.h \
    duration.h \
    evaluator.h \
    measurement.h \
    nosimd.h \
    randomizer.h \
    simd.h \
    variant.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/release/ -lbeuth-profiling
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/debug/ -lbeuth-profiling
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-profiling/ -lbeuth-profiling

INCLUDEPATH += $$PWD/../../libs/beuth-profiling
DEPENDPATH += $$PWD/../../libs/beuth-profiling
