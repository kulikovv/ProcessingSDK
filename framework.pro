CONFIG +=ordered
TEMPLATE=subdirs


#PROJECT_ROOT = "XXXX"

#CONFIG(debug, debug|release) {
#      LIBS += -L $$OUT_PWD/../../qptk/debug
#      LIBS += -L $$OUT_PWD/../propertyeditor/debug -lPropertyEditor

#}
#CONFIG(release, debug|release) {
#       LIBS += -L $$OUT_PWD/../../qptk/debug
#       LIBS += -L $$OUT_PWD/../propertyeditor/release -lPropertyEditor
#}

SUBDIRS=  qptk/sdk.pro\
          tools\
          samples

include(qptk/config.pri)
TRANSLATIONS    = framework_ru.ts
