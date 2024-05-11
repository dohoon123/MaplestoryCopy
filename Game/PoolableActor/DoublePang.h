#pragma once
#include "PoolableActor/Skill.h"

class DoublePang : public Skill
{
public:
	DoublePang();

	void Update();
	void set_Location(vector<2> location, bool is_Flipped) override;
};
