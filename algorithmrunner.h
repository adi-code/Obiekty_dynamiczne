#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QObject>

#include "algorithmlistener.h"
#include "algorithmsettings.h"

class AlgorithmRunner// : public QObject
{
    //Q_OBJECT
public:
    //explicit AlgorithmRunner(QObject *parent = 0);
    AlgorithmRunner();
    virtual ~AlgorithmRunner();

    virtual void configure(AlgorithmSettings* settings) = 0;
    virtual void setListener(AlgorithmListener* listener) = 0;
    virtual void run() = 0;
    virtual std::vector<std::vector<double> > getResults() = 0;

//signals:

//public slots:

};

#endif // ALGORITHMRUNNER_H
