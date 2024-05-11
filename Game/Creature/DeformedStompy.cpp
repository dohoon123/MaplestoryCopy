#include "stdafx.h"
#include "DeformedStompy.h"
#define _USE_MATH_DEFINES
#include <math.h>

void DeformedStompy::Start()
{
	e_MState = M_ALERT;
	move_Speed = 25;
	max_Hp = 50000;
    current_Hp = 50000;
	is_Boss = true;
	dying_time = 2;

	bullet_Pooling = new ObjectPool();
	show_Damage_Pooling = new ObjectPool();

	//// IMAGE ////
	Monster_Move.Content = "BOSSDS_Move";
	Monster_Move.Location = { 0,-200 };
	Monster_Move.Length = { 211, 221 };
	Monster_Move.Repeatable = true;
	Monster_Move.Duration = 1;

	Monster_Alert = "BOSSDS_Move";
	Alert_Length = { 211, 221 };

	Monster_Idle = "BOSSDS_Stand";
	Idle_Length = { 210, 219 };

	Monster_Die = "BOSSDS_Die";
	Die_Length = { 639, 380 };

	Monster_Skill = "BOSSDS_Skill1";
	Skill_Length = { 282, 288 };

	collide_region.Length = { 180, 210 };

	//// SOUND ////
	Hit.Content = "StompyDamage";
	Hit.volume = 0.1f;
	Die.Content = "StompyDie";
	Die.volume = 0.1f;

	Hp_Bar.Content = "MonsterLifeBar";
	Hp_Bar.Length = { 52, 10 };
	Hp_Bar.Location = Monster_Move.Location + vector<2>(0, 100);
	Hp_Bar.Location[1] = round(Hp_Bar.Location[1]);

	Hp_Green_Bar.Content = "MonsterLifeBar_Green";
	Hp_Green_Bar.Length = { 46, 4 };
	Hp_Green_Bar.Location = Monster_Move.Location + vector<2>(0, 100);
	Hp_Green_Bar.Location[1] = round(Hp_Green_Bar.Location[1]);
}

bool DeformedStompy::Update()
{
	set_Player_Location(GetPlayer->get_Player_Location().Location);
	////////////////////// PLAYER & MONSTER'S SKILL COLLISION CONTROL ////////////////////////
	std::vector<IObjectPoolable*>::iterator bullet_iter;

	if (!is_Dead()) {
		for (bullet_iter = bullet_Pooling->vec_PoolObjects.begin(); bullet_iter != bullet_Pooling->vec_PoolObjects.end(); ++bullet_iter) {
			if (static_cast<BossBullet*>(*bullet_iter)->is_Using) {
				Collision::Point p;
				p.Location = static_cast<BossBullet*>(*bullet_iter)->bullet.Location;
				if (Collision::Collide(p, GetPlayer->get_Rect()))//Collide
				{
					GetPlayer->set_Just_Hit(50);
					static_cast<BossBullet*>(*bullet_iter)->is_Using = false;
				}
			}
		}
	}

	if (e_MType == M_LAND && !is_On_Land) {// gravity
		Monster_Move.Location[1] -= 1;
	}

	collide_region.Location = Monster_Move.Location;

	switch (e_MState)
	{
	case M_ALERT:
		if (pattern_Time > 0) {// just hang around
			pattern_Time -= Time::Get::Delta();
			chase_Player();
		}
		else {//finish selecting pattern
			pattern_Time = 6;
			e_MState = M_SKILL;
		}
		break;
	case M_HIT:
		e_MState = M_ALERT;
		break;
	case M_DIE:
		if (dying_time > 0) {
			dying_time -= Time::Get::Delta();
			Monster_Move.Draw();
		}
		else {
			is_Hp_Bar = false;
		}
		break;
	case M_SKILL:
		Monster_Move.Content = Monster_Skill;
		Monster_Move.Length = Skill_Length;
		Monster_Move.Duration = 1.8f;

		if (bullet_Interval > 0) {
			bullet_Interval -= Time::Get::Delta();
		}
		else {
			auto bullet = bullet_Pooling->GetRecycledObject<BossBullet>();
			float const radian = static_cast<float>(M_PI) / 180.0f * ((360/12) * bullet_Num);
			vector<2> location = { cos(radian) * (Monster_Move.Length[1]/2), sin(radian) * (Monster_Move.Length[1] / 2) };

			bullet->Set_Location(Monster_Move.Location, location);

			bullet_Interval = 0.15f;
			bullet_Num -= 1;
		}
		
		if (bullet_Num == 0) {
			e_MState = M_ALERT;
			Monster_Move.Content = Monster_Alert;
			Monster_Move.Length = Alert_Length;
			Monster_Move.Duration = 1;
			Monster_Move.Playback = 0;
			bullet_Num = 12;
			
			for (auto elem : bullet_Pooling->vec_PoolObjects) {
				static_cast<BossBullet*>(elem)->Set_Destination(player_Location);
			}
		}
		Monster_Move.Draw();
		break;
	}

	if (is_Hp_Bar) {
		Hp_Bar.Location = Monster_Move.Location + vector<2>(0, Monster_Move.Length[1] / 2);
		float new_Hp = (current_Hp / max_Hp) * 46;
		float new_Location = -((46 - new_Hp)) / 2;
		Hp_Green_Bar.Length = { new_Hp, 4 };
		Hp_Green_Bar.Location = Monster_Move.Location + vector<2>(new_Location, Monster_Move.Length[1] / 2);
		Hp_Bar.Draw();
		Hp_Green_Bar.Draw();
	}

	for (auto damage : show_Damage_Pooling->vec_PoolObjects){ damage->Update(); }
	for (auto bullet : bullet_Pooling->vec_PoolObjects)		{ bullet->Update(); }

	return true;
}

void DeformedStompy::End()
{
	delete bullet_Pooling;
}