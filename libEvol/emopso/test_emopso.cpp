
#include "emopso.h"
#include <src/advancednotifier/CAdvancedNotifier.h>
#include <src/functions/ZDTTestProblems.h>
#include <src/functions/WATERProblem.h>
#include <src/functions/AncientTestProblem.h>

int main()
{
  EMOPSO  algorithm;
  //AncientTestProblem problem;
  WATERProblem problem;
  problem.setItselfToAlgorithm(&algorithm);
  CAdvancedNotifier* notifier = new CAdvancedNotifier(&algorithm,&problem);
  algorithm.setNotifier(notifier);
  algorithm.run();
}

//---------------------------------------------------------------------------
