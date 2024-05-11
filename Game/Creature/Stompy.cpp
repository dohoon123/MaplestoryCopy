#include "stdafx.h"
#include "Stompy.h"

void Stompy::Start()
{
	max_Hp = 7500;
	current_Hp = 7500;
	collsion_Damage = 70;

	show_Damage_Pooling = new ObjectPool();

	Monster_Move.Content = "StompyStand";
	Monster_Move.Length = { 181, 179 };
	Monster_Move.Location = { -200 , -300 };
	Monster_Move.Repeatable = true;
	Monster_Move.Duration = 1;

	collide_region.Length = { 181.0f, 179.0f };

	Monster_Alert = "StompyMove";
	Alert_Length = { 181, 179 };

	Monster_Idle = "StompyStand";
	Idle_Length = { 181, 179 };

	Monster_Hit = "StompyHit";
	Hit_Length = { 178, 184 };

	Monster_Die = "StompyDie";
	Die_Length = { 179, 188 };

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

