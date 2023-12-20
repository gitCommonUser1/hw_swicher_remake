QT += quick concurrent xml
CONFIG += c++11

#QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
#QMAKE_CXXFLAGS_STL_ON = /EHa

TRANSLATIONS += english.ts \
                chinese.ts


CONFIG += exceptions

QMAKE_CXXFLAGS_RELEASE += -O3


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include ($$PWD/QSwitcher.pri)

#libs
LIBS += \
     -lrvav_shared -lndi -lzlog -lrkuvc \
     -lm -lpthread -lasound -lavformat -lavdevice -lavcodec -lavfilter -lswresample -lavutil -lSDL2 -lSDL2_ttf -lrga -ldrm \
     -lrockchip_mpp  -lz -lasound -lv4l2 -lv4lconvert -ljpeg -leasymedia -lfdk-aac -ljemalloc -lpng -lmk_api -lfreetype \
     -lcrypto -lssl


SOURCES += \
    audioinput.cpp \
    audiomixer.cpp \
    audiooutput.cpp \
    audiosource.cpp \
    chromaparameters.cpp \
    colorback.cpp \
    colorbacks.cpp \
    dipparameters.cpp \
    downstreamkey.cpp \
    downstreamkeys.cpp \
    fadetoblack.cpp \
    format.cpp \
    gateway.cpp \
    gostreamsystem.cpp \
    ipaddress.cpp \
    key.cpp \
    keyboardevent.cpp \
    keys.cpp \
    language.cpp \
    leftmenumodel.cpp \
    lumaparameters.cpp \
    macro.cpp \
    macropool.cpp \
    macrorecorder.cpp \
    macrorunner.cpp \
    macroworker.cpp \
        main.cpp \
    marker.cpp \
    messagedialogcontrol.cpp \
    micinput.cpp \
    micinputs.cpp \
    mixeffectblock.cpp \
    mixeffectblocks.cpp \
    mixparameters.cpp \
    mvlayout.cpp \
    mvmeter.cpp \
    mvmeters.cpp \
    myprovider.cpp \
    ndi.cpp \
    ndilistmodelitem.cpp \
    network.cpp \
    nexttransition.cpp \
    op.cpp \
    osee_math.cpp \
    outformat.cpp \
    outputcolorspace.cpp \
    outsource.cpp \
    outsources.cpp \
    panel.cpp \
    patternparameters.cpp \
    pipparameters.cpp \
    playback.cpp \
    playbackgroupmanager.cpp \
    preview.cpp \
    primarydns.cpp \
    program.cpp \
    record.cpp \
    regexp.cpp \
    resizeparameters.cpp \
    rightmenumodel.cpp \
    sdimages.cpp \
    secondarydns.cpp \
    setting.cpp \
    settings.cpp \
    control.cpp \
    models.cpp \
    src/datafile.c \
    src/device_info.c \
    src/dhcp_event.cpp \
    src/hdmi_in.c \
    src/hdmi_out.c \
    src/md5.c \
    src/media_sd.cpp \
    src/mp4.c \
    src/ndi_license.c \
    src/net_opr.c \
    src/openssl.c \
    src/pc_cmd.c \
    src/pccmd.cpp \
    src/qthread_connect.cpp \
    src/qthread_route.cpp \
    src/qthread_uevent.cpp \
    src/str2umd.c \
    src/tcp_socket.c \
    src/uvc_out_power.c \
    src/uvc_ports.c \
    src/w25qxx.c \
    srcname.cpp \
    srcnames.cpp \
    srcselection.cpp \
    srcselections.cpp \
    still.cpp \
    stillgenerator.cpp \
    stills.cpp \
    stillselection.cpp \
    stream.cpp \
    streams.cpp \
    subnetmask.cpp \
    supersource.cpp \
    supersourceborder.cpp \
    supersourcemask.cpp \
    supersources.cpp \
    testqml.cpp \
    transitionstyle.cpp \
    wipeparameters.cpp \
    xmlprocessor.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = hw_rockchip_qt

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/freetype2

DEPENDPATH += $$PWD/include/freetype2

HEADERS += \
    AuxCallback.h \
    KeyImplement.h \
    MenuDataType.h \
    MenuEnums.h \
    MenuThirdImplement.h \
    audioinput.h \
    audiomixer.h \
    audiooutput.h \
    audiosource.h \
    chromaparameters.h \
    colorback.h \
    colorbacks.h \
    dipparameters.h \
    downstreamkey.h \
    downstreamkeys.h \
    fadetoblack.h \
    format.h \
    gateway.h \
    gostreamsystem.h \
    include/crc32.h \
    include/data_list.h \
    include/datafile.h \
    include/device_info.h \
    include/dhcp_event.h \
    include/files.h \
    include/freetype2/freetype/config/ftconfig.h \
    include/freetype2/freetype/config/ftheader.h \
    include/freetype2/freetype/config/ftmodule.h \
    include/freetype2/freetype/config/ftoption.h \
    include/freetype2/freetype/config/ftstdlib.h \
    include/freetype2/freetype/config/integer-types.h \
    include/freetype2/freetype/config/mac-support.h \
    include/freetype2/freetype/config/public-macros.h \
    include/freetype2/freetype/freetype.h \
    include/freetype2/freetype/ftadvanc.h \
    include/freetype2/freetype/ftbbox.h \
    include/freetype2/freetype/ftbdf.h \
    include/freetype2/freetype/ftbitmap.h \
    include/freetype2/freetype/ftbzip2.h \
    include/freetype2/freetype/ftcache.h \
    include/freetype2/freetype/ftchapters.h \
    include/freetype2/freetype/ftcid.h \
    include/freetype2/freetype/ftcolor.h \
    include/freetype2/freetype/ftdriver.h \
    include/freetype2/freetype/fterrdef.h \
    include/freetype2/freetype/fterrors.h \
    include/freetype2/freetype/ftfntfmt.h \
    include/freetype2/freetype/ftgasp.h \
    include/freetype2/freetype/ftglyph.h \
    include/freetype2/freetype/ftgxval.h \
    include/freetype2/freetype/ftgzip.h \
    include/freetype2/freetype/ftimage.h \
    include/freetype2/freetype/ftincrem.h \
    include/freetype2/freetype/ftlcdfil.h \
    include/freetype2/freetype/ftlist.h \
    include/freetype2/freetype/ftlogging.h \
    include/freetype2/freetype/ftlzw.h \
    include/freetype2/freetype/ftmac.h \
    include/freetype2/freetype/ftmm.h \
    include/freetype2/freetype/ftmodapi.h \
    include/freetype2/freetype/ftmoderr.h \
    include/freetype2/freetype/ftotval.h \
    include/freetype2/freetype/ftoutln.h \
    include/freetype2/freetype/ftparams.h \
    include/freetype2/freetype/ftpfr.h \
    include/freetype2/freetype/ftrender.h \
    include/freetype2/freetype/ftsizes.h \
    include/freetype2/freetype/ftsnames.h \
    include/freetype2/freetype/ftstroke.h \
    include/freetype2/freetype/ftsynth.h \
    include/freetype2/freetype/ftsystem.h \
    include/freetype2/freetype/fttrigon.h \
    include/freetype2/freetype/fttypes.h \
    include/freetype2/freetype/ftwinfnt.h \
    include/freetype2/freetype/otsvg.h \
    include/freetype2/freetype/t1tables.h \
    include/freetype2/freetype/ttnameid.h \
    include/freetype2/freetype/tttables.h \
    include/freetype2/freetype/tttags.h \
    include/freetype2/ft2build.h \
    include/general.h \
    include/gpio.h \
    include/hdmi_in.h \
    include/hdmi_out.h \
    include/hw_config.h \
    include/list.h \
    include/md5.h \
    include/media_sd.h \
    include/ndi_license.h \
    include/net_opr.h \
    include/openssl.h \
    include/osee_errors.h \
    include/osee_syslog.h \
    include/osee_time.h \
    include/pc_cmd.h \
    include/pccmd.h \
    include/qthread_connect.h \
    include/qthread_route.h \
    include/qthread_uevent.h \
    include/rv_switch_api.h \
    include/str2umd.h \
    include/tcp_socket.h \
    include/uvc_out_power.h \
    include/uvc_ports.h \
    include/w25qxx.h \
    ipaddress.h \
    key.h \
    keyboardevent.h \
    keys.h \
    language.h \
    leftmenumodel.h \
    lumaparameters.h \
    macro.h \
    macropool.h \
    macrorecorder.h \
    macrorunner.h \
    macroworker.h \
    marker.h \
    messagedialogcontrol.h \
    micinput.h \
    micinputs.h \
    mixeffectblock.h \
    mixeffectblocks.h \
    mixparameters.h \
    mp4.h \
    mvlayout.h \
    mvmeter.h \
    mvmeters.h \
    myprovider.h \
    ndi.h \
    ndilistmodelitem.h \
    network.h \
    nexttransition.h \
    op.h \
    osee_math.h \
    outformat.h \
    outputcolorspace.h \
    outsource.h \
    outsources.h \
    panel.h \
    patternparameters.h \
    pipparameters.h \
    playback.h \
    playbackgroupmanager.h \
    preview.h \
    primarydns.h \
    profile_include.h \
    program.h \
    record.h \
    regexp.h \
    resizeparameters.h \
    rightmenumodel.h \
    sdimages.h \
    secondarydns.h \
    setting.h \
    settings.h \
    control.h \
    models.h \
    srcname.h \
    srcnames.h \
    srcselection.h \
    srcselections.h \
    still.h \
    stillgenerator.h \
    stills.h \
    stillselection.h \
    stream.h \
    streams.h \
    subnetmask.h \
    supersource.h \
    supersourceborder.h \
    supersourcemask.h \
    supersources.h \
    testqml.h \
    timeZone.h \
    transitionstyle.h \
    wipeparameters.h \
    xmlprocessor.h

DISTFILES +=
