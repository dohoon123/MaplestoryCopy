#include "stdafx.h"
#include "MagnumPunch.h"

MagnumPunch::MagnumPunch()
{
	Skill_Animation.Content = "MagnumPunch";
	Skill_Animation.Length = { 474 , 219 };
	Skill_Animation.Repeatable = true;
	Skill_Animation.Duration = 0.6f;
	Skill_Animation.is_flipped = false;

	Effect_Sound.Content = "MagnumPunch";
	Effect_Sound.volume = 1.0f;

	hit_Num = 3;
	max_hit = 2;
}

void MagnumPunch::Update()
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

void MagnumPunch::set_Location(vector<2> location, bool is_Flipped)
{
	is_Using = true;
	if (is_Flipped) {
		Skill_Animation.Location = location + vector<2>(50, 0);
	}
	else {
		Skill_Animation.Location = location + vector<2>(-50, 0);
	}
	Skill_Animation.is_flipped = is_Flipped;
}
