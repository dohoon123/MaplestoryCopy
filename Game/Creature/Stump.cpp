#include "stdafx.h"
#include "Stump.h"

void Stump::Start()
{

	max_Hp = 2000;
	current_Hp = 2000;
	collsion_Damage = 30;

	show_Damage_Pooling = new ObjectPool();

	Monster_Move.Content = "DarkAxeStump";
	Monster_Move.Length = { 80.0f, 80.0f };
	Monster_Move.Location = { -200 , -300 };
	Monster_Move.Repeatable = true;
	Monster_Move.Duration = 1;

	collide_region.Length = { 80.0f, 80.0f };

	Monster_Alert = "DarkAxeStump";
	Alert_Length = { 80, 80 };

	Monster_Idle = "DarkAxeStumpStand";
	Idle_Length = { 80, 80 };

	Monster_Hit = "DarkAxeStumpHit";
	Hit_Length = { 59, 93 };

	Monster_Die = "DarkAxeStumpDie";
	Die_Length = { 66, 92 };

	Hit.Content = "StumpDamage";
	Hit.volume = 0.1f;

	Die.Content = "StumpDie";
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

