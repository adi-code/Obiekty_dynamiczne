#ifndef EMOPSOALGORITHMRUNNER_H
#define EMOPSOALGORITHMRUNNER_H

#include "algorithmrunner.h"

class EmopsoAlgorithmRunner : public AlgorithmRunner
{
public:
    EmopsoAlgorithmRunner();
    ~EmopsoAlgorithmRunner();

    void configure(AlgorithmSettings* p_settings);
    void setListener(AlgorithmListener* p_listener);
    void run();
    std::vector<std::vector<double> > getResults();

private:
    void* m_algorithm;
    AlgorithmListener* m_listener;
    AlgorithmSettings* m_settings;
};

#endif // EMOPSOALGORITHMRUNNER_H
