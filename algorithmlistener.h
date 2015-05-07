#ifndef ALGORITHMLISTENER_H
#define ALGORITHMLISTENER_H

#include <QObject>

class AlgorithmListener : public QObject
{
    Q_OBJECT
public:
    explicit AlgorithmListener(QObject *parent = 0);
    ~AlgorithmListener();

    // @see libEvol/libEvolOptimizer/ICNotifier.h
    virtual void beforeStart(const char* _name) = 0;
    virtual void afterStop(const char* _name) = 0;
    virtual void error(const char* _error) = 0;
    virtual void generationDone(unsigned int _g) = 0;

signals:

public slots:
};

#endif // ALGORITHMLISTENER_H
