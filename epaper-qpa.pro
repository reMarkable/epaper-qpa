TARGET = epaper
TEMPLATE = lib

CONFIG += plugin
CONFIG += qpa/genericunixfontdatabase c++17
QT += core-private gui-private input_support-private fontdatabase_support-private eventdispatcher_support-private

SOURCES =   main.cpp \
            epaperintegration.cpp \
            epaperbackingstore.cpp \
            epaperevdevkeyboardhandler.cpp \
            epaperevdevkeyboardmanager.cpp
HEADERS =   epaperintegration.h \
            epaperbackingstore.h \
            epaperevdevkeyboardhandler.h \
            epaperevdevkeyboardmanager.h

HEADERS += \
    map/epaperevdevkeyboardmap_de.h \
    map/epaperevdevkeyboardmap_dk.h \
    map/epaperevdevkeyboardmap_es.h \
    map/epaperevdevkeyboardmap_flavor.h \
    map/epaperevdevkeyboardmap_fr.h \
    map/epaperevdevkeyboardmap_no.h \
    map/epaperevdevkeyboardmap_se.h \
    map/epaperevdevkeyboardmap_uk.h \
    map/epaperevdevkeyboardmap_us_rm.h \

OTHER_FILES += minimal.json

target.path += $$[QT_INSTALL_PLUGINS]/platforms
INSTALLS += target

