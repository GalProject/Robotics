#include "LocalizationManager.h"
#include <iostream>
#include <cmath>

using namespace std;
#define PARTICLES_NUM 500
#define DEL_PARTICLE 200
#define RADIUS 8
#define ROULETTE 5
#define TRASHOLD 0.5




LocalizationManager::LocalizationManager(const OccupancyGrid &ogrid, Hamster *hamster) : ogrid(ogrid), hamster(hamster) {
	cout << "width: " << ogrid.getWidth() << ", height: " << ogrid.getHeight() << endl;
}

void LocalizationManager::getRandomNearGoodParticles() {

	std::sort(this->particles.begin(),this->particles.end(),lessThanBelief);
	int j=0;

	for (int i = PARTICLES_NUM-1 ;i > PARTICLES_NUM -1 - DEL_PARTICLE ; i--) {
		int roulette = rouletteWheel();
		do{

			int angle = rand() % 360;
			int radius = rand() % RADIUS;
			particles[j]->j = (particles[roulette]->j + radius * cos (angle));
			particles[j]->i = (particles[roulette]->i + radius * sin (angle));

		} while (ogrid.getCell(particles[j]->i, particles[j]->j) != CELL_FREE);

		particles[j]-> teta = rand() % 360;
		particles[j]-> x = (particles[j]->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution();
		particles[j]-> y = ((double)ogrid.getHeight() / 2 - particles[j]->i) * ogrid.getResolution();
		particles[j]->belief = computeBelief(particles[j]);
		j++;
	}

}

int LocalizationManager::rouletteWheel() {

	double wheel_location =0;
	double total_weight=0;
	double curr_sum;
	int index=0;
	for (int i = 1 ; i<ROULETTE ; i++) {
		total_weight += particles[PARTICLES_NUM-i]->belief;
	}

	wheel_location = (double) (rand() / (double) (RAND_MAX)) *total_weight;
	index=PARTICLES_NUM-1;
	curr_sum = particles[index]->belief;

	while (curr_sum < wheel_location && index > PARTICLES_NUM - 1 - ROULETTE ) {
		index--;
		curr_sum = curr_sum + particles[index]->belief;
	}

	return index;
}

void LocalizationManager::getRandomLocation(Particle *particle) {
	do {
		particle->j = rand() % ogrid.getWidth();
		particle->i = rand() % ogrid.getHeight();

	} while (ogrid.getCell(particle->i, particle->j) != CELL_FREE);

	particle->x = (particle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution() ;
	particle->y = ((double)ogrid.getHeight() / 2 - particle->i) * ogrid.getResolution();

	particle->teta = rand() % 360;
}

void LocalizationManager::initParticles() {
	particles.resize(PARTICLES_NUM);

	for (int i = 0; i < particles.size(); i++) {
		particles[i] = new Particle();
		getRandomLocation(particles[i]);

	}
}

double LocalizationManager::computeBelief(Particle *particle) {
	LidarScan scan = hamster->getLidarScan();

	int hits = 0;
	int misses = 0;

	for (int i = 0; i < scan.getScanSize(); i++) {
		double angle = scan.getScanAngleIncrement() * i * DEG2RAD;

		if (scan.getDistance(i) < scan.getMaxRange() - 0.001) {
			double obsX = particle->x + scan.getDistance(i) * cos(angle + particle->teta * DEG2RAD - 180 * DEG2RAD);
			double obsY = particle->y + scan.getDistance(i) * sin(angle + particle->teta * DEG2RAD - 180 * DEG2RAD);

			int pixelsY = ((double)ogrid.getHeight() / 2 -(double)( obsY / ogrid.getResolution()) );
			int pixelsX = ((double)(obsX / ogrid.getResolution()) + (double)ogrid.getWidth() / 2);

			if (ogrid.getCell(pixelsY, pixelsX) == CELL_OCCUPIED) {
				hits++;
			} else {
				misses++;
			}
		}
	}
	return (float)hits / (hits + misses);
}

void LocalizationManager::updateParticles(double dis,double deltaTeta) {
	for (int i = 0; i < particles.size(); i++) {
		Particle *particle = particles[i];

		particle->x += dis*cos(particle->teta * DEG2RAD);
		particle->y += dis*sin(particle->teta * DEG2RAD);


		particle->teta += deltaTeta;
		particle->i = (double)ogrid.getHeight() / 2 - particle->y / ogrid.getResolution() ;
		particle->j = particle->x / ogrid.getResolution() + ogrid.getWidth() / 2;
		particle->belief = computeBelief(particle);


		if (ogrid.getCell(particle->i, particle->j) != CELL_FREE) {
			if (particles[i]->belief < TRASHOLD ) {
				getRandomLocation(particle);
			}
		}
	}
	getRandomNearGoodParticles();
}

void LocalizationManager::printParticles() const {
	for (int i = 0; i < particles.size(); i++) {
		Particle *particle = particles[i];
		cout << "Particle " << i << ": " << particle->x << "," << particle->y << ", belief: " << particle->belief << endl;
	}
}

vector<Particle *> LocalizationManager::getParticles() const {
	return particles;
}



LocalizationManager::~LocalizationManager() {
}

