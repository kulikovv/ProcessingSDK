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
          tools

win32-msvc* {
    !contains(QMAKE_TARGET.arch, x86_64) {
            message("x86 build")

            ## Windows x86 (32bit) specific build here
            SUBDIRS +=samples

        } else {
            message("x86_64 build")

            ## Windows x64 (64bit) specific build here
            SUBDIRS +=samples
        }

}

include(qptk/config.pri)
TRANSLATIONS    = framework_ru.ts
