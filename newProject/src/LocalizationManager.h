#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include "Particle.h"
#include <vector>
#include <HamsterAPIClientCPP/Hamster.h>

using namespace std;
using namespace HamsterAPI;

class LocalizationManager {
private:
	vector<Particle *> particles;
	const OccupancyGrid &ogrid;//map
	Hamster *hamster;
	void getRandomLocation(Particle *particle);
	double computeBelief(Particle *particle);

public:
	int rouletteWheel();
	void getRandomNearGoodParticles();
	LocalizationManager(const OccupancyGrid &ogrid, Hamster *hamster);
	void initParticles();
	void updateParticles(double dis,double deltaTeta);
	void printParticles() const;
	vector<Particle *> getParticles() const;
	struct myclass{
		bool operator()(Particle * par1 , Particle * par2){
			return (par1->belief < par2->belief);
		}
	}lessThanBelief;
	virtual ~LocalizationManager();
};

#endif /* LOCALIZATIONMANAGER_H_ */
