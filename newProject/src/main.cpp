#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include "Robot.h"
#include "LocalizationManager.h"
#include "Map.h"

using namespace std;
using namespace HamsterAPI;


int main() {

	Robot* r;

	try {
		r = new Robot();
		float count = 0;
		LocalizationManager manager(r->getOccupancyGrid(), r->getHamster());

		manager.initParticles();

		while (r->getHamster()->isConnected()) {
			try {
				r->getMap()->showMap();

				std::clock_t start;
				double duration;
				start = std::clock();


				do{
					LidarScan scan = r->getHamster()->getLidarScan();

					if (scan.getDistance(180) < 0.4) {
						r->getHamster()->sendSpeed(-0.5, 0);
						cout << "Front: " << scan.getDistance(180) << endl;
					} else if (scan.getDistance(180) < 0.8) {
						r->getHamster()->sendSpeed(0.5, 45);
						cout << "Front: " << scan.getDistance(180) << endl;
					}
					else
						r->getHamster()->sendSpeed(1.0, 0);
						duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
				}while(duration<0.5);


				Speed s = r->getHamster()->getSpeed();
				float speed = s.getVelocity();
				float wheelsAngle = s.getWheelsAngle();

				if(speed>0){
					if(wheelsAngle>0)//left and forward
						manager.updateParticles(0.01,45);

					else if(wheelsAngle==0)
						manager.updateParticles(0.16,0);

					else if(wheelsAngle<0)//right and forward
						manager.updateParticles(0.01,-45);
				}

				r->getMap()->drawParticles(manager.getParticles());
				count =0;

			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}
		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}

	delete r;
	return 0;
}

