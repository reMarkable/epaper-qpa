TARGET = epaper


QT += core-private gui-private platformsupport-private

#LIBS += -lQt5PlatformSupport -lQt5Gui

SOURCES =   main.cpp \
            epaperintegration.cpp \
            epaperbackingstore.cpp
HEADERS =   epaperintegration.h \
            epaperbackingstore.h

OTHER_FILES += minimal.json

CONFIG +=  plugin
CONFIG += qpa/genericunixfontdatabase
#PLUGIN_TYPE = platforms
#PLUGIN_CLASS_NAME = epaperIntegrationPlugin
#!equals(TARGET, $$QT_DEFAULT_QPA_PLUGIN): PLUGIN_EXTENDS = -
# load(qt_plugin)
TEMPLATE = lib
