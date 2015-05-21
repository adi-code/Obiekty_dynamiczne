//---------------------------------------------------------------------------


#include "messages.h"

//---------------------------------------------------------------------------

const char* M_OPT_STARTED 				=	"Optimization started";
const char* M_OPT_FINISHED				=   "Optimization finished";
const char* M_OPT_NO_FSOL 				=   "Feasible solutions does not exist";

const char* E_OPT_BV_NS	  				=	"Binary-coded variables number is not set!\nConfigure it first!";
const char* E_OPT_FV_NS					=	"Real-coded variables number is not set!\nConfigure it first!";

const char* E_OPT_FBV_NS				=   "Decision variables number is not set!\nConfigure it first!";

const char* E_OPT_NP					=  	"Negative value of the parameter!\nEnter a positive one!";

const char* E_OPT_BV_NO					=	"Binary variables number has to be >=0! Zero was set";
const char* E_OPT_FV_NO					=	"Real-coded variables number has to >=0! Zero was set";

const char* E_OPT_NMO					=   "Defined problem is not multi objective one!\nPlease set at least 2 objective functions!";
const char* E_OPT_OFNS					=	"Objective function was not set!";
const char* E_OPT_TERM					=   "Optimization terminated";

const char* E_OPT_PARAM_BS 				=	"Incorrect parameter value:\nNumber of binary coded decision variables(must be >0)\nDefault value will be used.";
const char* E_OPT_PARAM_FS 				= 	"Incorrect parameter value:\nNumber of real coded decision variables(must be >0)\nDefault value will be used.";
const char* E_OPT_PARAM_POP_SIZE 		= 	"Incorrect parameter value:\nNumber of the individuals in the population(must be >0)\nDefault value will be used.";
const char* E_OPT_PARAM_GENERATIONS 	= 	"Incorrect parameter value:\nNumber of generations(must be >0)\nDefault value will be used.";
const char* E_OPT_PARAM_CROSSOVER 		=	"Incorrect parameter value:\nCross-over propability (between 0.5 and 1)\nDefault value will be used.";
const char* E_OPT_PARAM_MUTF 			= 	"Incorrect parameter value:\nThe mutation probability for real-coded vectors\n(between 0 and 1.0/ Number of real coded decision variables)\nDefault value will be used.";
const char* E_OPT_PARAM_MUTB 			= 	"Incorrect parameter value:\nThe mutation probability for real-coded vectors\n(between 0 and 1.0/Number of binary coded decision variables)\nDefault value will be used.";
const char* E_OPT_PARAM_DI 				= 	"Incorrect parameter value:\nDistribution Index for crossover between 0.5 to 100 (Default = 20)\nDefault value will be used.";
const char* E_OPT_PARAM_DIM 			= 	"Incorrect parameter value:\nDistribution Index for mutation between (Default = 10 to 500)\nDefault value will be used.";
const char* E_OPT_PARAM_SEED 			= 	"Incorrect parameter value:\nRandom seed (has to be between 0 and 1)\nDefault value will be used.";

const char* E_OPT_ARCH_SIZE 			=	"Incorrect parameter value:\nArchive size(must be >0)\nDefault value will be used.";

const char* E_OPT_PARAM_LBSIZE			=	"Incorrect parameter value:\nLocal best count (has to be between 0 and population size)\nDefault value will be used.";

const char* E_OPT_PARAM_CACCEL			=   "Incorrect parameter value:\nCognitive aceleration (has to be >=0)\nDefault value will be used.";
const char* E_OPT_PARAM_SACCEL   		=   "Incorrect parameter value:\nSocial aceleration (has to be >=0)\nDefault value will be used.";
const char* E_OPT_PARAM_GACCEL   		=   "Incorrect parameter value:\nGlobal aceleration (has to be >=0)\nDefault value will be used.";

const char* E_OPT_PARAM_DECEL			=	"Incorrect parameter value:\nDeceleration coefficient (has to be between 0 and 1)\nDefault value will be used.";
const char* E_OPT_FT					= 	"Incorrect parameter value:\nNumber of fly iterations (has to be >0)\nDefault value will be used.";
const char* E_OPT_PARAM_MAXVEL			=   "Incorrect parameter value:\nMaximum velocity coefficient (has to be >=0)\nDefault value will be used.";
