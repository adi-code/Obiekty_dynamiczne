#ifndef AMGA2ALGORITHMRUNNER_H
#define AMGA2ALGORITHMRUNNER_H

#include <QObject>

#include "algorithmrunner.h"

class Amga2AlgorithmRunner : /*public QObject,*/ public AlgorithmRunner
{
    //Q_OBJECT
public:
    //explicit Amga2AlgorithmRunner(QObject *parent = 0);
    Amga2AlgorithmRunner();
    ~Amga2AlgorithmRunner();

    void configure(AlgorithmSettings* p_settings);
    void setListener(AlgorithmListener* p_listener);
    void run();

//signals:

//public slots:

private:
    void* m_algorithm;
    AlgorithmListener* m_listener;
    AlgorithmSettings* m_settings;
};

#endif // AMGA2ALGORITHMRUNNER_H
