#pragma once
#include "PoolableActor/Damage_Effect.h"
#include "Util/ObjectPool.h"
#include "PoolableActor/MagnumPunch.h"
#include "PoolableActor/DoublePang.h"
#include "PoolableActor/DoubleJump.h"
#include "PoolableActor/ReleasePileBunker.h"
#include "PoolableActor/Ducking.h"
#include "Manager/Singleton.h"

#define GetPlayer Player::GetInstance()

enum E_PSTATE { P_STAND, P_ATTACK, P_WALK, P_JUMP, P_PRONE, P_ROPE, P_DIE };
enum E_PAIRSTATE { P_GRAVITY, P_LANDING };

class Player final : public Singleton<Player>
{
public:
	Player();
	~Player();

	virtual bool Update();

public:
	Collision::RectAngle get_Rect() const;
	Collision::Point get_Player_Location();

	bool is_Falling() { return is_falling; }
	bool is_Hanging() { return rope_Available; }
	bool is_Dead() { return (hp <= 0) ? true : false; }

public://setter
	void set_Landing();
	void set_Gravity();
	void set_Player_XLocation(float x);
	void set_Player_YLocation(float y);
	void set_Player_Move_Horizontal(float x) { Player_Animation->Location[0] += x; }

	void set_Rope_Available();
	void set_Rope_Disable();
	void set_Down_Jump_Available(bool available) { down_jump_available = available; }
	void set_Rope_X(float x);
	void set_Hit(int damage_taken);
	void set_Just_Hit(int damage_taken);
	void set_Stunned(float stun_time);

	void set_Knockback(float horizontal, float vertical);
	void set_Revive();

public://getter
	Rendering::Animation::Component* get_Player_Animation();

	int get_Bullet_Num() const { return bullet_remainder; }
	int get_Revolver_Stack() const { return revolver_Stack; }

	int get_Damage() const;
	int get_Change_Rate() const { return change_Rate; }
	float get_Bottom() const;
	float get_Ignore_Collision_Time() const;
	
	float get_Max_Hp() const { return max_hp; }
	float get_Max_Mp() const { return max_mp; }
	float get_Hp() const { return hp; }
	float get_Mp() const { return mp; }

private:
	int attack_Damage = 400;
	float max_hp;
	float max_mp;
	float hp;
	float mp;
	float hit_Recovery = 1.0f; // for normal monster body hit interval
	float stunned_Time = 0;

	int revolver_Stack = 0; // for release pile bunker
	int bullet_remainder = 6;
	float jump_Power = 0;
	UINT jump_Count = 0;// how many times to double jump
	
	vector<2> direction = {0, 0};
	float jump_Push_Power = 0;// double jump
	float dash_Push_Power = 0;//ducking, sway push power
	float walk_Power = 0;// walk
	float dash_Time = 0;// ducking, sway dash time
	float charge_Time = 0;// ducking, sway charge time

	bool touched_Ground = false;// to check speed decrease
	bool is_Flipped = false;
	bool is_falling = true;
	bool is_Ducking = false;
	bool is_Charging_Sound = false;
	bool is_Dying_Sound = false;
	bool jump_Press_Block = false;// for ducking, sway stop handling

	int change_Rate = 10;// for mastery skills
	std::string prev_Skill = "";

	float prev_YLocation;
	float move_Speed = 200;// original : 150
	float ignore_Collision_Time = 0;
	Rendering::Animation::Component* Player_Animation;
	Rendering::Animation::Component* Ducking_Animation;
	Rendering::Animation::Component* Sway_Animation;
	Rendering::Animation::Component* Stun_Animation;

	E_PSTATE e_PState = P_JUMP;
	E_PAIRSTATE e_PAirState = P_GRAVITY;

	Sound::Sound Jump;
	Sound::Sound Ducking_Charge;
	Sound::Sound Hit;
	Sound::Sound Dying;
	bool rope_Available = false;
	bool down_jump_available = false;
	float remember_Rope_X = 0;

private:
	void set_Charging_Sound();
	void draw_Player();
public:
	ObjectPool* attack_SkillPooling;
	ObjectPool* none_Attack_SkillPooling;
	ObjectPool* show_Damage_Pooling;

};