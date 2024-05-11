#pragma once
#include "Util/IObjectPoolable.h"

class Skill : public IObjectPoolable
{
public:
	Rendering::Animation::Component Skill_Animation;
	int hit_Num = 1;
	bool is_Using = false;
	bool is_Hit = false;
	Sound::Sound Effect_Sound;
	int max_hit = 1;
	int current_hit = 0;

public:
	virtual void set_Location(vector<2> location, bool is_Flipped);
	void set_Now_Hit() { is_Hit = true; }
};

