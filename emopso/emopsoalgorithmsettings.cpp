#include "emopsoalgorithmsettings.h"

const unsigned int EmopsoAlgorithmSettings::SOCIAL_ACC_SETTING = 0;
const unsigned int EmopsoAlgorithmSettings::COGNITIVE_ACC_SETTING = 1;
const unsigned int EmopsoAlgorithmSettings::INERTIA_SETTING = 2;
const unsigned int EmopsoAlgorithmSettings::FLY_TIME_SETTING = 3;
const unsigned int EmopsoAlgorithmSettings::CLUSTERS_SETTING = 4;
const unsigned int EmopsoAlgorithmSettings::ARCHIVE_SIZE_SETTING = 5;


EmopsoAlgorithmSettings::EmopsoAlgorithmSettings() :
    AlgorithmSettings(),
    m_social_acc(1.5),
    m_cognitive_acc(1.5),
    m_inertia(0.4),
    m_fly_time(5),
    m_max_archive_size(100),
    m_number_of_clusters(5)
{

}

EmopsoAlgorithmSettings::~EmopsoAlgorithmSettings()
{

}

std::vector<AlgorithmSetting> EmopsoAlgorithmSettings::getSettingsList()
{
    static std::vector<AlgorithmSetting> settings;

    if(settings.empty()) {
        settings.emplace_back(AlgorithmSetting("Parametr socjalny", 0, 0.0, 10.0, 1.5));
        settings.emplace_back(AlgorithmSetting("Parametr kognitywny", 0, 0.0, 10.0, 1.5));
        settings.emplace_back(AlgorithmSetting("Bezwładność", 0, 0.0, 1.0, 0.4));
        settings.emplace_back(AlgorithmSetting("Czas pojedynczego ruchu", 1, 1, 20, 5));
        settings.emplace_back(AlgorithmSetting("Liczba podpopulacji", 1, 2, 20, 5));
        settings.emplace_back(AlgorithmSetting("Wielkość archiwum", 1, 10, 10000, 100));
    }

    return settings;
}

void EmopsoAlgorithmSettings::setParameters(const std::vector<double> &p_params)
{
//    double* ptrs[] = {&m_social_acc, &m_cognitive_acc, &m_inertia, &m_fly_time, &m_number_of_clusters};

//    for(unsigned int i=0;i<std::min(p_params.size(), 5U);++i) {
//        *ptrs[i] = p_params[i];
//    }

//    if(p_params.size() >= 6) {
//        m_archive_size = p_params[5];
//    }

    m_social_acc = p_params[0];
    m_cognitive_acc = p_params[1];
    m_inertia = p_params[2];
    m_fly_time = p_params[3];
    m_number_of_clusters = p_params[4];
    m_max_archive_size = p_params[5];

}

void *EmopsoAlgorithmSettings::getImpl(const std::string &p_setting)
{
    std::map<std::string, unsigned int> setting_map = {{"SOCIAL", SOCIAL_ACC_SETTING},
                                                       {"COGNITIVE", COGNITIVE_ACC_SETTING},
                                                       {"INERTIA", INERTIA_SETTING},
                                                       {"FLY_TIME", FLY_TIME_SETTING},
                                                       {"CLUSTERS", CLUSTERS_SETTING},
                                                       {"ARCHIVE_SIZE", ARCHIVE_SIZE_SETTING}};

    unsigned int setting = 100;

    std::map<std::string, unsigned int>::iterator it = setting_map.find(p_setting);
    if(setting_map.end() != it) {
        setting = it->second;
    }

    switch(setting) {
    case SOCIAL_ACC_SETTING:
        return &m_social_acc;
    case COGNITIVE_ACC_SETTING:
        return &m_cognitive_acc;
    case INERTIA_SETTING:
        return &m_inertia;
    case FLY_TIME_SETTING:
        return &m_fly_time;
    case CLUSTERS_SETTING:
        return &m_number_of_clusters;
    case ARCHIVE_SIZE_SETTING:
        return &m_max_archive_size;
    default:
        return NULL;
    }
}

