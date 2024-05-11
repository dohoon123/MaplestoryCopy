#include "stdafx.h"
#include "DoublePang.h"

DoublePang::DoublePang()
{
	Skill_Animation.Content = "DoublePang";
	Skill_Animation.Length = { 435 , 378 };
	Skill_Animation.Repeatable = true;
	Skill_Animation.Duration = 0.6f;
	Skill_Animation.is_flipped = false;

	Effect_Sound.Content = "DoublePang";
	Effect_Sound.volume = 1.0f;

	hit_Num = 4;
	max_hit = 2;
}

void DoublePang::Update()
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

void DoublePang::set_Location(vector<2> location, bool is_Flipped)
{
	is_Using = true;
	if (is_Flipped) {
		Skill_Animation.Location = location + vector<2>(50, 85);
	}
	else {
		Skill_Animation.Location = location + vector<2>(-50, 85);
	}
	Skill_Animation.is_flipped = is_Flipped;
}
