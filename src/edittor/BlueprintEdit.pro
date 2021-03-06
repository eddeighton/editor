#-------------------------------------------------
#
# Project created by QtCreator 2013-09-13T00:38:11
#
#-------------------------------------------------


QT       += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlueprintEdit
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    blueprintscene.cpp \
    blueprintview.cpp \
    glyphs.cpp \
    blueprintitemmodel.cpp \
    blueprintselectionmodel.cpp \
    blueprintTools.cpp \
    blueprinttoolbox.cpp \
    preview.cpp \
    property.cpp \
    mainwidget.cpp \
    geometryengine.cpp \
    analysisView.cpp
    
HEADERS  += mainwindow.h \
    blueprintscene.h \
    blueprintview.h \
    glyphs.h \
    blueprintitemmodel.h \
    messages.h \
    blueprintselectionmodel.h \
    blueprintTools.h \
    blueprinttoolbox.h \
    preview.h \
    property.h \
    mainwidget.h \
    geometryengine.h \
    analysisView.h

FORMS    += mainwindow.ui \
            preview.ui \
            property.ui

################################################
# cgal path
CGAL_INSTALL_PATH     = W:\root\thirdparty_x64\cgal\install
CGAL_AUX_INSTALL_PATH = W:\root\thirdparty_x64\cgal\downloaded_install
    
################################################
# common stuff
COMMON_PATH = W:\root\common\install

################################################
# ed stuff
ED_PATH = W:\root\ed\install

################################################
# blueprint stuff
BLUEPRINT_PATH = W:\root\blueprint\install
#BLUEPRINT_PATH = W:\root\blueprint\install_releaseSymbols

################################################
# boost stuff
BOOST_PATH = W:\root\thirdparty_x64\boost\install
BOOST_LIB_PATH = $$BOOST_PATH/lib

BOOST_REQUIRED_VERSION = 1.73.0
Boost_USE_STATIC_LIBS = ON
Boost_USE_MULTITHREADED = ON
Boost_USE_STATIC_RUNTIME = OFF
Boost_ARCHITECTURE = "-x64"
Boost_COMPILER = "-vc166"

DEFINES += BOOST_ALL_NO_LIB
INCLUDEPATH += $$BOOST_PATH/include/boost-1_73
DEPENDPATH += $$BOOST_LIB_PATH

################################################
## boost file system
win32:CONFIG(release, debug|release): LIBS += -L$$BOOST_LIB_PATH -llibboost_filesystem-vc166-mt-x64-1_73
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BOOST_LIB_PATH -llibboost_filesystem-vc166-mt-gd-x64-1_73

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$BOOST_LIB_PATH/libboost_filesystem-vc166-mt-x64-1_73.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$BOOST_LIB_PATH/libboost_filesystem-vc166-mt-gd-x64-1_73.lib

################################################
## boost chrono
win32:CONFIG(release, debug|release): LIBS += -L$$BOOST_LIB_PATH -llibboost_chrono-vc166-mt-x64-1_73
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BOOST_LIB_PATH -llibboost_chrono-vc166-mt-gd-x64-1_73

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$BOOST_LIB_PATH/libboost_chrono-vc166-mt-x64-1_73.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$BOOST_LIB_PATH/libboost_chrono-vc166-mt-gd-x64-1_73.lib

################################################
## boost iostreams
win32:CONFIG(release, debug|release): LIBS += -L$$BOOST_LIB_PATH -llibboost_iostreams-vc166-mt-x64-1_73
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BOOST_LIB_PATH -llibboost_iostreams-vc166-mt-gd-x64-1_73

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$BOOST_LIB_PATH/libboost_iostreams-vc166-mt-x64-1_73.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$BOOST_LIB_PATH/libboost_iostreams-vc166-mt-gd-x64-1_73.lib

################################################
## cgal
win32:CONFIG(release, debug|release): LIBS += -L$$CGAL_INSTALL_PATH/lib -lCGAL-vc142-mt-5.1.1-I-900
else:win32:CONFIG(debug, debug|release): LIBS += -L$$CGAL_INSTALL_PATH/lib -lCGAL-vc142-mt-5.1.1-I-900

win32:CONFIG(release, debug|release): LIBS += -L$$CGAL_AUX_INSTALL_PATH/auxiliary/gmp/lib -llibgmp-10
else:win32:CONFIG(debug, debug|release): LIBS += -L$$CGAL_AUX_INSTALL_PATH/auxiliary/gmp/lib -llibgmp-10

win32:CONFIG(release, debug|release): LIBS += -L$$CGAL_AUX_INSTALL_PATH/auxiliary/gmp/lib -llibmpfr-4
else:win32:CONFIG(debug, debug|release): LIBS += -L$$CGAL_AUX_INSTALL_PATH/auxiliary/gmp/lib -llibmpfr-4

INCLUDEPATH += $$CGAL_INSTALL_PATH/include
INCLUDEPATH += $$CGAL_AUX_INSTALL_PATH/auxiliary/gmp/include

#DEPENDPATH += $$ED_PATH/src/blueprint/src/

#QMAKE_LIBDIR += $$CGAL_INSTALL_PATH

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$COMMON_PATH/bin/commonlib.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$COMMON_PATH/bin/commonlibd.lib

#target_link_directories( ${targetname} PRIVATE ${CGAL_LIBRARIES_DIR} )
    
#target_link_libraries( ${targetname} PUBLIC ${CGAL_AUX_INSTALL_PATH}/auxiliary/gmp/lib/libgmp-10.lib )
#target_link_libraries( ${targetname} PUBLIC ${CGAL_AUX_INSTALL_PATH}/auxiliary/gmp/lib/libmpfr-4.lib )
#target_link_libraries( ${targetname} PUBLIC ${CGAL_INSTALL_PATH}/lib/CGAL-vc142-mt-5.1.1-I-900.lib )

################################################
## common lib
win32:CONFIG(release, debug|release): LIBS += -L$$COMMON_PATH/bin -lcommonlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$COMMON_PATH/bin -lcommonlibd

INCLUDEPATH += $$COMMON_PATH/include
#DEPENDPATH += $$ED_PATH/src/blueprint/src/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$COMMON_PATH/bin/commonlib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$COMMON_PATH/bin/commonlibd.lib

################################################
## blueprint lib
win32:CONFIG(release, debug|release): LIBS += -L$$BLUEPRINT_PATH/lib -lblueprintlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BLUEPRINT_PATH/lib -lblueprintlibd

INCLUDEPATH += $$BLUEPRINT_PATH/include
#DEPENDPATH += $$ED_PATH/src/blueprint/src/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$BLUEPRINT_PATH/lib/blueprintlib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$BLUEPRINT_PATH/lib/blueprintlibd.lib

################################################
## ed lib
win32:CONFIG(release, debug|release): LIBS += -L$$ED_PATH/lib -ledlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$ED_PATH/lib -ledlibd
#
INCLUDEPATH += $$ED_PATH/include
#DEPENDPATH += $$PWD/../../../../Simulation/src/root
#
win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$ED_PATH/lib/edlib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$ED_PATH/lib/edlibd.lib

RESOURCES += \
    resources.qrc \
    textures.qrc \
    shaders.qrc

