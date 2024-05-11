#pragma once
#include "Util/IObjectPoolable.h"

class Damage_Effect : public IObjectPoolable
{
public:
	std::vector<Rendering::Image::Component> damage_Image;
	vector<2> start_location;
	int hit_Num = 1;
	float display_Time = 2;
	float delay_Time = 0;
public:
	void Update();
	void set_Location(vector<2> location, int damage, int delayTime, bool is_player);
};

