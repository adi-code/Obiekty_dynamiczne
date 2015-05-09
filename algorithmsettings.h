#ifndef ALGORITHMSETTINGS_H
#define ALGORITHMSETTINGS_H

//#include <QObject>

#include "function.h"

struct AlgorithmSetting {
    std::string m_label;
    unsigned int m_type;
    double m_min;
    double m_max;
    double m_default;

    AlgorithmSetting(const std::string& p_label, const unsigned int& p_type,
                     const double& p_min, const double& p_max,
                     const double& p_default) : m_label(p_label), m_type(p_type),
                     m_min(p_min), m_max(p_max), m_default(p_default) {}
};

class AlgorithmSettings// : public QObject
{
    //Q_OBJECT
public:
    //explicit AlgorithmSettings(QObject *parent = 0);
    AlgorithmSettings();
    ~AlgorithmSettings();

    void addObjectiveFunction(Function* p_func);
    std::vector<Function*> getObjectiveFunctions();

    unsigned int getFloatSize();
    void setFloatSize(unsigned int p_size);

    unsigned int getBinarySize();
    void setBinarySize(unsigned int p_size);

    unsigned int getPopSize();
    void setPopSize(unsigned int p_size);

    double getFHLimit(unsigned int p_i);
    void setFHLimit(double p_limit, unsigned int p_i);

    double getFLLimit(unsigned int p_i);
    void setFLLimit(double p_limit, unsigned int p_i);

    double getBHLimit(unsigned int p_i);
    void setBHLimit(double p_limit, unsigned int p_i);

    double getBLLimit(unsigned int p_i);
    void setBLLimit(double p_limit, unsigned int p_i);

    unsigned int getGenerations();
    void setGenerations(unsigned int p_generations);

    virtual std::vector<AlgorithmSetting> getSettingsList();
    virtual void setParameters(const std::vector<double> &p_params);

    template <typename T>
    T get(const std::string& p_name) {
        T* ptr = static_cast<T*>(getImpl(p_name));

        if(NULL == ptr) {
            return T();
        }

        return *ptr;
    }

protected:
    virtual void* getImpl(const std::string& p_name) = 0;

protected:
    std::vector<Function*> m_obj_funcs;

    unsigned int m_float_size;
    unsigned int m_binary_size;
    unsigned int m_pop_size;
    unsigned int m_generations;

    std::vector<std::pair<double, double> > m_f_limits;
    std::vector<std::pair<double, double> > m_b_limits;

//signals:

//public slots:
};

#endif // ALGORITHMSETTINGS_H
