#ifndef NSGA2ALGORITHMRUNNER_H
#define NSGA2ALGORITHMRUNNER_H

#include "algorithmrunner.h"

class Nsga2AlgorithmRunner : public AlgorithmRunner
{
public:
    Nsga2AlgorithmRunner();
    ~Nsga2AlgorithmRunner();
    void configure(AlgorithmSettings* p_settings);
    void setListener(AlgorithmListener* p_listener);
    void run();
    std::vector<std::vector<double> > getResults();

private:
    void* m_algorithm;
    AlgorithmListener* m_listener;
    AlgorithmSettings* m_settings;
};

#endif // NSGA2ALGORITHMRUNNER_H
