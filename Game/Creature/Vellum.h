#pragma once
#include "Monster.h"
#include "PoolableActor/VellumStone.h"
#include "PoolableActor/VellumTale.h"
#include "Manager/Player.h"

const float STONE_TIME = 9;
const float TALE_TIME = 2.5f;
const float PATTERN_TIME = 5;
const float START_CORRECTION	= 614;
const float START_CORRECTION_Y	= 62;
const float SHORT_CORRECTION	= 391;
const float SHORT_CORRECTION_Y	= 64;
const float MIDDLE_CORRECTION	= 535;
const float MIDDLE_CORRECTION_Y = 60;
const float LONG_CORRECTION		= 450;
const float LONG_CORRECTION_Y	= 60;
const float FIREBALL_TIME		= 0.8f;

const float KNOCKBACK_HORIZONTAL = 1.4f;
const float KNOCKBACK_VERTICAL	 = 4.8f;


enum class E_VSTATE { V_MOVE, V_SKILL, V_DIE };
enum class E_VSKILLSTATE { V_FIREBALL, V_DIVE, V_LONGNECK, V_SHORTNECK };

class Vellum final : public Monster
{
public:
	virtual void Start()	override;
	virtual bool Update()	override;
	virtual void End()		override;

public:
	ObjectPool* Tale_Pooling;
	ObjectPool* Stone_Pooling;
	float pattern_Time = 6;// time to select next pattern
	float stone_Time = STONE_TIME;
	float tale_Time = TALE_TIME;
	float Elapsed_Time = 0;
	UINT neck_stage = 0;
	UINT pattern_num = 0;

	E_VSTATE      e_VState;
	E_VSKILLSTATE e_VSkillState = E_VSKILLSTATE::V_LONGNECK;
	E_VSKILLSTATE prev_Pattern; // to select btw dive & fireball;

private:
	Sound::Sound rise;
	Sound::Sound sink;
	Sound::Sound longneck_Rise;
	Sound::Sound fireball;

	float loc_Arr[6] = { -1200, -1000, -800, -600, -400, -200 };
	float tale_interval_Arr[3] = { -1.2f, -1.8f, 0.5f };

	Rendering::Animation::Component Vellum_Warning;
	Rendering::Image::Component test;
	Collision::RectAngle attack_region;
	void set_Vellum_Move();
	__forceinline void set_Neck_Stage();
	void select_Pattern();

	bool start_motion = false;

	void Set_Rise_Sound();	
	void Set_Sink_Sound();
	void Set_LongNeck_Sound();
	void Set_Fireball_Sound();

	bool is_Rise_Sound	   = false;
	bool is_Sink_Sound	   = false;
	bool is_LongRise_Sound = false;
	bool is_Fireball_Sound = false;
	
	bool is_Player_Hit = false;
public:
	virtual void set_Hit(int damage_taken, int hit_num, int change_rate) override;
	virtual bool is_Dead() const override;
};

