#ifndef PARTICLE_H_
#define PARTICLE_H_

class Particle {
public:
	int i, j;//map
	double x, y;//real
	double teta; //teta equlas to yaw
	double belief;

	void operator=(const Particle& p)
	{
		this->x = p.x;
		this->y = p.y;
		this->teta = p.teta;
		this->i = p.i;
		this->j = p.j;
		this->belief = p.belief;
	}
};

#endif /* PARTICLE_H_ */
