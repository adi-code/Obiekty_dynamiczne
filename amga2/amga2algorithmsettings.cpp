#include "amga2algorithmsettings.h"

const unsigned int Amga2AlgorithmSettings::CROSS_SETTING = 0;
const unsigned int Amga2AlgorithmSettings::MUT_SETTING = 1;
const unsigned int Amga2AlgorithmSettings::DI_SETTING = 2;
const unsigned int Amga2AlgorithmSettings::DIM_SETTING = 3;
const unsigned int Amga2AlgorithmSettings::ARCHIVE_SIZE_SETTING = 4;

//Amga2AlgorithmSettings::Amga2AlgorithmSettings(QObject *parent) : AlgorithmSettings(parent)//QObject(parent)
Amga2AlgorithmSettings::Amga2AlgorithmSettings() : AlgorithmSettings(), m_cross(0.5),
    m_mut(0.1), m_di(0.0), m_dim(0.0), m_archive_size(10)
{

}

Amga2AlgorithmSettings::~Amga2AlgorithmSettings()
{

}

void* Amga2AlgorithmSettings::getImpl(const std::string& p_setting) {
    std::map<std::string, unsigned int> setting_map = {{"CROSS", CROSS_SETTING},
                                                       {"MUT", MUT_SETTING},
                                                       {"DI", DI_SETTING},
                                                       {"DIM", DIM_SETTING},
                                                       {"ARCHIVE_SIZE", ARCHIVE_SIZE_SETTING}};

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
    case ARCHIVE_SIZE_SETTING:
        return &m_archive_size;
    default:
        return NULL;
    }
}

std::vector<AlgorithmSetting> Amga2AlgorithmSettings::getSettingsList() {
    static std::vector<AlgorithmSetting> settings;

    if(settings.empty()) {
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo krzyżowania", 0, 0.0, 1.0, 0.5));
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo mutacji", 0, 0.0, 1.0, 0.1));
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo DI", 0, 0.0, 1.0, 0.5));
        settings.emplace_back(AlgorithmSetting("Prawdopodobieństwo DIM", 0, 0.0, 1.0, 0.5));
        settings.emplace_back(AlgorithmSetting("Wielkość archiwum", 1, 10, 10000, 100));
    }

    return settings;
}

void Amga2AlgorithmSettings::setParameters(const std::vector<double> &p_params) {
    double* ptrs[] = {&m_cross, &m_mut, &m_di, &m_dim};

    for(unsigned int i=0;i<std::min(p_params.size(), 4U);++i) {
        *ptrs[i] = p_params[i];
    }

    if(p_params.size() >= 5) {
        m_archive_size = p_params[4];
    }
}
