#include "algorithmsettings.h"

//AlgorithmSettings::AlgorithmSettings(QObject *parent) : QObject(parent)
AlgorithmSettings::AlgorithmSettings()
{

}

AlgorithmSettings::~AlgorithmSettings()
{

}

template <typename T>
T AlgorithmSettings::get(unsigned int i) {
    return *static_cast<T*>(getImpl(i));
}
