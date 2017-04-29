#ifndef ROBOT_H_
#define ROBOT_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Robot.h"
#include "LocalizationManager.h"
#include "Map.h"

using namespace HamsterAPI;

class Robot {

private:
	Hamster *hamster;
	Map * map;
	OccupancyGrid ogrid;

public:
	Robot();
	virtual ~Robot();


	void setHamster(Hamster * ham){
		this->hamster = ham;
	}
	Hamster* getHamster(){
		return this->hamster;
	}
	void setMap(Map* m){
		this->map = m;
	}
	Map* getMap(){
		return this->map;
	}
	void setOccupancyGrid(OccupancyGrid og){
		this->ogrid = og;
	}
	OccupancyGrid& getOccupancyGrid(){
		return this->ogrid;
	}

};

#endif /* ROBOT_H_ */
