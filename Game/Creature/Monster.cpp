#include "stdafx.h"
#include "Monster.h"

void Monster::Start()
{
}

bool Monster::Update()
{
	set_Player_Location(GetPlayer->get_Player_Location().Location);

	collide_region.Location = Monster_Move.Location;

	if (e_MType == M_LAND && !is_On_Land) {// gravity
		Monster_Move.Location[1] -= 1;
	}
	////////////////////// PLAYER & MONSTER COLLISION CONTROL ////////////////////////
	if (Collision::Collide(get_Rect(), GetPlayer->get_Rect())){//Collide
		if (!is_Dead())
		{ GetPlayer->set_Hit(get_Collision_Damage()); }
	}

	switch (e_MState)
	{
	case M_ALERT:
		chase_Player();
		break;
	case M_IDLE:
		if (abs(Monster_Move.Location[0] - player_Location[0]) <= 200 && 
			abs(Monster_Move.Location[1] - player_Location[1]) <= 200) {// close enough
			e_MState = M_ALERT;
			Monster_Move.Content = Monster_Alert;
			Monster_Move.Length = Alert_Length;

			float direction;
			direction = Monster_Move.Location[0] - player_Location[0];
			if (direction >= 0) {// player  monster
				Monster_Move.is_flipped = false;
				chase_Direction = 0;//go left to chase player
			}
			else {// monster  player
				Monster_Move.is_flipped = true;
				chase_Direction = 1;//go right to chase player
			}

			break;
		}

		if (move_Time > 0) {
			move_Time -= Time::Get::Delta();

			switch (init_Direction)
			{
			case 0: 
				Monster_Move.Location -= vector<2>(1, 0) * move_Speed * Time::Get::Delta();
				Monster_Move.Draw();
				break;
			case 2:
				Monster_Move.Location += vector<2>(1, 0) * move_Speed * Time::Get::Delta();
				Monster_Move.Draw();
				break;
			case 1:
				Monster_Move.Draw();
				break;
			}
		}
		else {
			Wander();
		}
		break;
	case M_HIT:
		if (hit_Recovery > 0) {
			Monster_Move.Content = Monster_Hit;
			Monster_Move.Length = Hit_Length;
			hit_Recovery -= Time::Get::Delta();
		}
		else {
			e_MState = M_ALERT;
			Monster_Move.Content = Monster_Alert;
			Monster_Move.Length = Alert_Length;
			is_hit = false;
		}
		Monster_Move.Draw();

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

	for (auto damage : show_Damage_Pooling->vec_PoolObjects)
	{
		damage->Update();
	}

	return true;
}

void Monster::End()
{
}

int Monster::select_Direction()
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 2);

	return dis(gen);
}

void Monster::set_Player_Location(vector<2> vec)
{
	player_Location = vec;
}

void Monster::chase_Player()
{
	float direction;

	if (move_Time > 0) {
		move_Time -= Time::Get::Delta();
	}
	else {
		move_Time = 1;
		direction = Monster_Move.Location[0] - player_Location[0];
		if (direction >= 0) {// player  monster
			Monster_Move.is_flipped = false;
			chase_Direction = 0;//go left to chase player
		}
		else {// monster  player
			Monster_Move.is_flipped = true;
			chase_Direction = 1;//go right to chase player
		}
	}
	if (chase_Direction) {
		Monster_Move.Location += vector<2>(1, 0) * move_Speed * Time::Get::Delta();
	}
	else {
		Monster_Move.Location -= vector<2>(1, 0) * move_Speed * Time::Get::Delta();
	}	

	Monster_Move.Draw();
}

void Monster::Wander()
{
	init_Direction = select_Direction();

	switch (init_Direction)
	{
	case 0:
		Monster_Move.Content = Monster_Alert;
		Monster_Move.Length = Alert_Length;
		Monster_Move.Location -= vector<2>(1, 0) * move_Speed * Time::Get::Delta();
		Monster_Move.is_flipped = false;
		Monster_Move.Draw();
		break;
	case 1:
		Monster_Move.Content = Monster_Idle;
		Monster_Move.Length = Idle_Length;
		Monster_Move.Draw();
		break;
	case 2:
		Monster_Move.Content = Monster_Alert;
		Monster_Move.Length = Alert_Length;
		Monster_Move.Location += vector<2>(1, 0) * move_Speed * Time::Get::Delta();
		Monster_Move.is_flipped = true;
		Monster_Move.Draw();
		break;
	}
	move_Time = 2;
}

Collision::RectAngle Monster::get_Rect() const
{
	vector<2> Length;
	float Angle = 0;
	vector<2> Location;

	Length = Monster_Move.Length;
	Location = Monster_Move.Location;

	Collision::RectAngle Rect = { Length, Angle, Location };

	return Rect;
}

bool Monster::is_Hit()
{
	return is_hit;
}

void Monster::set_Hit(int damage_taken, int hit_num, int change_rate)
{
	if (e_MState != M_ATTACK && e_MState != M_SKILL) {// monster is not attacking
		e_MState = M_HIT;
	}
	int rate = damage_taken / change_rate;
	std::mt19937 gen(rd());
	is_Hp_Bar = true;
	for (int i = 0; i < hit_num; ++i) {
		std::uniform_int_distribution<int> dis(damage_taken - rate, damage_taken + rate);
		auto damage = show_Damage_Pooling->GetRecycledObject<Damage_Effect>();
		damage->set_Location(collide_region.Location + vector<2>(0, i*26 + collide_region.Length[1]/2), dis(gen), i, false);
		current_Hp -= damage_taken;
	}

	hit_Recovery = 0.55f;
	
	Hit.Play();
	if (current_Hp <= 0) {
		Monster_Move.Content = Monster_Die;
		Monster_Move.Length = Die_Length;

		Die.Play();
		e_MState = M_DIE;
		Monster_Move.Playback = 0;

		if (is_Boss) {
			Monster_Move.Duration = 2;
			Monster_Move.Location += vector<2>(0, 100);
		}
	}
}

void Monster::set_Start_Location(int i)
{
	int y_loc = select_Direction();

	switch (y_loc)
	{
	case 0:// 1st floor
		Monster_Move.Location = vector<2>(i * 65, -200);
		break;
	case 1:
		Monster_Move.Location = vector<2>(i * 65, 100);
		break;
	case 2:
		Monster_Move.Location = vector<2>(i * 65, 400);
		break;
	}
}

bool Monster::is_Land() const
{
	if (e_MType == M_LAND) { return true; }
	else { return false; }
}

bool Monster::is_Dead() const
{
	if (e_MState == M_DIE) { return true; }
	else { return false; }
}
