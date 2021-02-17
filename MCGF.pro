QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mcgf/events/mcgf_events.cpp \
    mcgf/file/mcgf_file.cpp \
    mcgf/network/mcgf_network.cpp \
    mcgf/threads/mcgf_threads.cpp \
    qt/src/main_window.cpp

HEADERS += \
    mcgf/events/mcgf_events.h \
    mcgf/file/mcgf_file.h \
    mcgf/network/mcgf_network.h \
    mcgf/threads/mcgf_threads.h \
    qt/inc/main_window.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    qt/ui/main_window.ui

DISTFILES += \
    .gitignore \
    .vscode/settings.json \
    LICENSE \
    MCGF_zh_CN.ts \
    README.md \
    mcgf.code-workspace \
    test.html
