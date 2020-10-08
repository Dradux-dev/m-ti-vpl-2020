CONFIG += qt
QT += core
QT -= gui

QMAKE_CXXFLAGS +=-msse4.1

TEMPLATE = lib

CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# The follow define enables profiling for the math library. Disable
# profiling by mark the define as commentary within this file.
DEFINES += ENABLE_BEUTH_PROFILING

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    beuthmath.cpp

HEADERS += \
    beuth-math_global.h \
    beuthmath.h \
    binary.hpp \
    linear.hpp \
    matrix.hpp \
    non-linear.hpp \
    simd.hpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../beuth-profiling/release/ -lbeuth-profiling
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../beuth-profiling/debug/ -lbeuth-profiling
else:unix: LIBS += -L$$OUT_PWD/../beuth-profiling/ -lbeuth-profiling

INCLUDEPATH += $$PWD/../beuth-profiling
DEPENDPATH += $$PWD/../beuth-profiling
