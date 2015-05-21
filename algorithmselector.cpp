#include "algorithmselector.h"

#include "amga2/amga2algorithmrunner.h"
#include "amga2/amga2algorithmsettings.h"
#include "emopso/emopsoalgorithmrunner.h"
#include "emopso/emopsoalgorithmsettings.h"
#include "nsga2/nsga2algorithmrunner.h"
#include "nsga2/nsga2algorithmsettings.h"

AlgorithmSelector::AlgorithmSelector(QObject *parent) : QObject(parent), m_current_alg(0)
{
    // AMGA2
    m_algorithms.emplace_back(
                std::make_pair<AlgorithmRunner*, AlgorithmSettings*>(
                    new Amga2AlgorithmRunner(), new Amga2AlgorithmSettings()));
    // EMOPSO
    m_algorithms.emplace_back(
                std::make_pair<AlgorithmRunner*, AlgorithmSettings*>(
                    new EmopsoAlgorithmRunner(), new EmopsoAlgorithmSettings()));
    // NSGA2
    m_algorithms.emplace_back(
                std::make_pair<AlgorithmRunner*, AlgorithmSettings*>(
                    new Nsga2AlgorithmRunner(), new Nsga2AlgorithmSettings()));
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
        delete m_algorithms[m_current_alg].first;
        m_algorithms[m_current_alg].first = NULL;
        if(m_current_alg == 0)
        {
            m_algorithms[m_current_alg].first = new Amga2AlgorithmRunner();
        }
        else if(m_current_alg == 1)
        {
            m_algorithms[m_current_alg].first = new EmopsoAlgorithmRunner();
        }
        else if(m_current_alg == 2)
        {
            m_algorithms[m_current_alg].first = new Nsga2AlgorithmRunner();
        }
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


