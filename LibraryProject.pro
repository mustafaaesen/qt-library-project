QT += core gui widgets sql

TEMPLATE = app
TARGET = LibraryProject
INCLUDEPATH += .

# Input
HEADERS += \
    mainwindow.h \
    kitapislemleri.h \
    oduncalmaislemleri.h \
    oduncteslimislemleri.h \
    uyeislemleri.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    kitapislemleri.cpp \
    oduncalmaislemleri.cpp \
    oduncteslimislemleri.cpp \
    uyeislemleri.cpp

FORMS += \
    mainwindow.ui \
    kitapislemleri.ui \
    oduncalmaislemleri.ui \
    oduncteslimislemleri.ui \
    uyeislemleri.ui

RESOURCES += Resources.qrc

CONFIG += c++17