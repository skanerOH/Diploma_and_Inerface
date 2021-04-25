QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Algorithms/HuffLeafCh.cpp \
    Algorithms/HuffmanK.cpp \
    Algorithms/HuffmanS.cpp \
    Algorithms/LZWCoder.cpp \
    Algorithms/LZWDecoder.cpp \
    Algorithms/NumericCoder.cpp \
    Algorithms/NumericDecoder.cpp \
    Algorithms/TypeCompression/Int/IntTypeCoder.cpp \
    Algorithms/TypeCompression/Int/IntTypeDecoder.cpp \
    Algorithms/TypeCompression/UShort/UShortTypeCoder.cpp \
    Algorithms/TypeCompression/UShort/UShortTypeDecoder.cpp \
    CSVBlockCompressor.cpp \
    CSVBlockDecompressor.cpp \
    ColumnAnalyzer.cpp \
    ColumnCompressor.cpp \
    ColumnDecompressor.cpp \
    FileCompressor.cpp \
    FileDecompressor.cpp \
    RecordParser.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Algorithms/HuffLeafCh.h \
    Algorithms/HuffmanK.h \
    Algorithms/HuffmanS.h \
    Algorithms/LZWCoder.h \
    Algorithms/LZWDecoder.h \
    Algorithms/NumericCoder.h \
    Algorithms/NumericDecoder.h \
    Algorithms/TypeCompression/Int/IntTypeCoder.h \
    Algorithms/TypeCompression/Int/IntTypeDecoder.h \
    Algorithms/TypeCompression/UShort/UShortTypeCoder.h \
    Algorithms/TypeCompression/UShort/UShortTypeDecoder.h \
    CSVBlockCompressor.h \
    CSVBlockDecompressor.h \
    ColumnAnalyzer.h \
    ColumnCompressor.h \
    ColumnDecompressor.h \
    FileCompressor.h \
    FileDecompressor.h \
    RecordParser.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CSVCompressor.pro.user
