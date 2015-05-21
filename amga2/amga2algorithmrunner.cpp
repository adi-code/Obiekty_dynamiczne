#include "amga2algorithmrunner.h"

#include "libEvol/amga2/Amga2.h"
#include "libEvol/libEvolOptimizer/ICNotifier.h"

class Amga2ICNotifier : public ICNotifier {
public:
    Amga2ICNotifier() : ICNotifier(), m_listener(NULL) {}

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

class Amga2ICQualityFunction : public ICQualityFunction {
public:
    Amga2ICQualityFunction() : ICQualityFunction(), m_func(NULL) {}

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

//Amga2AlgorithmRunner::Amga2AlgorithmRunner(QObject *parent) : QObject(parent), m_algorithm(new Amga2()),
//    m_listener(NULL), m_settings(NULL)
Amga2AlgorithmRunner::Amga2AlgorithmRunner() : m_algorithm(new Amga2()),
    m_listener(NULL), m_settings(NULL)
{

}

Amga2AlgorithmRunner::~Amga2AlgorithmRunner()
{
    if(m_algorithm != NULL) {
        delete static_cast<Amga2*>(m_algorithm);
    }
}

void Amga2AlgorithmRunner::configure(AlgorithmSettings *p_settings) {
    m_settings = p_settings;
    Amga2* algorithm = static_cast<Amga2*>(m_algorithm);

    std::vector<Function*> obj_funcs = p_settings->getObjectiveFunctions();
    std::vector<ICQualityFunction*> lib_obj_funcs;
    for(unsigned int i=0;i<obj_funcs.size();++i) {
        Function* func = obj_funcs.at(i);
        Amga2ICQualityFunction* icQualityFunc = new Amga2ICQualityFunction();
        icQualityFunc->setFunction(func);
        lib_obj_funcs.emplace_back(icQualityFunc);
        algorithm->addObjectiveFnc(lib_obj_funcs.at(i));
    }

    double cross = p_settings->get<double>("CROSS");
    double mut = p_settings->get<double>("MUT");
    double di = p_settings->get<double>("DI");
    double dim = p_settings->get<double>("DIM");
    int archive_size = p_settings->get<int>("ARCHIVE_SIZE");

    algorithm->setCross(cross);
    algorithm->setPMut(mut);
    algorithm->setDi(di);
    algorithm->setDiM(dim);
    algorithm->setArchiveSize(archive_size);

    algorithm->setGenerations(p_settings->getGenerations());
//    algorithm->setGenerations(10);

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

void Amga2AlgorithmRunner::setListener(AlgorithmListener *p_listener) {
    m_listener = p_listener;
}

void Amga2AlgorithmRunner::run() {
    Amga2* algorithm = static_cast<Amga2*>(m_algorithm);

    //Amga2ICNotifier notifier;
    //notifier.setListener(m_listener);

    //algorithm->setNotifier(&notifier);

    Amga2ICNotifier* n = new Amga2ICNotifier;
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

std::vector<std::vector<double> > Amga2AlgorithmRunner::getResults() {
    std::vector<std::vector<double> > result;

    Amga2* algorithm = static_cast<Amga2*>(m_algorithm);
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
