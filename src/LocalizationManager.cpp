/*
 * LocalizationManager.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: user
 */

#include "LocalizationManager.h"


LocalizationManager::LocalizationManager(const OccupancyGrid& ogrid,Hamster* hamster) :ogrid(ogrid),hamster(hamster){

}

void LocalizationManager::getRandomLocation(Particle* particle){


	do{
		particle->j = rand() % ogrid.getWidth();
		particle->i = rand() % ogrid.getHeight();

	}while(ogrid.getCell(particle->j,particle->i) != CELL_FREE);

	particle->x = particle->j * ogrid.getResolution();
	particle->y = particle->i * ogrid.getResolution();

	particle->yaw = rand()%360;
}

void LocalizationManager::initParticles(){

	particles.resize(PARTICLES_NUM);

	for (int i = 0; i < particles.size(); ++i) {
		particles[i] = new Particle();
		getRandomLocation(particles[i]);
	}

}

void LocalizationManager::updateParticles(double deltaX,double deltaY,double deltaYaw){

	for (int i = 0;i < particles.size(); ++i) {
		Particle* particle = particles[i];
		particle->x += deltaX;
		particle->y += deltaY;
		particle->yaw += deltaYaw;
		//pixals - mehalkim ba function
		particle->i = particle->y / ogrid.getResolution();
		particle->j = particle->x / ogrid.getResolution();


		particle->belief = computeBelief(particle);
	}

}

double LocalizationManager::computeBelief(Particle* particle){
	LidarScan scan = hamster->getLidarScan();

	int hits=0;
	int misses=0;

	for(int i=0; i< scan.getScanSize();i++){
		//i ze a mispar shel a keren ki zarih laahpil be radians
		double angle = scan.getScanAngleIncrement() * i * DEG2RAD;

		//paga be masho, 0.001 its for 3.999 (baiot shel egol)
		if(scan.getDistance(i) < scan.getMaxRange() - 0.001){
			//zarih limzo ma ha x,y shel a michshol
			//yesh nosha x+ayeter shel a meshulash kafol cos shel a zavit shel a keren + a zavit shel a yaw ve mahpilim be DEG2RAD ki ze be maalot
			double obsX = particle->x + scan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);
			double obsY = particle->y + scan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);

			int pixelsX = obsX / ogrid.getResolution() + ogrid.getWidth() /2;
			int pixelsY = obsY / ogrid.getResolution() + ogrid.getHeight() /2;

			if (ogrid.getCell(pixelsX,pixelsY) == CELL_OCCUPIED){
				hits++;
			}else{
				misses++;
			}
		}
	}

	return (float)hits/(hits+misses);
}

void LocalizationManager::printParticles() const{
for (int i = 0;i < particles.size(); ++i) {
	Particle* particle = particles[i];
	cout << "Particle " << i << ": " << particle->x << "," << particle->y << ", Belief: " << particle->belief << endl;

}
}

vector<Particle*> LocalizationManager::getParticles(){
	return this->particles;
}
LocalizationManager::~LocalizationManager() {
	// TODO Auto-generated destructor stub
}

