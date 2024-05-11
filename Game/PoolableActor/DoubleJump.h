#pragma once
#include "PoolableActor/Skill.h"

class DoubleJump : public Skill
{
public:
	DoubleJump();

	void Update();
	void set_Location(vector<2> location, bool is_Flipped) override;
};

