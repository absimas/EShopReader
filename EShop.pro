#-------------------------------------------------
#
# Project created by QtCreator 2014-11-28T17:07:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EShop
TEMPLATE = app

SOURCES +=\
    backend/Customer.cpp \
    backend/helper.cpp \
    backend/JSONImporter.cpp \
    backend/main_test.cpp \
    backend/Parser.cpp \
    backend/Shop.cpp \
    backend/XMLImporter.cpp \
    backend/XMLExporter.cpp \
    backend/Item.cpp \
    backend/Cart.cpp \
    GUI/MainWindow.cpp \
    GUI/TabHost.cpp \
    GUI/GridItem.cpp \
    GUI/CustomTab.cpp \
    GUI/EbayTab.cpp \
    GUI/CartTab.cpp \
    GUI/CartGridItem.cpp \
    GUI/FadingGridItem.cpp \
    main.cpp

HEADERS  += \
    backend/Customer.h \
    backend/helper.h \
    backend/JSONImporter.h \
    backend/Parser.h \
    backend/Shop.h \
    backend/XMLImporter.h \
    backend/XMLExporter.h \
    backend/Item.h \
    backend/Cart.h \
    GUI/TabHost.h \
    GUI/MainWindow.h \
    GUI/GridItem.h \
    GUI/CustomTab.h \
    GUI/EbayTab.h \
    GUI/CartTab.h \
    GUI/CartGridItem.h \
    GUI/FadingGridItem.h

FORMS    += \
    GUI/MainWindow.ui \
    GUI/GridItem.ui
