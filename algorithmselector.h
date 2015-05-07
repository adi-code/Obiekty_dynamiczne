#ifndef ALGORITHMSELECTOR_H
#define ALGORITHMSELECTOR_H

#include <QObject>

#include "algorithmrunner.h"
#include "algorithmsettings.h"

class AlgorithmSelector : public QObject
{
    Q_OBJECT
public:
    explicit AlgorithmSelector(QObject *parent = 0);
    ~AlgorithmSelector();

    AlgorithmRunner* getAlgorithmRunner();
    AlgorithmSettings* getAlgorithmSettings();

signals:

public slots:
};

#endif // ALGORITHMSELECTOR_H
