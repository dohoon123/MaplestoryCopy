#include "stdafx.h"
#include "DoubleJump.h"

DoubleJump::DoubleJump()
{
	Skill_Animation.Content = "DoubleJump";
	Skill_Animation.Length = { 230 , 94 };
	Skill_Animation.Repeatable = true;
	Skill_Animation.Duration = 0.7f;
	Skill_Animation.is_flipped = false;

	Effect_Sound.Content = "DoubleJump";
	Effect_Sound.volume = 1.0f;
}

void DoubleJump::Update()
{
	IObjectPoolable::Update();// check this object is recyclable

	if (is_Using) {

		Skill_Animation.DelayTime += Time::Get::Delta();

		if (Skill_Animation.DelayTime > Skill_Animation.Duration) {
			Skill_Animation.Playback = 0;
			Skill_Animation.DelayTime = 0;
			SetRecycle();
			is_Using = false;
			return;
		}

		Skill_Animation.Draw();
	}
}

void DoubleJump::set_Location(vector<2> location, bool is_Flipped)
{
	is_Using = true;
	if (is_Flipped) {
		Skill_Animation.Location = location + vector<2>(5, 10);
	}
	else {
		Skill_Animation.Location = location + vector<2>(-5, 10);
	}
	Skill_Animation.is_flipped = is_Flipped;
}
