QT += \
    quick \
    core    \
    gui \
    concurrent \
    network \
    widgets \
    qml \
    quickcontrols

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}

TEMPLATE = app

# Specifies project configuration and compiler options.
# The values are recognized internally by qmake and have special meaning
CONFIG += input

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    Model/TomoModel/mode_model.cpp \
    Model/TomoModel/state_model.cpp \
    Script/Config/cfg_app.cpp \
    ViewModel/DashboardVM/dash_board_vm.cpp \
    ViewModel/TomoVM/Feature/mode_tomo_vm.cpp \
    ViewModel/TomoVM/Feature/state_tomo_vm.cpp \
    ViewModel/TomoVM/tomo_vm.cpp \
    ViewModel/controlbar_vm.cpp \
    ViewModel/image_view_vm.cpp \
    ViewModel/image_writer.cpp \
    ViewModel/login_vm.cpp \
    ViewModel/main_vm.cpp \
    ViewModel/master_app.cpp \
    ViewModel/systembar_vm.cpp \
    ViewModel/track_vm.cpp \
    Dialog/modal_dialogbox.cpp \
    Model/image_model.cpp \
    Model/login_model.cpp \
    ViewModel/HeadVM/head_vm.cpp \
    ViewModel/ShipperVM/shipper_vm.cpp \
    ViewModel/SettingVM/setting_vm.cpp \
    ViewModel/SystemLogVM/base_terminal.cpp \
    ViewModel/SystemLogVM/system_log_vm.cpp \
    io_control_vm.cpp \
    ViewModel/IoVM/io_control_vm.cpp \
    ViewModel/ImagingVM/imaging_client_vm.cpp \
    ViewModel/StationVM/station_vm.cpp \
    ViewModel/ProductionVM/production_vm.cpp \
    ViewModel/PimVM/pim_vm.cpp \
    Model/notify_model.cpp \
    ViewModel/NotifyVM/notify_vm.cpp \
    ViewModel/RvizVM/displayconfig.cpp \
    ViewModel/RvizVM/simplegrid.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Model/TomoModel/mode_model.h \
    Model/TomoModel/state_model.h \
    Script/Config/cfg_app.h \
    ViewModel/DashboardVM/dash_board_vm.h \
    ViewModel/TomoVM/Feature/mode_tomo_vm.h \
    ViewModel/TomoVM/Feature/state_tomo_vm.h \
    ViewModel/TomoVM/tomo_vm.h \
    ViewModel/controlbar_vm.h \
    ViewModel/image_view_vm.h \
    ViewModel/image_writer.h \
    ViewModel/login_vm.h \
    ViewModel/main_vm.h \
    ViewModel/master_app.h \
    ViewModel/systembar_vm.h \
    ViewModel/track_vm.h \
    Dialog/modal_dialogbox.h \
    Model/image_model.h \
    Model/login_model.h \
    Script/Define/struct_def.h \
    ViewModel/HeadVM/head_vm.h \
    ViewModel/SettingVM/setting_vm.h \
    ViewModel/SystemLogVM/base_terminal.h \
    ViewModel/SystemLogVM/system_log_vm.h \
    io_control_vm.h \
    ViewModel/IOControl/io_control_vm.h \
    ViewModel/IoVM/io_control_vm.h \
    ViewModel/IoVM/def_modules.h \
    ViewModel/IoVM/def_server.h \
    Script/Define/struct_def.h \
    ViewModel/ImagingVM/imaging_client_vm.h \
    ViewModel/StationVM/station_vm.h \
    ViewModel/IoVM/def_delta.h \
    ViewModel/IoVM/def_modules.h \
    ViewModel/IoVM/def_wago.h \
    ViewModel/IoVM/delta_servo.h \
    ViewModel/ProductionVM/production_vm.h \
    ViewModel/ShipperVM/shipper_vm.h \
    ViewModel/PimVM/pim_vm.h \
    Model/notify_model.h \
    ViewModel/NotifyVM/notify_vm.h \
    ViewModel/RvizVM/displayconfig.h \
    ViewModel/RvizVM/simplegrid.h

#---------------------------------------------------------------- My Libary ---------------------------------------------------------#

