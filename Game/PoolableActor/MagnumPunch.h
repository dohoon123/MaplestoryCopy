#pragma once
#include "PoolableActor/Skill.h"

class MagnumPunch : public Skill
{
public:
	MagnumPunch();

	void Update();
	void set_Location(vector<2> location, bool is_Flipped) override;
};

