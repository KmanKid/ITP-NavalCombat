QT       += core gui
QT       += network
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gridcell.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    gridcell.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc

DISTFILES += \
    assets/grid_cell_empty.png \
    assets/grid_cell_hit.png \
    assets/grid_cell_miss.png \
    assets/grid_cell_on_hover.png \
    assets/gridcell_blue_full.png \
    assets/gridcell_blue_full_preview.png \
    assets/gridcell_blue_hit.png \
    assets/gridcell_red_full.png \
    assets/gridcell_red_hit.png \
    assets/logo_nc.png \
    assets/n_c_icon.png \
    assets/nc_icon.png \
    assets/nc_logo.png \
    assets/ship_destroyed_horizontal.png \
    assets/ship_destroyed_vertical.png \
    assets/ships/blue_ship_1.png \
    assets/ships/blue_ship_2.png \
    assets/ships/blue_ship_3.png \
    assets/ships/blue_ship_4.png \
    assets/ships/red_ship_1.png \
    assets/ships/red_ship_2.png \
    assets/ships/red_ship_3.png \
    assets/ships/red_ship_4.png \
    assets/ships/size_four_blue.png \
    assets/ships/size_four_red.png \
    assets/ships/size_one_blue.png \
    assets/ships/size_one_red.png \
    assets/ships/size_three_blue.png \
    assets/ships/size_three_red.png \
    assets/ships/size_two_blue.png \
    assets/ships/size_two_red.png \
    assets/vs.png
