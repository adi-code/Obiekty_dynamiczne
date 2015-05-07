#include "amga2algorithmsettings.h"

//Amga2AlgorithmSettings::Amga2AlgorithmSettings(QObject *parent) : AlgorithmSettings(parent)//QObject(parent)
Amga2AlgorithmSettings::Amga2AlgorithmSettings() : AlgorithmSettings()
{

}

Amga2AlgorithmSettings::~Amga2AlgorithmSettings()
{

}

void* Amga2AlgorithmSettings::getImpl(unsigned int i) {
    int* x = new int;
    *x = 7;
    return x;
}
