#ifndef AMGA2ALGORITHMSETTINGS_H
#define AMGA2ALGORITHMSETTINGS_H

#include <QObject>

#include "algorithmsettings.h"

class Amga2AlgorithmSettings : /*public QObject, */public AlgorithmSettings
{
    //Q_OBJECT
public:
    //explicit Amga2AlgorithmSettings(QObject *parent = 0);
    Amga2AlgorithmSettings();
    ~Amga2AlgorithmSettings();

    std::vector<AlgorithmSetting> getSettingsList();
    void setParameters(const std::vector<double>& p_params);

protected:
    void* getImpl(const std::string& p_setting);

protected:
    static const unsigned int CROSS_SETTING;
    static const unsigned int MUT_SETTING;
    static const unsigned int DI_SETTING;
    static const unsigned int DIM_SETTING;
    static const unsigned int ARCHIVE_SIZE_SETTING;

    double m_cross;
    double m_mut;
    double m_di;
    double m_dim;
    int m_archive_size;

//signals:

//public slots:
};

#endif // AMGA2ALGORITHMSETTINGS_H
