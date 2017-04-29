#include "Map.h"

Map::Map(OccupancyGrid &ogrid) : ogrid(ogrid), m(ogrid.getWidth(), ogrid.getHeight(), CV_8UC3, cv::Scalar(0, 0, 0)) {
	cv::namedWindow("Robot Map");
}

void Map::initMap() {
	int width = ogrid.getWidth();
	int height = ogrid.getHeight();

	unsigned char pixel;

	for (int i = 250; i < height-350; i++)
		for (int j = 250; j < width-350; j++) {
			if (ogrid.getCell(i, j) == CELL_FREE)
			{
				pixel = 255;
			}
			else if (ogrid.getCell(i, j) == CELL_OCCUPIED)
			{
				pixel = 0;
			}
			else if (ogrid.getCell(i, j) == CELL_UNKNOWN)
				pixel = 128;

			m.at<Vec3b>(2*(i-250),2*(j-250))[0] = pixel;
			m.at<Vec3b>(2*(i-250),2*(j-250))[1] = pixel;
			m.at<Vec3b>(2*(i-250),2*(j-250))[2] = pixel;
			m.at<Vec3b>(2*(i-250)+1,2*(j-250))[0] = pixel;
			m.at<Vec3b>(2*(i-250)+1,2*(j-250))[1] = pixel;
			m.at<Vec3b>(2*(i-250)+1,2*(j-250))[2] = pixel;
			m.at<Vec3b>(2*(i-250),2*(j-250)+1)[0] = pixel;
			m.at<Vec3b>(2*(i-250),2*(j-250)+1)[1] = pixel;
			m.at<Vec3b>(2*(i-250),2*(j-250)+1)[2] = pixel;
			m.at<Vec3b>(2*(i-250)+1,2*(j-250)+1)[0] = pixel;
			m.at<Vec3b>(2*(i-250)+1,2*(j-250)+1)[1] = pixel;
			m.at<Vec3b>(2*(i-250)+1,2*(j-250)+1)[2] = pixel;
		}

}

void Map::drawParticles(vector<Particle *> particles) {
	initMap();



	int size = particles.size();
	int best = size * 0.5;
	int i = 0;

	for (; i < size - best; i++) {
		int x = particles[i]->i;
		int y = particles[i]->j;
		m.at<Vec3b>(2*(x-250),2*(y-250))[0] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250))[1] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250))[2] = 255;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250))[0] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250))[1] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250))[2] = 255;
		m.at<Vec3b>(2*(x-250),2*(y-250)+1)[0] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250)+1)[1] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250)+1)[2] = 255;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250)+1)[0] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250)+1)[1] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250)+1)[2] = 255;
	}

	for (; i < size; i++) {
		int x = particles[i]->i;
		int y = particles[i]->j;
		m.at<Vec3b>(2*(x-250),2*(y-250))[0] = 255;
		m.at<Vec3b>(2*(x-250),2*(y-250))[1] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250))[2] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250))[0] = 255;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250))[1] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250))[2] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250)+1)[0] = 255;
		m.at<Vec3b>(2*(x-250),2*(y-250)+1)[1] = 0;
		m.at<Vec3b>(2*(x-250),2*(y-250)+1)[2] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250)+1)[0] = 255;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250)+1)[1] = 0;
		m.at<Vec3b>(2*(x-250)+1,2*(y-250)+1)[2] = 0;
	}

}



void Map::showMap() {
	// Show the matrix on the window
	cv::imshow("Robot Map", m);

	// Delay for 1 millisecond to allow the window to process
	// incoming events
	cv::waitKey(1);
}

Map::~Map() {
}

