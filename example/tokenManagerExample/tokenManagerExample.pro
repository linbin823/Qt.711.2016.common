#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T13:38:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tokenManagerExample
TEMPLATE = app

include(..\..\src\tokenManager\tokenManager.pri)

SOURCES +=  main.cpp\
            example.cpp \

HEADERS  += example.h \

FORMS    += example.ui
