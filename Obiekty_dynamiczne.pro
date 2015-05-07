TEMPLATE = app
CONFIG += console

QT += widgets

SOURCES += main.cpp \
    dynamicalsystem.cpp \
    dsmatrix.cpp \
    dsidentitymatrix.cpp \
    regulator.cpp \
    mainwindow.cpp \
    algorithmshandler.cpp \
    algorithmrunner.cpp \
    algorithmsettings.cpp \
    algorithmlistener.cpp \
    algorithmselector.cpp \
    amga2/amga2algorithmrunner.cpp \
    amga2/amga2algorithmsettings.cpp \
    function.cpp

HEADERS += \
    dynamicalsystem.h \
    enumerated_types.h \
    dsmatrix.h \
    dsidentitymatrix.h \
    regulator.h \
    mainwindow.h \
    algorithmshandler.h \
    algorithmrunner.h \
    algorithmsettings.h \
    algorithmlistener.h \
    algorithmselector.h \
    amga2/amga2algorithmrunner.h \
    amga2/amga2algorithmsettings.h \
    function.h

# libEvolOptimizer

HEADERS += \
    libEvol/amga2/Individual.h \
    libEvol/amga2/Amga2.h \
    libEvol/emopso/Particle.h \
    libEvol/emopso/emopso.h \
    libEvol/libEvolOptimizer/mtrand.h \
    libEvol/libEvolOptimizer/messages.h \
    libEvol/libEvolOptimizer/ICSingleObjOptimizer.h \
    libEvol/libEvolOptimizer/ICQualityFunction.h \
    libEvol/libEvolOptimizer/ICParticle.h \
    libEvol/libEvolOptimizer/ICNotifier.h \
    libEvol/libEvolOptimizer/ICMultiObjectiveOptimizer.h \
    libEvol/libEvolOptimizer/ICEvolOptimizer.h \
    libEvol/nsga2/CNSGA2Optimizer.h \
    libEvol/omniopt/OmniOptOptimizer.h \

INCLUDEPATH += libEvol/libEvolOptimizer \
                libEvol/emopso \
                libEvol/amga2 \
                libEvol/omniopt \
                libEvol/nsga2


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libEvol/ -lLIB_OPTIM
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libEvol/ -lLIB_OPTIM
else:unix: LIBS += -L$$PWD/libEvol/ -lLIB_OPTIM

INCLUDEPATH += $$PWD/libEvol/
DEPENDPATH += $$PWD/libEvol/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libEvol/libLIB_OPTIM.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libEvol/libLIB_OPTIM.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libEvol/LIB_OPTIM.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libEvol/LIB_OPTIM.lib
else:unix: PRE_TARGETDEPS += $$PWD/libEvol/libLIB_OPTIM.a
