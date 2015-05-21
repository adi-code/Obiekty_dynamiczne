//mopso con epsilon dominance y clusters

#include <assert.h>
#include <limits>
#include "emopso.h"
#include "efile.h"
#include <messages.h>

EMOPSO::~EMOPSO(){

}

EMOPSO::EMOPSO() {

  setPopSize(40);
  setBinarySize(0);
  setFloatSize(4);
  setGenerations(625);
  setNumberOfClusters(3);
  setSeed(1);

  for (int i = 0; i != getFloatSize() ; i++) {
	  setFLLimit(-5,i);
	  setFHLimit(5,i);
  }

  setMaxArchiveSize(100);
  setSocialAcc(2.0);
  setCognitiveAcc(2.0);
  setInertia(0.4);
  setFlyTime(5);
  nclusters= 3;
  gen=0;

  archive = new EFILE;

}
void EMOPSO::setMaxArchiveSize(int _as){
    if (_as <= 0) {
       fireOnErrorNotify(E_OPT_ARCH_SIZE);
    } else {
        maxarchiveSize = _as;
    }
}

void EMOPSO::setSocialAcc(double _sacc){
    if (_sacc < 0) {
       fireOnErrorNotify(E_OPT_PARAM_SACCEL);
    } else {
        C1 = _sacc;
    }

}

void EMOPSO::setCognitiveAcc(double _cacc){
    if (_cacc < 0) {
       fireOnErrorNotify(E_OPT_PARAM_CACCEL);
    } else {
        C2 = _cacc;
    }
}

void EMOPSO::setInertia(double _i){
    if (_i < 0) {
       fireOnErrorNotify(E_OPT_PARAM_DECEL);
    } else {
        W = _i;
    }
}

void EMOPSO::setFlyTime(int _ft){
    if (_ft <= 0) {
       fireOnErrorNotify(E_OPT_FT);
    } else {
       flyTime = _ft;
    }

}

void EMOPSO::initialize(){
	//archive.clear();
   for(int _i(0);_i<nparticles;_i++){
    for(int _j(0);_j<ndimensions;_j++){
      particles[_i].vel[_j]=0;
      particles[_i].x[_j]=rnd(lb[_j],ub[_j]);
      if (_j>=ndimensions-getBinarySize()){
          particles[_i].x[_j] = (int)particles[_i].x[_j];
      }
      particles[_i].xpbest[_j]=particles[_i].x[_j];
    }

    function(_i);

    for(int _j(0);_j<nobjectives;_j++){
      particles[_i].fxpbest[_j]=particles[_i].fx[_j];
    }

	assert (particles[_i].fx.size() == nobjectives);
    archive->add(particles[_i],-1);
  }
}

void EFILE::clear() {
	//solutions.clear();
  //allsolutions.clear();
}

void EMOPSO::alocateMemory() {
  nparticles = getPopSize();
  ndimensions = getFloatSize()+getBinarySize();
  nobjectives = getObjectiveFncCnt();
  nconstr = getConstraintFncCnt();
  gmax = getGenerations();

  lb.resize(ndimensions);
  ub.resize(ndimensions);

  for(int _j(0);_j<ndimensions-getBinarySize();_j++){
	lb[_j] = getFLLimit(_j);
	ub[_j] = getFHLimit(_j);
  }
  for(int _j(ndimensions-getBinarySize());_j<ndimensions;_j++){
    lb[_j] = getBLLimit(_j);
    ub[_j] = getBHLimit(_j);
  }

  nclusters = _numberOfClusters;
  //particles.clear();
 // particles.resize(nparticles);
  for (int i = 0; i < nparticles; i++){
	particles.push_back(Particle(ndimensions,nobjectives, nconstr));
  }

  archive->init(ndimensions, nobjectives, nclusters,maxarchiveSize, 2*maxarchiveSize, nconstr); //hard-coded parameters!
}

void EMOPSO::run() {
  //proxy

  alocateMemory();
  execute();

}


void EMOPSO::execute(){
  fireOnBeforeStartNotify("starting");
  initialize();
  for(gen=0;gen<gmax;gen++){
    archive->updatematrix=true;
    archive->hierarchicalClustering();

//	printf("\nSOLUTIONS\n");
//	for (int i=0; i < nclusters; i++) {
//		 printf("%i\t",archive.nsolutionsbyswarm[i]);
//	}
//	printf("\n\n");
//
//	printf("\nProximity Matrix index\n");
//	for (int i=0; i < nclusters; i++) {
//		 for (int j=0; j < archive.nsolutionsbyswarm[i]; j++) {
//			printf("%i\t",archive.proximitymatrixindex[i][j]);
//		 }
//		 printf("\n\n");
//	}



	flight();
    fireOnGenerationDoneNotify(gen);
    //archive.printSolutions();
  }
  setSolution(this->output());
  fireOnAfterStopNotify("done");
}

void EMOPSO::flight(){
  for(int _i(0);_i<nparticles;_i++){
  	//numer klastra od zera do nclusters - 1
    int _whichcluster=(int)_i/(nparticles/nclusters);
    int _gbestselected;
	//wybranie losowego lidera... ale z archiwum najlepszych do tej pory!

    _gbestselected=archive->selectClusteredRandomSolution(_whichcluster);
    Particle _gbestarchparticle(ndimensions,nobjectives, nconstr);
    _gbestarchparticle = archive->solutions[_gbestselected];

    for(int _k(0);_k<flyTime;_k++){ //there was 5 in here instead of 1. No idea why...
    	//ilość generacji jednego PSO - była ustawiona na 5
		for(int _j(0);_j<ndimensions;_j++){
            particles[_i].vel[_j]=W*particles[_i].vel[_j]+C1*rnd(0,1)*(_gbestarchparticle.x[_j]-particles[_i].x[_j])+C2*rnd(0,1)*(particles[_i].xpbest[_j]-particles[_i].x[_j]);

            //uwaga - w razie czego nie stosować perturbacji!!!
			//perturbation(_i);
    		particles[_i].x[_j]+=particles[_i].vel[_j];

            if (_j>=ndimensions-getBinarySize()){
                particles[_i].x[_j] = (int)particles[_i].x[_j];
            }

            if(particles[_i].x[_j]<lb[_j])
    			particles[_i].x[_j]=lb[_j];
    			//particles[_i].vel[_j]=0;
    		//może dodać zerowanie prędkości na granicy?
    		if(particles[_i].x[_j]>ub[_j])
    			particles[_i].x[_j]=ub[_j];
    			//particles[_i].vel[_j]=0;
		}
    	//std::cout << particles.size() << std::endl;
		function(_i);
    	//sprawdź czy lepsze od dotychczasowego pbest
        int _tmp=archive->dominePBest(particles[_i]);
		if(_tmp==11||_tmp==1){
    		//zapamiętaj to rozwiązanie jako pbest
    		copy(particles[_i].fxpbest,particles[_i].fx);
    		copy(particles[_i].xpbest,particles[_i].x);
    		//dodaj to rozwiązanie do archiwum/usuń zdominowane i jeszcze wiele wiele innych rzeczy...
            archive->add(particles[_i],(int)_i/(nparticles/nclusters));
		}
	}
  }
}

void EMOPSO::perturbation(int _whichparticle){
  int _dimension=0;
  double _lb,_ub,_rango;

  //prawdopodobieństwo mutacji maleje liniowo od jedynki do zera.
  double _gt=(double)gen/gmax;

  double  _pM=pow(_gt,1.7)-2.0*_gt+1.0;
  int _flag(0);
  if((rnd(0.0,1.0)>_pM)&&_flag<=ndimensions){

      _dimension= (int) rnd(0,ndimensions);
      _rango=(ub[_dimension]-lb[_dimension])*_pM/2.0;//totGen

      if(particles[_whichparticle].x[_dimension]-_rango<lb[_dimension])
  _lb=lb[_dimension];
      else
  _lb=particles[_whichparticle].x[_dimension]-_rango;

      if(particles[_whichparticle].x[_dimension]+_rango>ub[_dimension])
  _ub=ub[_dimension];
      else
  _ub=particles[_whichparticle].x[_dimension]+_rango;

  particles[_whichparticle].x[_dimension]=rnd(_lb,_ub);
  if (_dimension>=ndimensions-getBinarySize()){
      particles[_whichparticle].x[_dimension] = (int)particles[_whichparticle].x[_dimension];
  }
  _flag++;

    }

}

void EMOPSO::function(int particleIndex){
	computeObjectives(particleIndex);
  computeConstraints(particleIndex);
}

void EMOPSO::computeConstraints(int i) {
  double constraint;

   for (int j = 0 ; j < particles[i].constraints.size(); j++) {
    constraint = calculateConstraintFnc(&(particles[i].x[0]), ndimensions, j);
    if (constraint > 0) {
    } else {
      constraint = 0;
    }
    particles[i].constraints[j] = constraint;

  }
}

void EMOPSO::computeObjectives(int i) {
  for (int j = 0; j != particles[i].fx.size(); j++) {
    particles[i].fx[j] = calculateObjectiveFnc(&(particles[i].x[0]), ndimensions, j);

  }
}

double EMOPSO::rnd(double _min,double _max){
  return((double)(_min + ((double)(_max-_min)*rand()/(double)(RAND_MAX+_min))));
}


void EMOPSO::copyx(std::vector<double> &_a,std::vector<double> &_b){
  for(int _i(0);_i<ndimensions;_i++){
    _a[_i]=_b[_i];
  }
}

void EMOPSO::copyfx(std::vector<double> &_a,std::vector<double> &_b){
  for(int _i(0);_i<nobjectives;_i++){
    _a[_i]=_b[_i];
  }
}


void EMOPSO::copy(std::vector<double> &_a,std::vector<double> &_b){
  for(int _i(0);_i<_a.size();_i++){
    _a[_i]=_b[_i];
  }

}

void EMOPSO::setNumberOfClusters(int n) {
	_numberOfClusters = n;
}
int EMOPSO::getNuberOfClusters() {
	return _numberOfClusters;
}

/********************************************************************************************/




CMultiObjectiveSolutionSet * EMOPSO::output(){
  archive->finalSolutions();
  archive->printSolutions();

  CMultiObjectiveSolutionSet* set =  new  CMultiObjectiveSolutionSet();
  for (int i = 0; i != archive->nsolutions; i++) {
    set->solutions.push_back(archive->solutions[i].convertToCMultiObjectiveSolution());
  }
  return set;
}



//
//Particle::Particle(const Particle & other) {
//
//}
