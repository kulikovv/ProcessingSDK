TARGET = %ProjectName:c%
TEMPLATE = lib


DEFINES += %ProjectName:u%_LIBRARY

SOURCES += %ProjectName:l%.cpp

HEADERS += %ProjectName:l%.h\
        %ProjectName:l%_global.h

CONFIG += plugin 

@if "%Useopencv%" == "true"
CONFIG += opencv
@endif

@if "%Usekinect%" == "true"
CONFIG += kinect
@endif

include(%EthoStudioDir%/EthoStudioSDK/ethostudioconfig.pri )