#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T16:24:37
#
#-------------------------------------------------

QT       += script gui
greaterThan(QT_MAJOR_VERSION,4){
    QT += widgets
}


TARGET = ScriptEditor
TEMPLATE = lib

DEFINES += SCRIPTEDITOR_LIBRARY

SOURCES += scripteditor.cpp \
    scriptworker.cpp \
    scripttexteditor.cpp \
    scripthighlight.cpp \
    linecounter.cpp

HEADERS += scripteditor.h\
        ScriptEditor_global.h \
    scriptworker.h \
    scripttexteditor.h \
    scripthighlight.h \
    linecounter.h


LEVEL=../
include (../../qptk/config.pri)
message(quote($$DESTDIR))
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
