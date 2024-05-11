#pragma once
#include "Util/IObjectPoolable.h"

class VellumStone final : public IObjectPoolable
{
public:
	Rendering::Animation::Component stone;
	Rendering::Animation::Component stone_after;
	Collision::RectAngle collide_region;
public:
	void Set_Location(float x_loc);
	void Update();
public:
	bool is_Using = false;
private:
	bool is_Hit = false;
};

