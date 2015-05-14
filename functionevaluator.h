#ifndef FUNCTIONEVALUATOR_H
#define FUNCTIONEVALUATOR_H

#include <string>
#include <unordered_map>
#include <vector>

class FunctionEvaluator
{
public:
    FunctionEvaluator();
    ~FunctionEvaluator();

    std::vector<double> evaluate(double* p_pf, unsigned int p_nf);

    // cache
    void clearCache();

protected:
    virtual std::vector<double> evaluateImpl(double* p_pf, unsigned int p_nf) = 0;

    // cache
    std::vector<double> getFromCache(const std::string& p_hash);
    std::string getHash(double* p_pf, unsigned int p_nf);
    bool isCached(const std::string& p_hash);
    void storeInCache(const std::string& p_hash, const std::vector<double>& p_value);

protected:
    std::unordered_map<std::string, std::vector<double> > m_cache;
};

#endif // FUNCTIONEVALUATOR_H
