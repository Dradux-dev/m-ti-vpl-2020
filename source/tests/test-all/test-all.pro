TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS +=-msse4.1

SOURCES += \
        ../test-math/matrix-mt.cpp \
        ../test-math/matrix-plain.cpp \
        ../test-thread/batch.cpp \
        ../test-thread/counting-semaphore.cpp \
        ../test-thread/thread-manager-job.cpp \
        ../test-thread/thread-manager-jobresult.cpp \
        ../test-thread/thread-manager.cpp \
        ../test-thread/thread.cpp \
        ../test-thread/threadpool.cpp \
        main.cpp

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
