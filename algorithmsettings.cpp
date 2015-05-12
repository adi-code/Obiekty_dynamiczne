#include "algorithmsettings.h"

//AlgorithmSettings::AlgorithmSettings(QObject *parent) : QObject(parent)
AlgorithmSettings::AlgorithmSettings() : m_float_size(4), m_binary_size(0),
    m_pop_size(0), m_generations(0)
{

}

AlgorithmSettings::~AlgorithmSettings()
{

}

void AlgorithmSettings::addObjectiveFunction(Function *p_func) {
    m_obj_funcs.emplace_back(p_func);
}

std::vector<Function*> AlgorithmSettings::getObjectiveFunctions() {
    return m_obj_funcs;
}

unsigned int AlgorithmSettings::getFloatSize() {
    return m_float_size;
}

void AlgorithmSettings::setFloatSize(unsigned int p_size) {
    m_float_size = p_size;
}

unsigned int AlgorithmSettings::getBinarySize() {
    return m_binary_size;
}

void AlgorithmSettings::setBinarySize(unsigned int p_size) {
    m_binary_size = p_size;
}

unsigned int AlgorithmSettings::getPopSize() {
    return m_pop_size;
}

void AlgorithmSettings::setPopSize(unsigned int p_size) {
    m_pop_size = p_size;
}

unsigned int AlgorithmSettings::getGenerations() {
    return m_generations;
}

void AlgorithmSettings::setGenerations(unsigned int p_generations) {
    m_generations = p_generations;
}

double AlgorithmSettings::getFHLimit(unsigned int p_i) {
    if(m_f_limits.size() > p_i) {
        return m_f_limits[p_i].second;
    }

    return 0.0;
}

void AlgorithmSettings::setFHLimit(double p_limit, unsigned int p_i) {
    if(m_f_limits.size() <= p_i) {
        m_f_limits.resize(p_i+1);
    }

    m_f_limits[p_i].second = p_limit;
}

double AlgorithmSettings::getFLLimit(unsigned int p_i) {
    if(m_f_limits.size() > p_i) {
        return m_f_limits[p_i].first;
    }

    return 0.0;
}

void AlgorithmSettings::setFLLimit(double p_limit, unsigned int p_i) {
    if(m_f_limits.size() <= p_i) {
        m_f_limits.resize(p_i+1);
    }

    m_f_limits[p_i].first = p_limit;
}

double AlgorithmSettings::getBHLimit(unsigned int p_i) {
    if(m_b_limits.size() > p_i) {
        return m_b_limits[p_i].second;
    }

    return 0.0;
}

void AlgorithmSettings::setBHLimit(double p_limit, unsigned int p_i) {
    if(m_b_limits.size() <= p_i) {
        m_b_limits.resize(p_i+1);
    }

    m_b_limits[p_i].second = p_limit;
}

double AlgorithmSettings::getBLLimit(unsigned int p_i) {
    if(m_b_limits.size() > p_i) {
        return m_b_limits[p_i].first;
    }

    return 0.0;
}

void AlgorithmSettings::setBLLimit(double p_limit, unsigned int p_i) {
    if(m_b_limits.size() <= p_i) {
        m_b_limits.resize(p_i+1);
    }

    m_b_limits[p_i].first = p_limit;
}

std::vector<AlgorithmSetting> AlgorithmSettings::getSettingsList() {
    std::vector<AlgorithmSetting> result;
    return result;
}

void AlgorithmSettings::setParameters(const std::vector<double> &/*p_params*/) {
}
