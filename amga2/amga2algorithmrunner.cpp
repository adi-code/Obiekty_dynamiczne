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

//Amga2AlgorithmRunner::Amga2AlgorithmRunner(QObject *parent) : QObject(parent), m_algorithm(new Amga2()),
//    m_listener(NULL), m_settings(NULL)
Amga2AlgorithmRunner::Amga2AlgorithmRunner() : m_algorithm(new Amga2()),
    m_listener(NULL), m_settings(NULL)
{

}

Amga2AlgorithmRunner::~Amga2AlgorithmRunner()
{
    if(m_algorithm != NULL) {
        delete m_algorithm;
    }
}

void Amga2AlgorithmRunner::configure(AlgorithmSettings *p_settings) {
    m_settings = p_settings;
    Amga2* algorithm = static_cast<Amga2*>(m_algorithm);
    algorithm->setGenerations(300);
    algorithm->setFloatSize(1);
    algorithm->setBinarySize(0);
    algorithm->setPopSize(200);
    algorithm->setSeed(0.5);
}

void Amga2AlgorithmRunner::setListener(AlgorithmListener *p_listener) {
    m_listener = p_listener;
}

void Amga2AlgorithmRunner::run() {
    Amga2* algorithm = static_cast<Amga2*>(m_algorithm);

    Amga2ICNotifier notifier;
    notifier.setListener(m_listener);

    algorithm->setNotifier(&notifier);


    for (int i = 0; i < algorithm->getFloatSize() ; i++) {
        algorithm->setFLLimit(-50,i);
        algorithm->setFHLimit(50,i);
    }

    for (int i = 0; i < algorithm->getBinarySize(); i++) {
        algorithm->setBLLimit(1,i);
        algorithm->setBHLimit(5,i);
    }

    //Adding obj functions:
    //algorithm->addObjectiveFnc(&f1);
    //algorithm->addObjectiveFnc(&f2);
    //algorithm->addObjectiveFnc(&r);
    //algorithm->addConstraintFnc(&g1);

    //OPTIMIZATION RUN
    algorithm->run();
}