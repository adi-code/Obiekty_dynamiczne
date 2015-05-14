#include "algorithmrunner.h"

#include "libEvol/amga2/Amga2.h"

//AlgorithmRunner::AlgorithmRunner(QObject *parent) : QObject(parent)
AlgorithmRunner::AlgorithmRunner()
{

}

AlgorithmRunner::~AlgorithmRunner()
{

}

//void AlgorithmRunner::run() {
//    Amga2 alg;
//    //optimizer = new Amga2();
//    alg.setNotifier();
//    alg.setGenerations(300);
//    alg.setFloatSize(1);
//    alg.setBinarySize(0);
//    alg.setPopSize(200);
//    alg.setSeed(0.5);

//    for (int i = 0; i < alg.getFloatSize() ; i++) {
//      alg.setFLLimit(-50,i);
//      alg.setFHLimit(50,i);
//    }

//    for (int i = 0; i < alg.getBinarySize(); i++) {
//      alg.setBLLimit(1,i);
//      alg.setBHLimit(5,i);
//    }

//    //Adding obj functions:
//    alg.addObjectiveFnc(&f1);
//    alg.addObjectiveFnc(&f2);
//    //alg.addObjectiveFnc(&r);
//    alg.addConstraintFnc(&g1);

//    //OPTIMIZATION RUN
//    alg.run();


    //RESULTS:
    /*CMultiObjectiveSolutionSet * solutionSet = alg.getSolutionSet();

    std::ofstream rf("result.txt");
    for (CMultiObjectiveSolutionSet::Iterator solution = solutionSet->begin();
         solution != solutionSet->end(); solution++) {

         for (int i=0; i < (*solution)->FBinaryVars.size(); i++) {
              std::cout <<  (*solution)->FBinaryVars[i] << '\t';
              rf << (*solution)->FBinaryVars[i] << '\t';
         }
         for (int i=0; i < (*solution)->FFloatVars.size(); i++) {
              std::cout <<  (*solution)->FFloatVars[i] << '\t';
              rf << (*solution)->FFloatVars[i] << '\t';
         }
         for (int i=0; i < (*solution)->FObjFncVals.size(); i++) {
              std::cout <<  (*solution)->FObjFncVals[i] << '\t';
              rf << (*solution)->FObjFncVals[i] << '\t';
         }
         std::cout << '\n';
         rf <<'\n';

    }*/
//}
