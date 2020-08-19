TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/release/ -lbeuth-math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-math/debug/ -lbeuth-math
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-math/ -lbeuth-math

INCLUDEPATH += $$PWD/../../libs/beuth-math
DEPENDPATH += $$PWD/../../libs/beuth-math

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-ai/release/ -lbeuth-ai
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/beuth-ai/debug/ -lbeuth-ai
else:unix: LIBS += -L$$OUT_PWD/../../libs/beuth-ai/ -lbeuth-ai

INCLUDEPATH += $$PWD/../../libs/beuth-ai
DEPENDPATH += $$PWD/../../libs/beuth-ai
