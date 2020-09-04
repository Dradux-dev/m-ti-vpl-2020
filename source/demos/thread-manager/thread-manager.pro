TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
		
HEADERS +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-thread/release/ -lbeuth-thread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-thread/debug/ -lbeuth-thread
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-thread/ -lbeuth-thread

INCLUDEPATH += $$PWD/../../libs/beuth-thread
DEPENDPATH += $$PWD/../../libs/beuth-thread
