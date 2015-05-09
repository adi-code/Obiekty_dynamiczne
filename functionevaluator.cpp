#include "functionevaluator.h"

#include <sstream>

FunctionEvaluator::FunctionEvaluator()
{

}

FunctionEvaluator::~FunctionEvaluator()
{

}

std::vector<double> FunctionEvaluator::evaluate(double *p_pf, unsigned int p_nf) {
    std::string hash = getHash(p_pf, p_nf);

    if(isCached(hash)) {
        return getFromCache(hash);
    }

    std::vector<double> result = evaluateImpl(p_pf, p_nf);

    storeInCache(hash, result);

    return result;
}

void FunctionEvaluator::clearCache() {
    m_cache.clear();
}

std::vector<double> FunctionEvaluator::getFromCache(const std::string &p_hash) {
    std::unordered_map<std::string, std::vector<double> >::iterator it = m_cache.find(p_hash);
    if(m_cache.end() != it) {
        return it->second;
    }

    std::vector<double> result = {0.0};
    return result;
}

std::string FunctionEvaluator::getHash(double *p_pf, unsigned int p_nf) {
    std::ostringstream ss;
    for(unsigned int i=0;i<p_nf;++i) {
        ss << p_pf[i] << " ";
    }
    return ss.str();
}

bool FunctionEvaluator::isCached(const std::string &p_hash) {
    std::unordered_map<std::string, std::vector<double> >::iterator it = m_cache.find(p_hash);
    return m_cache.end() != it;
}

void FunctionEvaluator::storeInCache(const std::string &p_hash, const std::vector<double> &p_value) {
    m_cache.emplace(p_hash, p_value);
}