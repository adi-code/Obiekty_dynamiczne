#include "nsga2algorithmsettings.h"

const unsigned int Nsga2AlgorithmSettings::CROSS_SETTING = 0;
const unsigned int Nsga2AlgorithmSettings::MUT_SETTING = 1;
const unsigned int Nsga2AlgorithmSettings::DI_SETTING = 2;
const unsigned int Nsga2AlgorithmSettings::DIM_SETTING = 3;

Nsga2AlgorithmSettings::Nsga2AlgorithmSettings() : AlgorithmSettings(), m_cross(0.5),
m_mut(0.1), m_di(0.0), m_dim(0.0)
{

}

Nsga2AlgorithmSettings::~Nsga2AlgorithmSettings()
{

}

std::vector<AlgorithmSetting> Nsga2AlgorithmSettings::getSettingsList()
{
    static std::vector<AlgorithmSetting> settings;

    if(settings.empty()) {
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo krzyżowania", 0, 0.0, 1.0, 0.5));
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo mutacji", 0, 0.0, 1.0, 0.1));
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo DI", 0, 0.0, 1.0, 0.5));
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo DIM", 0, 0.0, 1.0, 0.5));
    }

    return settings;
}

void Nsga2AlgorithmSettings::setParameters(const std::vector<double> &p_params)
{
    double* ptrs[] = {&m_cross, &m_mut, &m_di, &m_dim};

    for(unsigned int i=0;i<std::min(p_params.size(), 4U);++i) {
        *ptrs[i] = p_params[i];
    }
}

void *Nsga2AlgorithmSettings::getImpl(const std::string &p_setting)
{
    std::map<std::string, unsigned int> setting_map = {{"CROSS", CROSS_SETTING},
                                                       {"MUT", MUT_SETTING},
                                                       {"DI", DI_SETTING},
                                                       {"DIM", DIM_SETTING}};

    unsigned int setting = 100;

    std::map<std::string, unsigned int>::iterator it = setting_map.find(p_setting);
    if(setting_map.end() != it) {
        setting = it->second;
    }

    switch(setting) {
    case CROSS_SETTING:
        return &m_cross;
    case MUT_SETTING:
        return &m_mut;
    case DI_SETTING:
        return &m_di;
    case DIM_SETTING:
        return &m_dim;
    default:
        return NULL;
    }
}

