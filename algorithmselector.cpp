#include "algorithmselector.h"

#include "amga2/amga2algorithmrunner.h"
#include "amga2/amga2algorithmsettings.h"

AlgorithmSelector::AlgorithmSelector(QObject *parent) : QObject(parent)
{

}

AlgorithmSelector::~AlgorithmSelector()
{

}

AlgorithmRunner* AlgorithmSelector::getAlgorithmRunner() {
    return new Amga2AlgorithmRunner();
}

AlgorithmSettings* AlgorithmSelector::getAlgorithmSettings() {
    return new Amga2AlgorithmSettings();
}


