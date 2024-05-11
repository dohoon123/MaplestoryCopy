#pragma once
#include "Util/IObjectPoolable.h"

class BossBullet : public IObjectPoolable
{
public:
	Rendering::Image::Component bullet;
public:
	void Set_Location(vector<2> monster_location, vector<2> location);
	void Update();
	void Set_Destination(vector<2> dest);
public:
	bool is_Using = false;
private:
	float move_Speed = 1500;
	vector<2> destination;
	vector<2> start_Location;
	bool shoot = false;
};

