#ifndef EMOPSOALGORITHMSETTINGS_H
#define EMOPSOALGORITHMSETTINGS_H

#include "algorithmsettings.h"

class EmopsoAlgorithmSettings : public AlgorithmSettings
{
public:
    EmopsoAlgorithmSettings();
    ~EmopsoAlgorithmSettings();

    std::vector<AlgorithmSetting> getSettingsList();
    void setParameters(const std::vector<double>& p_params);

protected:
    void* getImpl(const std::string& p_setting);

    static const unsigned int SOCIAL_ACC_SETTING;
    static const unsigned int COGNITIVE_ACC_SETTING;
    static const unsigned int INERTIA_SETTING;
    static const unsigned int FLY_TIME_SETTING;
    static const unsigned int CLUSTERS_SETTING;
    static const unsigned int ARCHIVE_SIZE_SETTING;

    double m_social_acc;
    double m_cognitive_acc;
    double m_inertia;
    int m_fly_time;
    int m_number_of_clusters;
    int m_max_archive_size;
};

#endif // EMOPSOALGORITHMSETTINGS_H

