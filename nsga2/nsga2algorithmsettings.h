#ifndef NSGA2ALGORITHMSETTINGS_H
#define NSGA2ALGORITHMSETTINGS_H

#include "algorithmsettings.h"

class Nsga2AlgorithmSettings : public AlgorithmSettings
{
public:
    Nsga2AlgorithmSettings();
    ~Nsga2AlgorithmSettings();
    std::vector<AlgorithmSetting> getSettingsList();
    void setParameters(const std::vector<double>& p_params);


protected:
    void* getImpl(const std::string& p_setting);

    static const unsigned int CROSS_SETTING;
    static const unsigned int MUT_SETTING;
    static const unsigned int DI_SETTING;
    static const unsigned int DIM_SETTING;

    double m_cross;
    double m_mut;
    double m_di;
    double m_dim;
};

#endif // NSGA2ALGORITHMSETTINGS_H
