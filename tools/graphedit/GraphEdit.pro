#-------------------------------------------------
#
# Project created by QtCreator 2011-05-21T18:28:59
#
#-------------------------------------------------

QT       += core widgets gui

TARGET = GraphEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    node.cpp \
    link.cpp \
    editorscene.cpp \
    graphproxyeditor.cpp

HEADERS  += mainwindow.h \
    node.h \
    link.h \
    editorscene.h \
    crossplatform.h \
    graphproxyeditor.h

INCLUDEPATH += "../propertyeditor"
INCLUDEPATH += "../scripteditor"

FORMS    += mainwindow.ui




CONFIG += dirs
RESOURCES += resources.qrc
LEVEL=../
include (../../qptk/config.pri)
LIBS += -L$$quote($$DESTDIR)
LIBS += -lPropertyEditor
LIBS += -lScriptEditor

RC_FILE = images/win.rc
#INCLUDEPATH += "../../qptk"
