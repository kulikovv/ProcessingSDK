#-------------------------------------------------
#
# Project created by QtCreator 2011-07-10T13:13:29
#
#-------------------------------------------------

TARGET = PropertyEditor
TEMPLATE = lib
greaterThan(QT_MAJOR_VERSION,4){
    QT += widgets
}

DEFINES += PROPERTYEDITOR_LIBRARY

SOURCES += QVariantDelegate.cpp \
    QPropertyModel.cpp \
    QPropertyEditorWidget.cpp \
    Property.cpp \
    EnumProperty.cpp \
    ColorCombo.cpp \
    PropertyTabWidget.cpp \
    PropertyDialog.cpp \
    PropertyStackedWidget.cpp \
    PropertyConfigurationWidget.cpp \
    properyagregator.cpp

HEADERS += QVariantDelegate.h \
    QPropertyModel.h \
    QPropertyEditorWidget.h \
    Property.h \
    EnumProperty.h \
    ColorCombo.h \
    PropertyEditor_global.h \
    PropertyTabWidget.h \
    PropertyDialog.h \
    PropertyStackedWidget.h \
    PropertyConfigurationWidget.h \
    properyagregator.h


CONFIG += dirs
LEVEL=../
include (../../qptk/config.pri)
