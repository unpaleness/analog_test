#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T11:44:40
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -Wall -Wextra

QT += gui \
      widgets
TARGET = test.bin
TEMPLATE = app

DESTDIR = ../
QMAKE_LIBDIR += ../
OBJECTS_DIR = obj
MOC_DIR = moc

INCLUDEPATH += . \
               ../analogwidgets

LIBS += -lanalogwidgets \
        -L.
#win32 {
#    LIBS += "e:/MY_WORK/DEVELOP/uspduser/TableDensity.dll"
#    LIBS += "D:/!!/NISE104/uspduser/TableDensity.dll"
#}
#unix:!macx {
#    LIBS += "/lib/libTableDensity.so"
#}


SOURCES += main.cpp \
           runner.cpp

HEADERS += runner.h

