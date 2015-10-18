# Plik utworzony przez mened?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./analogwidgets/code
# Cel to biblioteka analogwidgets
#RESOURCES += analogwidgets.qrc 

QMAKE_CXXFLAGS += -Wall -Wextra

HEADERS += \
           ../analogwidgets/functions.h \
           ../analogwidgets/manometer.h \
           ../analogwidgets/thermometer.h \
           ../analogwidgets/abstractmeter.h \
           ../analogwidgets/widgetwithbackground.h \
           defines.h \
           item.h

SOURCES += \
           functions.cpp \
           manometer.cpp \
           thermometer.cpp \
           abstractmeter.cpp \
           widgetwithbackground.cpp

# Plik utworzony przez menad?era projektów kdevelopa

INCLUDEPATH += . \
               ../analogwidgets

include (../Flags.pri )
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = ../
TARGET = analogwidgets
CONFIG += warn_on \
          staticlib
TEMPLATE = lib


headers.files += ../analogwidgets/*.h
headers.path += $$INSTALL_INCLUDEDIR/analogwidgets

staticlib.files += ../libanalogwidgets.a 
staticlib.path += $$INSTALL_LIBDIR/ 


INSTALLS += headers \
            staticlib


WIN32{
  DEFINES+= WIN32
}
