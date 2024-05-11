#include "stdafx.h"
#include "VellumStone.h"
#include "Manager/Player.h"

void VellumStone::Set_Location(float x_loc)
{
	stone.Content = "DropStone";
	stone.Length = { 247 , 1937 };
	stone.Location = { x_loc, 650 };
	stone.Repeatable = true;
	stone.Duration = 3;
	stone.is_flipped = false;

	stone_after.Content = "DropStoneAfter";
	stone_after.Length = { 306 , 301 };
	stone_after.Location = { x_loc, -150 };
	stone_after.Repeatable = true;
	stone_after.Duration = 0.8f;
	stone_after.is_flipped = false;

	collide_region.Length   = { 70, 150 };
	collide_region.Location = { x_loc, -225 };
	is_Using = true;
}

void VellumStone::Update()
{
	IObjectPoolable::Update();// check this object is recyclable
	if (is_Using) {
		if (stone.DelayTime < stone.Duration) {
			stone.DelayTime += Time::Get::Delta();
			if (!is_Hit) {
				if (stone.DelayTime > 8.0f * (stone.Duration / 9.0f)) {// last 1/6 time
					if (Collision::Collide(GetPlayer->get_Rect(), collide_region) && !GetPlayer->is_Dead()) {
						GetPlayer->set_Stunned(3.0f); 
						is_Hit = true;
					}
				}
			}
			stone.Draw();//
			return;
		}

		if (stone_after.DelayTime < stone_after.Duration) {
			stone_after.DelayTime += Time::Get::Delta();
			stone_after.Draw();
			return;
		}
		is_Using = false;
		stone.DelayTime = 0;
		stone.Playback = 0;
		stone_after.DelayTime = 0;
		stone_after.Playback = 0;
		is_Hit = false;
		SetRecycle();
	}
}
