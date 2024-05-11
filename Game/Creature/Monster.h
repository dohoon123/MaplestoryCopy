#pragma once
#include "Manager/Creature.h"
#include "PoolableActor/Damage_Effect.h"
#include "Manager/Player.h"
#include "Util/ObjectPool.h"
#include <time.h>

enum E_MSTATE { M_ALERT, M_IDLE, M_HIT, M_DIE, M_ATTACK, M_SKILL};
enum E_MTYPE { M_AIR, M_LAND };

class Monster : public Creature
{
public:
	virtual void Start()	override;
	virtual bool Update()	override;
	virtual void End()		override;
protected:
	int select_Direction();
	int chase_Direction = 0;//direction when chase

	Rendering::Animation::Component Monster_Move;

	Rendering::Image::Component Hp_Bar;
	Rendering::Image::Component Hp_Green_Bar;

	const char* Monster_Alert;
	vector<2> Alert_Length;
	const char* Monster_Idle;
	vector<2> Idle_Length;
	const char* Monster_Hit;
	vector<2> Hit_Length;
	const char* Monster_Die;
	vector<2> Die_Length;
	const char* Monster_Attack;
	vector<2> Attack_Length;
	const char* Monster_Skill;
	vector<2> Skill_Length;

	int collsion_Damage = 0;

	vector<2> player_Location = {0, 0};

	E_MSTATE e_MState	= M_IDLE;
	E_MTYPE	e_MType		= M_LAND;

	float move_Speed = 50;
	float move_Time = 0;
	float dying_time = 1;
	float hit_Recovery = 0.55f;
	int init_Direction;
 
	bool is_hit = false;
	bool is_On_Land = false;
	bool is_Hp_Bar = false;
	bool is_Boss = false;

	void Wander();
	void chase_Player();

protected:
	float max_Hp = 100;
	float current_Hp = 100;
	std::random_device rd;
public:
	void set_Player_Location(vector<2> vec);
	void set_Landing() { is_On_Land = true; }
	void set_Falling() { is_On_Land = false; }

	Sound::Sound Hit;
	Sound::Sound Die;

	Collision::RectAngle get_Rect() const;

	bool Is_Boss() { return is_Boss; }
	bool is_Hit();
	bool get_Is_Hp_Bar() { return is_Hp_Bar; }

	virtual void set_Hit(int damage_taken, int hit_num, int change_rate);
	void set_Start_Location(int i);
	void set_Monster_YLocation(float y) { Monster_Move.Location[1] = y; }
	void set_Monster_Move_Horizontal(float x) { Monster_Move.Location[0] += x; }

	bool is_Land() const;
	virtual bool is_Dead() const;

	float get_Max_Hp() const { return max_Hp; }
	float get_Current_Hp() const { return current_Hp; }

	int get_Collision_Damage() const { return collsion_Damage; }
	Collision::RectAngle collide_region;

	ObjectPool* show_Damage_Pooling;// use to pooling

};