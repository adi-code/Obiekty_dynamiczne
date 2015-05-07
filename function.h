#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>

class Function : public QObject
{
    Q_OBJECT
public:
    explicit Function(QObject *parent = 0);
    ~Function();

    virtual double calculate(double* p_pf, unsigned int p_nf) = 0;

signals:

public slots:
};

#endif // FUNCTION_H
