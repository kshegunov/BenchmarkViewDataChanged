QT       += core gui widgets widgets-private

TARGET = BenchmarkView
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    benchmarkview.cpp \
    benchmarkmodel.cpp

HEADERS += \
    benchmarkview.h \
    benchmarkmodel.h

FORMS += \
    benchmarkview.ui
