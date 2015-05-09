#include "algorithmselector.h"

#include "amga2/amga2algorithmrunner.h"
#include "amga2/amga2algorithmsettings.h"

AlgorithmSelector::AlgorithmSelector(QObject *parent) : QObject(parent), m_current_alg(0)
{
    // AMGA2
    m_algorithms.emplace_back(
                std::make_pair<AlgorithmRunner*, AlgorithmSettings*>(
                    new Amga2AlgorithmRunner(), new Amga2AlgorithmSettings()));
}

AlgorithmSelector::~AlgorithmSelector()
{
    for(unsigned int i=0;i<m_algorithms.size();++i) {
        delete m_algorithms[i].first;
        delete m_algorithms[i].second;
    }
}

AlgorithmRunner* AlgorithmSelector::getAlgorithmRunner() {
    if(m_algorithms.size() > m_current_alg) {
        return m_algorithms[m_current_alg].first;
    }

    return NULL;
}

AlgorithmSettings* AlgorithmSelector::getAlgorithmSettings() {
    if(m_algorithms.size() > m_current_alg) {
        return m_algorithms[m_current_alg].second;
    }

    return NULL;
}

void AlgorithmSelector::select(unsigned int p_i) {
    m_current_alg = p_i;
}


