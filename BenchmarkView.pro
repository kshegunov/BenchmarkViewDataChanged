QT       += core gui widgets widgets-private

TARGET = BenchmarkView
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    benchmarkview.cpp

HEADERS += \
    benchmarkview.h \
    benchmarkmodel.h
