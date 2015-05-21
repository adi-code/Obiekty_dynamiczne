TEMPLATE = app
CONFIG += console

# enable C++11
CONFIG += c++11
# disable boost/multi_array/concept_checks.hpp warnings
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs

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
    function.cpp \
    controltimefunction.cpp \
    functionevaluator.cpp \
    dynamicsystemfunctionevaluator.cpp \
    overshootfunction.cpp \
    squareerrorintegralfunction.cpp \
    emopso/emopsoalgorithmsettings.cpp \
    emopso/emopsoalgorithmrunner.cpp \
    nsga2/nsga2algorithmsettings.cpp \
    nsga2/nsga2algorithmrunner.cpp \
    libEvol/amga2/Amga2.cpp \
    libEvol/amga2/Individual.cpp \
    libEvol/amga2/RandomNumberGenerator.cpp \
    libEvol/emopso/efile.cpp \
    libEvol/emopso/emopso.cpp \
    libEvol/emopso/Particle.cpp \
    libEvol/libEvolOptimizer/ICEvolOptimizer.cpp \
    libEvol/libEvolOptimizer/ICMultiObjectiveOptimizer.cpp \
    libEvol/libEvolOptimizer/ICQualityFunction.cpp \
    libEvol/libEvolOptimizer/ICSingleObjOptimizer.cpp \
    libEvol/libEvolOptimizer/messages.cpp \
    libEvol/libEvolOptimizer/mtrand.cpp \
    libEvol/nsga2/CNSGA2Optimizer.cpp \
    libEvol/nsga2/NSGA2.cpp \
    libEvol/omniopt/OmniOptOptimizer.cpp \
    libEvol/omniopt/priv_omniopt/allocate.cpp \
    libEvol/omniopt/priv_omniopt/crossover.cpp \
    libEvol/omniopt/priv_omniopt/crowddist.cpp \
    libEvol/omniopt/priv_omniopt/decode.cpp \
    libEvol/omniopt/priv_omniopt/display.cpp \
    libEvol/omniopt/priv_omniopt/dominance.cpp \
    libEvol/omniopt/priv_omniopt/eval.cpp \
    libEvol/omniopt/priv_omniopt/fillnds.cpp \
    libEvol/omniopt/priv_omniopt/global.cpp \
    libEvol/omniopt/priv_omniopt/initialize.cpp \
    libEvol/omniopt/priv_omniopt/input.cpp \
    libEvol/omniopt/priv_omniopt/list.cpp \
    libEvol/omniopt/priv_omniopt/merge.cpp \
    libEvol/omniopt/priv_omniopt/mutation.cpp \
    libEvol/omniopt/priv_omniopt/output.cpp \
    libEvol/omniopt/priv_omniopt/pdef.cpp \
    libEvol/omniopt/priv_omniopt/pdefinit.cpp \
    libEvol/omniopt/priv_omniopt/rand.cpp \
    libEvol/omniopt/priv_omniopt/rank.cpp \
    libEvol/omniopt/priv_omniopt/report.cpp \
    libEvol/omniopt/priv_omniopt/sampleprob1.cpp \
    libEvol/omniopt/priv_omniopt/sampleprob2.cpp \
    libEvol/omniopt/priv_omniopt/sampleprob3.cpp \
    libEvol/omniopt/priv_omniopt/sampleprob4.cpp \
    libEvol/omniopt/priv_omniopt/sampleprob5.cpp \
    libEvol/omniopt/priv_omniopt/sort.cpp \
    libEvol/omniopt/priv_omniopt/tourselect.cpp \
    libEvol/omniopt/priv_omniopt/utility.cpp

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
    function.h \
    controltimefunction.h \
    functionevaluator.h \
    dynamicsystemfunctionevaluator.h \
    overshootfunction.h \
    squareerrorintegralfunction.h \
    emopso/emopsoalgorithmsettings.h \
    emopso/emopsoalgorithmrunner.h \
    nsga2/nsga2algorithmsettings.h \
    nsga2/nsga2algorithmrunner.h \
    libEvol/amga2/Amga2.h \
    libEvol/amga2/Individual.h \
    libEvol/amga2/RandomNumberGenerator.h \
    libEvol/emopso/efile.h \
    libEvol/emopso/emopso.h \
    libEvol/emopso/Particle.h \
    libEvol/libEvolOptimizer/ICEvolOptimizer.h \
    libEvol/libEvolOptimizer/ICMultiObjectiveOptimizer.h \
    libEvol/libEvolOptimizer/ICNotifier.h \
    libEvol/libEvolOptimizer/ICParticle.h \
    libEvol/libEvolOptimizer/ICQualityFunction.h \
    libEvol/libEvolOptimizer/ICSingleObjOptimizer.h \
    libEvol/libEvolOptimizer/messages.h \
    libEvol/libEvolOptimizer/mtrand.h \
    libEvol/nsga2/CNSGA2Optimizer.h \
    libEvol/nsga2/NSGA2.h \
    libEvol/omniopt/OmniOptOptimizer.h \
    libEvol/omniopt/priv_omniopt/global.h \
    libEvol/omniopt/priv_omniopt/pdef.h \
    libEvol/omniopt/priv_omniopt/rand.h

# libEvolOptimizer

INCLUDEPATH += libEvol/libEvolOptimizer \
               libEvol/emopso \
               libEvol/amga2 \
               libEvol/omniopt \
               libEvol/nsga2

