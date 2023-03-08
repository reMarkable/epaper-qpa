TARGET = epaper
TEMPLATE = lib

CONFIG += plugin
CONFIG += qpa/genericunixfontdatabase c++17
QT += core-private gui-private input_support-private

qtHaveModule(fontdatabase_support-private) {
    QT += fontdatabase_support-private 
}
qtHaveModule(eventdispatcher_support-private) {
    QT += eventdispatcher_support-private
}

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
    map/epaperevdevkeyboardmap_fr.h \
    map/epaperevdevkeyboardmap_no.h \
    map/epaperevdevkeyboardmap_se.h \
    map/epaperevdevkeyboardmap_uk.h \
    map/epaperevdevkeyboardmap_us_rm.h \

OTHER_FILES += minimal.json

target.path += $$[QT_INSTALL_PLUGINS]/platforms
INSTALLS += target

