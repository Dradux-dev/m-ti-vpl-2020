TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS +=-msse4.1

SOURCES += \
        main.cpp

DISTFILES += \
    TestRectangle.py \
    classRectangle.py \
    initiale_Tests/PngTestGradient.py \
    initiale_Tests/PngTestGreyscale.py \
    initiale_Tests/PngTestViereck.py \
    test-coloring/TestRectangle.py \
    test-coloring/classRectangle.py
