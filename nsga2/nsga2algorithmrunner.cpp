#include "nsga2algorithmrunner.h"

#include "libEvol/nsga2/CNSGA2Optimizer.h"
#include "libEvol/libEvolOptimizer/ICNotifier.h"

class Nsga2ICNotifier : public ICNotifier {
public:
    Nsga2ICNotifier() : ICNotifier(), m_listener(NULL) {}

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

class Nsga2ICQualityFunction : public ICQualityFunction {
public:
    Nsga2ICQualityFunction() : ICQualityFunction(), m_func(NULL) {}

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

Nsga2AlgorithmRunner::Nsga2AlgorithmRunner() : m_algorithm(new CNSGA2Optimizer()),
    m_listener(NULL), m_settings(NULL)
{

}

Nsga2AlgorithmRunner::~Nsga2AlgorithmRunner()
{
    if(m_algorithm != NULL) {
        delete static_cast<CNSGA2Optimizer*>(m_algorithm);
    }
}

void Nsga2AlgorithmRunner::configure(AlgorithmSettings *p_settings)
{
    m_settings = p_settings;
    CNSGA2Optimizer* algorithm = static_cast<CNSGA2Optimizer*>(m_algorithm);

    std::vector<Function*> obj_funcs = p_settings->getObjectiveFunctions();
    std::vector<ICQualityFunction*> lib_obj_funcs;
    for(unsigned int i=0;i<obj_funcs.size();++i) {
        Function* func = obj_funcs.at(i);
        Nsga2ICQualityFunction* icQualityFunc = new Nsga2ICQualityFunction();
        icQualityFunc->setFunction(func);
        lib_obj_funcs.emplace_back(icQualityFunc);
        algorithm->addObjectiveFnc(lib_obj_funcs.at(i));
    }

    double cross = p_settings->get<double>("CROSS");
    double mut = p_settings->get<double>("MUT");
    double di = p_settings->get<double>("DI");
    double dim = p_settings->get<double>("DIM");

    algorithm->setCross(cross);
    algorithm->setFloatPMut(mut);
    algorithm->setDi(di);
    algorithm->setDiM(dim);
    algorithm->setBinaryPMut(0.0);

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

void Nsga2AlgorithmRunner::setListener(AlgorithmListener *p_listener)
{
    m_listener = p_listener;
}

void Nsga2AlgorithmRunner::run()
{
    CNSGA2Optimizer* algorithm = static_cast<CNSGA2Optimizer*>(m_algorithm);

    //Amga2ICNotifier notifier;
    //notifier.setListener(m_listener);

    //algorithm->setNotifier(&notifier);

    Nsga2ICNotifier* n = new Nsga2ICNotifier;
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

std::vector<std::vector<double> > Nsga2AlgorithmRunner::getResults()
{
    std::vector<std::vector<double> > result;

    CNSGA2Optimizer* algorithm = static_cast<CNSGA2Optimizer*>(m_algorithm);
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

