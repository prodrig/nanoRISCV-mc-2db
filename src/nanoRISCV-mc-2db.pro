QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG -= c++17
#CONFIG += gnu++17
QMAKE_CXXFLAGS += -std=c++17

QMAKE_LIBS += C:\bin\SystemC\lib\libsystemc.a
QMAKE_INCDIR += C:\bin\SystemC\include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#SOURCES += \
#    qt\main.cpp \
#    qt\mainwindow.cpp \
#    qt-sc\sc_config.cpp \
#    qt-sc\sc_debug_object.cpp \
#    qt-sc\sc_qt_adaptor.cpp \
#    qt-sc\sc_run.cpp \
#    qt-sc\scqt_worker.cpp \
#    sc\sc_main.cpp \
#    sc\common\scflexint.cpp \
#    sc\common\scqtregister.cpp \
#    sc\common\scregister.cpp

#HEADERS += \
#    qt\mainwindow.h \
#    sc_config.h \
#    sc_debug_object.h \
#    sc\sc_main.h \
#    sc_run.h \
#    sc_qt_adaptor.h \
#    scqt_worker.h \
#    scqtregister.h \
#    sc\common\scflexint.h \
#    sc\common\scmuxt.h \
#    sc\common\scregister.h \
#    sc\common\scregistert.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    qt/mainwindow.ui

HEADERS += \
    qt-sc/sc_config.h \
    qt-sc/sc_debug_object.h \
    qt-sc/sc_qt_adaptor.h \
    qt-sc/sc_run.h \
    qt-sc/scqt_worker.h \
    qt/mainwindow.h \
    sc/common/sc_arith.h \
    sc/common/sc_clk.h \
    sc/common/sc_flexint.h \
    sc/common/sc_gates.h \
    sc/common/sc_gates_pv.h \
    sc/common/sc_mux.h \
    sc/common/sc_reg.h \
    sc/common/sc_rst.h \
    sc/common/sc_terminals.h \
    sc/common/sc_tri.h \
    sc/sc_main.h

SOURCES += \
    qt-sc/sc_config.cpp \
    qt-sc/sc_debug_object.cpp \
    qt-sc/sc_qt_adaptor.cpp \
    qt-sc/sc_run.cpp \
    qt-sc/scqt_worker.cpp \
    qt/main.cpp \
    qt/mainwindow.cpp \
    sc/sc_main.cpp

QMAKE_INCDIR += \
    qt \
    qt-sc \
    sc \
    sc/common
