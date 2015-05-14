#include "function.h"

Function::Function(QObject *parent) : QObject(parent)
{

}

Function::~Function()
{

}

void Function::setEvaluator(FunctionEvaluator *p_evaluator) {
    m_evaluator = p_evaluator;
}
