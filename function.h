#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>

#include "functionevaluator.h"

class Function : public QObject
{
    Q_OBJECT
public:
    explicit Function(QObject *parent = 0);
    ~Function();

    virtual double calculate(double* p_pf, unsigned int p_nf) = 0;
    virtual void setEvaluator(FunctionEvaluator* p_evaluator);

protected:
    FunctionEvaluator* m_evaluator;

signals:

public slots:
};

#endif // FUNCTION_H
