#include "stdafx.h"
#include "ReleasePileBunker.h"

ReleasePileBunker::ReleasePileBunker()
{
	Skill_Animation.Content = "ReleasePileBunker";
	Skill_Animation.Length = { 1113 , 620 };
	Skill_Animation.Repeatable = true;
	Skill_Animation.Duration = 1.3f;
	Skill_Animation.is_flipped = false;

	Effect_Sound.Content = "ReleasePileBunker";
	Effect_Sound.volume = 0.4f;

	hit_Num = 11;
	max_hit = 2;
}

void ReleasePileBunker::Update()
{
	IObjectPoolable::Update();// check this object is recyclable

	if (is_Using) {

		Skill_Animation.DelayTime += Time::Get::Delta();

		if (Skill_Animation.DelayTime > Skill_Animation.Duration) {
			Skill_Animation.Playback = 0;
			Skill_Animation.DelayTime = 0;
			SetRecycle();
			is_Using = false;
			is_Hit = false;
			return;
		}
		Skill_Animation.Draw();
	}
}

void ReleasePileBunker::set_Location(vector<2> location, bool is_Flipped)
{
	is_Using = true;
	if (is_Flipped) {
		Skill_Animation.Location = location + vector<2>(400, 150);
	}
	else {
		Skill_Animation.Location = location + vector<2>(-400, 150);
	}
	Skill_Animation.is_flipped = is_Flipped;
}
