#include "emopsoalgorithmrunner.h"
#include "libEvol/emopso/emopso.h"
#include "libEvol/libEvolOptimizer/ICNotifier.h"


class EmopsoICNotifier : public ICNotifier {
public:
    EmopsoICNotifier() : ICNotifier(), m_listener(NULL) {}

    void setListener(AlgorithmListener* p_listener) {
        m_listener = p_listener;
    }

    virtual void onBeforeStart(const char* p_name) {
        if(NULL != m_listener) {
            m_listener->beforeStart(p_name);
        }
    }

    virtual void onAfterStop(const char* p_name) {
        if(NULL != m_listener) {
            m_listener->afterStop(p_name);
        }
    }

    virtual void onError(const char* p_error) {
        if(NULL != m_listener) {
            m_listener->error(p_error);
        }
    }

    virtual void onGenerationDone(unsigned int p_g) {
        if(NULL != m_listener) {
            m_listener->generationDone(p_g);
        }
    }

private:
    AlgorithmListener* m_listener;
};

class EmopsoICQualityFunction : public ICQualityFunction {
public:
    EmopsoICQualityFunction() : ICQualityFunction(), m_func(NULL) {}

    void setFunction(Function* func) {
        m_func = func;
    }

    double calculate(double* p_pf, unsigned int p_nf) {
        if(NULL != m_func) {
            return m_func->calculate(p_pf, p_nf);
        }

        return 0.0;
    }

private:
    Function* m_func;
};

EmopsoAlgorithmRunner::EmopsoAlgorithmRunner() : m_algorithm(new EMOPSO()),
    m_listener(NULL), m_settings(NULL)
{

}

EmopsoAlgorithmRunner::~EmopsoAlgorithmRunner()
{
    if(m_algorithm != NULL)
    {
        delete static_cast<EMOPSO*>(m_algorithm);
    }
}

void EmopsoAlgorithmRunner::configure(AlgorithmSettings *p_settings)
{
    m_settings = p_settings;
    EMOPSO* algorithm = static_cast<EMOPSO*>(m_algorithm);

    std::vector<Function*> obj_funcs = p_settings->getObjectiveFunctions();
    std::vector<ICQualityFunction*> lib_obj_funcs;
    for(unsigned int i=0;i<obj_funcs.size();++i) {
        Function* func = obj_funcs.at(i);
        EmopsoICQualityFunction* icQualityFunc = new EmopsoICQualityFunction();
        icQualityFunc->setFunction(func);
        lib_obj_funcs.emplace_back(icQualityFunc);
        algorithm->addObjectiveFnc(lib_obj_funcs.at(i));
    }

    double social_acc = p_settings->get<double>("SOCIAL");
    double cognitive_acc = p_settings->get<double>("COGNITIVE");
    double inertia = p_settings->get<double>("INERTIA");
    int fly_time = p_settings->get<int>("FLY_TIME");
    int number_of_clusters = p_settings->get<int>("CLUSTERS");
    int archive_size = p_settings->get<int>("ARCHIVE_SIZE");

    algorithm->setSocialAcc(social_acc);
    algorithm->setCognitiveAcc(cognitive_acc);
    algorithm->setInertia(inertia);
    algorithm->setFlyTime(fly_time);
    algorithm->setNumberOfClusters(number_of_clusters);
    algorithm->setMaxArchiveSize(archive_size);

    algorithm->setGenerations(p_settings->getGenerations());
    //algorithm->setGenerations(300);

    algorithm->setFloatSize(p_settings->getFloatSize());
    //algorithm->setFloatSize(1);

    algorithm->setBinarySize(p_settings->getBinarySize());
    //algorithm->setBinarySize(0);

    algorithm->setPopSize(p_settings->getPopSize());
    //algorithm->setPopSize(200);

    algorithm->setSeed(0.5);

    for (unsigned int i = 0; i < algorithm->getFloatSize() ; i++) {
        double fl = p_settings->getFLLimit(i);
        algorithm->setFLLimit(fl, i);
        //algorithm->setFLLimit(-50,i);
        double hl = p_settings->getFHLimit(i);
        algorithm->setFHLimit(hl, i);
        //algorithm->setFHLimit(50,i);
    }

    for (unsigned int i = 0; i < algorithm->getBinarySize(); i++) {
        algorithm->setBLLimit(p_settings->getBLLimit(i), i);
        //algorithm->setBLLimit(1,i);
        algorithm->setBLLimit(p_settings->getBLLimit(i), i);
        //algorithm->setBHLimit(5,i);
    }
}

void EmopsoAlgorithmRunner::setListener(AlgorithmListener *p_listener)
{
    m_listener = p_listener;
}

void EmopsoAlgorithmRunner::run()
{
    EMOPSO* algorithm = static_cast<EMOPSO*>(m_algorithm);

    //Amga2ICNotifier notifier;
    //notifier.setListener(m_listener);

    //algorithm->setNotifier(&notifier);

    EmopsoICNotifier* n = new EmopsoICNotifier;
    n->setListener(m_listener);
    algorithm->setNotifier(n);

//    for (int i = 0; i < algorithm->getFloatSize() ; i++) {
//        algorithm->setFLLimit(-50,i);
//        algorithm->setFHLimit(50,i);
//    }

//    for (int i = 0; i < algorithm->getBinarySize(); i++) {
//        algorithm->setBLLimit(1,i);
//        algorithm->setBHLimit(5,i);
//    }

    //Adding obj functions:
    //algorithm->addObjectiveFnc(&f1);
    //algorithm->addObjectiveFnc(&f2);
    //algorithm->addObjectiveFnc(&r);
    //algorithm->addConstraintFnc(&g1);

    //OPTIMIZATION RUN

//    bool configured = algorithm->checkConf();
//    if(configured)
//    {
//        algorithm->run();
//    }

    algorithm->run();
}

std::vector<std::vector<double> > EmopsoAlgorithmRunner::getResults()
{
    std::vector<std::vector<double> > result;
    EMOPSO* algorithm = static_cast<EMOPSO*>(m_algorithm);
    CMultiObjectiveSolutionSet* solution_set = algorithm->getSolutionSet();

    for(CMultiObjectiveSolutionSet::Iterator solution = solution_set->begin();
        solution != solution_set->end();
        solution++) {

        std::vector<double> sol;

        for(unsigned int i=0;i<(*solution)->FFloatVars.size();i++) {
            sol.emplace_back((*solution)->FFloatVars[i]);
        }

        for(unsigned int i=0;i<(*solution)->FObjFncVals.size();i++) {
            sol.emplace_back((*solution)->FObjFncVals[i]);
        }

        result.emplace_back(sol);
    }

    return result;
}

