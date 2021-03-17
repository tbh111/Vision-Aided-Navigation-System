QT  += core gui
QT  += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
CONFIG += debug_and_release
CONFIG(release,debug|release) {
    INCLUDEPATH += D:/opencv_build/opencv-qt/install/include
    LIBS += D:/opencv_build/opencv-qt/install/x64/mingw/bin/libopencv_*.dll
} else {
    INCLUDEPATH += D:/opencv_build/opencv-qt/debug_install/install/include
    LIBS += D:/opencv_build/opencv-qt/debug_install/install/x64/mingw/bin/libopencv_*.dll
}

