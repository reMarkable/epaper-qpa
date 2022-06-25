TARGET = epaper
TEMPLATE = lib

CONFIG += plugin
CONFIG += qpa/genericunixfontdatabase
QT += core-private gui-private input_support-private fontdatabase_support-private eventdispatcher_support-private

SOURCES =   main.cpp \
            epaperintegration.cpp \
            epaperbackingstore.cpp \
            epaperevdevkeyboardhandler.cpp \
            epaperevdevkeyboardmanager.cpp
HEADERS =   epaperintegration.h \
            epaperbackingstore.h \
            epaperevdevkeyboard_defaultmap.h \
            epaperevdevkeyboardhandler.h \
            epaperevdevkeyboardmanager.h

OTHER_FILES += minimal.json

target.path += $$[QT_INSTALL_PLUGINS]/platforms
INSTALLS += target

