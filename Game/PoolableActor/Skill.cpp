#include "stdafx.h"
#include "Skill.h"

void Skill::set_Location(vector<2> location, bool is_Flipped)
{
	is_Using = true;
	Skill_Animation.Location = location;
	Skill_Animation.is_flipped = is_Flipped;
}
