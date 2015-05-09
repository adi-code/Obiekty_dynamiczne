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

    void select(unsigned int p_i);

    AlgorithmRunner* getAlgorithmRunner();
    AlgorithmSettings* getAlgorithmSettings();

private:
    unsigned int m_current_alg;
    std::vector<std::pair<AlgorithmRunner*, AlgorithmSettings*> > m_algorithms;

signals:

public slots:
};

#endif // ALGORITHMSELECTOR_H
