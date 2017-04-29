#include "Robot.h"
using namespace std;

Robot::Robot() {
	hamster = new HamsterAPI::Hamster(1);
	sleep(1);

	int flag =1;
		while(this->hamster->isConnected())
		{
			try{
				while(flag==1)
				{
					try{
						this->ogrid = this->hamster->getSLAMMap();
						flag = 0;
					} catch(const HamsterAPI::HamsterError & ttt){
						cout<<"please wait.."<<endl;
					}
				}
				break;
			} catch(const HamsterAPI::HamsterError & ttt){
				cout<<"please wait.."<<endl;
			}
		}
		this->map = new Map(ogrid);
		map->initMap();
}

Robot::~Robot() {
	if(this->hamster)
		delete this->hamster;
	if(this->map)
		delete this->map;
}

