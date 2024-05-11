#pragma once
#include "PoolableActor/Skill.h"

class Ducking : public Skill
{
public:
	Ducking();

	void Update();
	void set_Location(vector<2> location, bool is_Flipped) override;
};

