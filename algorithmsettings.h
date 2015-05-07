#ifndef ALGORITHMSETTINGS_H
#define ALGORITHMSETTINGS_H

//#include <QObject>

class AlgorithmSettings// : public QObject
{
    //Q_OBJECT
public:
    //explicit AlgorithmSettings(QObject *parent = 0);
    AlgorithmSettings();
    ~AlgorithmSettings();

    template <typename T>
    T get(unsigned int i);

protected:
    virtual void* getImpl(unsigned int i) = 0;

//signals:

//public slots:
};

#endif // ALGORITHMSETTINGS_H
