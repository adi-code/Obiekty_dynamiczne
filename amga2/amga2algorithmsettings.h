#ifndef AMGA2ALGORITHMSETTINGS_H
#define AMGA2ALGORITHMSETTINGS_H

#include <QObject>

#include "algorithmsettings.h"

class Amga2AlgorithmSettings : /*public QObject, */public AlgorithmSettings
{
    //Q_OBJECT
public:
    //explicit Amga2AlgorithmSettings(QObject *parent = 0);
    Amga2AlgorithmSettings();
    ~Amga2AlgorithmSettings();

protected:
    void* getImpl(unsigned int i);

//signals:

//public slots:
};

#endif // AMGA2ALGORITHMSETTINGS_H
