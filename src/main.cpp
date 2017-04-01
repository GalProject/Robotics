/*
 * main.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: user
 */


#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include "Robot.h"
#include "LocalizationManager.h"

using namespace std;
using namespace HamsterAPI;

HamsterAPI::Hamster* hamster;


int main(){
	try {
		hamster = new HamsterAPI::Hamster(1);
		sleep(1);

		OccupancyGrid ogrid = hamster->getSLAMMap();

		Robot robot(hamster);
		LocalizationManager locManager(ogrid, hamster);

		locManager.initParticles();

		while (hamster->isConnected()) {

			try {
				HamsterAPI::LidarScan ld = hamster->getLidarScan();
				if (ld.getDistance(180) < 0.4) {
					hamster->sendSpeed(-0.5, 0);
					cout << "Front: " << ld.getDistance(180) << endl;
				} else if (ld.getDistance(180) < 0.8) {
					hamster->sendSpeed(0.5, 45);
					cout << "Front: " << ld.getDistance(180) << endl;
				}

				else
					hamster->sendSpeed(1.0, 0);

				cv::namedWindow("OccupancyGrid-view");
				int width = ogrid.getWidth();
				int height = ogrid.getHeight();
				unsigned char pixel;
				//CvMat* M = cvCreateMat(width, height, CV_8UC1);
				cv::Mat m = cv::Mat(width, height,CV_8UC1);
				locManager.initParticles();

				cv::Mat image = cv::Mat(width, height,CV_8UC1);





				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++) {
						if (ogrid.getCell(i, j) == CELL_FREE){
							image.at<cv::Vec3b>(i,j)[0] = 0;
							image.at<cv::Vec3b>(i,j)[1] = 0;
							image.at<cv::Vec3b>(i,j)[1] = 0;
							pixel = 255;
						}
						else if (ogrid.getCell(i, j) == CELL_OCCUPIED){
							image.at<cv::Vec3b>(i,j)[1] = 255;
							image.at<cv::Vec3b>(i,j)[1] = 255;
							image.at<cv::Vec3b>(i,j)[1] = 255;
							pixel = 255;
						}
						else if (ogrid.getCell(i,j) == CELL_UNKNOWN){
							image.at<cv::Vec3b>(i,j)[1] = 255;
							image.at<cv::Vec3b>(i,j)[1] = 255;
							image.at<cv::Vec3b>(i,j)[1] = 255;
							pixel = 255;
						}
						else
							pixel = 255;
						//cvmSet(p, i, j, pixel);


						image.at<cv::Vec3b>(i,j)[2] = 205;
						m.at<unsigned char>(i,j) = pixel;

					}

				cv::imshow("OccupancyGrid-view",m);
				cv::waitKey(1);


				robot.updatePose();
				locManager.updateParticles(robot.getDeltaX(), robot.getDeltaY(), robot.getDeltaYaw());
				locManager.printParticles();
				float myFloats[] = {};

				sleep(0.5);


			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}

