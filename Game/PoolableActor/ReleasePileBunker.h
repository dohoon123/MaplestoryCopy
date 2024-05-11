#pragma once
#include "PoolableActor/Skill.h"

class ReleasePileBunker : public Skill
{
public:
	ReleasePileBunker();

	void Update();
	void set_Location(vector<2> location, bool is_Flipped) override;
};

