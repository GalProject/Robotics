#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Particle.h"
#include <vector>

using namespace std;
using namespace HamsterAPI;
using namespace cv;

class Map {
private:
	cv::Mat m;
	const OccupancyGrid &ogrid;

public:
	Map(OccupancyGrid &ogrid);
	void initMap();
	void drawParticles(vector<Particle *> particles);
	void showMap();
	virtual ~Map();
};

#endif /* MAP_H_ */
