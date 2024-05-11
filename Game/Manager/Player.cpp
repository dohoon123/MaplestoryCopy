#include "stdafx.h"
#include "Player.h"
#include <iostream>

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

Player::Player()
{
	max_hp = 2000;
	max_mp = 3000;
	hp = 2000;
	mp = 3000;

	Player_Animation = new Rendering::Animation::Component();

	Player_Animation->Content = "Jump";
	Player_Animation->Location = { 300 , 200 };
	Player_Animation->Repeatable = true;
	Player_Animation->Duration = 2;
	Player_Animation->is_flipped = false;
	Player_Animation->DelayTime = 0;

	Ducking_Animation = new Rendering::Animation::Component();
	Ducking_Animation->Content = "DuckingCharge";
	Ducking_Animation->Length = { 203 , 237 };
	Ducking_Animation->Repeatable = true;
	Ducking_Animation->Duration = 0.6f;
	Ducking_Animation->is_flipped = false;
	Ducking_Animation->DelayTime = 0;

	Sway_Animation = new Rendering::Animation::Component();
	Sway_Animation->Content = "SwayCharge";
	Sway_Animation->Length = { 203 , 237 };
	Sway_Animation->Repeatable = true;
	Sway_Animation->Duration = 0.6f;
	Sway_Animation->is_flipped = true;
	Sway_Animation->DelayTime = 0;

	Stun_Animation = new Rendering::Animation::Component();
	Stun_Animation->Content = "Player_Stunned";
	Stun_Animation->Length = { 66, 18 };
	Stun_Animation->Repeatable = true;
	Stun_Animation->Duration = 0.3f;
	Stun_Animation->is_flipped = false;
	Stun_Animation->DelayTime = 0;

	///////////////////

	prev_YLocation = 200;

	jump_Power = 0;

	attack_SkillPooling = new ObjectPool();
	none_Attack_SkillPooling = new ObjectPool();
	show_Damage_Pooling = new ObjectPool();

	Jump.Content = "Jump";
	Jump.volume = 0.2f;

	Ducking_Charge.Content = "DuckingCharge";
	Ducking_Charge.volume = 0.35f;
	Ducking_Charge.loop = true;

	Hit.Content = "HitSound";
	Hit.volume = 1.0f;

	Dying.Content = "TombStone";
	Dying.volume = 1.0f;
}

Player::~Player()
{
	delete Player_Animation;
	delete attack_SkillPooling;
	delete none_Attack_SkillPooling;
}

bool Player::Update()
{

	direction = { 0, 0 };//empty

	if (hp <= 0) { 
		e_PState = P_DIE;
	}

	if (Player_Animation->Location[1] < -2000) { // when fall too deeply
		Player_Animation->Location = { 0, 0 };
	}

	if (Time::Get::Elapsed() < 5) {
		jump_Power = 0;
		Player_Animation->Location = { 0, 0 };
	}

	if (ignore_Collision_Time > 0)	{ ignore_Collision_Time -= Time::Get::Delta(); }
	
	if (dash_Time > 0) { dash_Time -= Time::Get::Delta(); }
	else { 
		dash_Push_Power = 0; 
		is_Ducking = false;
	}

	if (e_PAirState == P_LANDING) {

		jump_Count = 1;
		touched_Ground = true;

		switch (e_PState)
		{
		case P_ROPE: 
			break;
		case P_ATTACK:
			if (jump_Power < 0) {
				jump_Power = 0;
			}
			break;
		case P_STAND: case P_WALK: case P_JUMP: case P_PRONE:
			e_PState = P_STAND;
			if (jump_Power < 0) {
				jump_Power = 0;
			}
			break;
		}

		if ((!Input::Get::Key::Press(VK_LEFT) && !Input::Get::Key::Press(VK_RIGHT)) || e_PState == P_ATTACK) {
			if (abs(walk_Power) < 0.01) {
				walk_Power = 0;
			}
			else {
				if (walk_Power > 0) {
					walk_Power -= Time::Get::Delta() * 10;
				}
				else {
					walk_Power += Time::Get::Delta() * 10;
				}
			}
		}
	}

	if (touched_Ground) {
		if (abs(jump_Push_Power) > 0.01) {
			jump_Push_Power /= 1.01f;
		}
		else {
			jump_Push_Power = 0;
		}
	}

	if (e_PAirState == P_GRAVITY) {

		switch (e_PState)
		{
		case P_ROPE: case P_PRONE:
			break;
		case P_ATTACK:
			direction[1] -= 1;// gravity
			if (jump_Power > -2.5) {
				jump_Power -= 10 * Time::Get::Delta();
			}
			break;
		case P_STAND: case P_WALK:
			e_PState = P_JUMP;
			break;
		case P_JUMP:
			direction[1] -= 1;// gravity
			if (jump_Power > -2.5) {
				jump_Power -= 10 * Time::Get::Delta();
			}
			break;
		case P_DIE:
			direction[1] -= 1;// gravity
			jump_Power = 0;
			break;
		}
	}

	stunned_Time -= Time::Get::Delta();

	if (stunned_Time > 0) {// can't give inputs while stunned
		Stun_Animation->Location[0] = Player_Animation->Location[0];
		Stun_Animation->Location[1] = Player_Animation->Location[1] + Player_Animation->Length[1] / 2;
		dash_Push_Power = 0; jump_Push_Power = 0; walk_Power = 0; jump_Power = 0;
		Player_Animation->Location += (direction + vector<2>(dash_Push_Power + jump_Push_Power + walk_Power, jump_Power)) * move_Speed * Time::Get::Delta();

		if (Player_Animation->Location[1] - prev_YLocation <= 0.01f) {
			is_falling = true;
		}
		else {
			is_falling = false;
		}

		prev_YLocation = Player_Animation->Location[1];
		if (hit_Recovery > 0) { hit_Recovery -= Time::Get::Delta(); }

		draw_Player();
		Stun_Animation->Draw();

		for (auto skill : attack_SkillPooling->PoolObjects) { skill.second->Update(); }
		for (auto skill : none_Attack_SkillPooling->PoolObjects) { skill.second->Update(); }

		return true;
	}

	if (Input::Get::Key::Press(VK_LEFT) && !is_Ducking) { // left moving
		switch (e_PState)
		{
		case P_STAND: case P_WALK:
			e_PState = P_WALK;
			if (walk_Power > -1.5f) {
				walk_Power -= Time::Get::Delta() * 10;
			}
			is_Flipped = false;
			break;
		case P_JUMP:
			is_Flipped = false;
			break;
		case P_ATTACK: case P_ROPE: case P_PRONE:
			break;
		}
	}

	if (Input::Get::Key::Press(VK_RIGHT) && !is_Ducking) {// right moving
		switch (e_PState)
		{
		case P_STAND: case P_WALK:
			e_PState = P_WALK;
			if (walk_Power < 1.5f) {
				walk_Power += Time::Get::Delta() * 10;
			}
			is_Flipped = true;
			break;
		case P_JUMP:
			is_Flipped = true;
			break;
		case P_ATTACK: case P_ROPE:  case P_PRONE:
			break;
		}
	}

	if (Input::Get::Key::Up(VK_RIGHT) || Input::Get::Key::Up(VK_LEFT)) { // stop moving
		switch (e_PState)
		{
		case P_WALK:
			e_PState = P_STAND;
			break;
		}
	}

	if (Input::Get::Key::Press(VK_DOWN)) {
		switch (e_PState)
		{
		case P_STAND: case P_WALK: {
			if (rope_Available) {
				e_PState = P_ROPE;
				e_PAirState = P_LANDING;//////////////////// alert!!!!
				jump_Power = 0;
				jump_Push_Power = 0;
				walk_Power = 0;
				Player_Animation->Location[0] = remember_Rope_X;
				is_Flipped = true;
				break;
			}
			else {
				e_PState = P_PRONE;
				walk_Power = 0;
				direction[0] = 0;
			}
			break;
		}
		case P_ATTACK: case P_JUMP: case P_PRONE:
			break;
		case P_ROPE:
			direction[1] -= 1;
			break;
		}
	}

	if (Input::Get::Key::Up(VK_DOWN)) {
		switch (e_PState)
		{
		case P_PRONE:
			e_PState = P_STAND;
			break;
		}
	}

	if (Input::Get::Key::Press(VK_SPACE) && e_PAirState == P_LANDING && is_Ducking) {// ducking stop handling
		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_ATTACK:
			dash_Push_Power = 0;
			walk_Power = 0;
			is_Ducking = false;
			jump_Press_Block = true;
			break;
		case P_JUMP: case P_ROPE: case P_PRONE:
			break;
		}
	}
	if (Input::Get::Key::Up(VK_SPACE)) {// ducking stop handling
		jump_Press_Block = false;
	}


	if (Input::Get::Key::Press(VK_SPACE) && e_PState == P_PRONE && Input::Get::Key::Press(VK_DOWN) && !jump_Press_Block && down_jump_available) {// jump to underground
		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_PRONE:
			e_PState = P_JUMP;
			e_PAirState = P_GRAVITY;

			walk_Power = 0;
			jump_Power = 1.2f;
			ignore_Collision_Time = 0.3f;
			Jump.Play();
			break;
		case P_ATTACK: case P_JUMP: case P_ROPE:
			break;
		}
	}
	else if (Input::Get::Key::Press(VK_SPACE) && e_PAirState == P_LANDING && !jump_Press_Block) {// normal jump only on ground
		switch (e_PState)
		{
		case P_STAND: case P_WALK:
			e_PState = P_JUMP;
			jump_Power = 3.7f;
			Jump.Play();
			break;
		case P_ATTACK: case P_JUMP: case P_PRONE: case P_ROPE:
			break;
		}
	}
	else if (Input::Get::Key::Down(VK_SPACE) && e_PAirState == P_GRAVITY && e_PState == P_JUMP && !jump_Press_Block) {// double jump only on air
		switch (e_PState)
		{
		case P_JUMP:
		{
			if (Input::Get::Key::Down(VK_SPACE) && jump_Count == 1) {
				walk_Power /= 10; // eliminate resting movements

				jump_Count = 0;
				touched_Ground = false;
				jump_Power = 3.0f;

				auto skill = none_Attack_SkillPooling->GetRecycledObject<DoubleJump>("DoubleJump");
				skill->set_Location(Player_Animation->Location, is_Flipped);

				skill->Effect_Sound.Play();
				mp -= 5;
				if (is_Flipped) {
					jump_Push_Power = 3.0f; 
				}
				else { 
					jump_Push_Power = -3.0f; 
				}
			}
			break;
		}
		case P_ATTACK: case P_PRONE: case P_ROPE: case P_STAND: case P_WALK:
			break;
		}
	}


	if (Input::Get::Key::Down(VK_SPACE) && Input::Get::Key::Press(VK_LEFT)) {// rope jump left
		switch (e_PState)
		{
		case P_ROPE:
			e_PState = P_JUMP;
			touched_Ground = false;
			jump_Push_Power = -1.2f;
			jump_Count = 1;
			jump_Power = 2.0f;
			Jump.Play();
			break;
		}
	}

	if (Input::Get::Key::Down(VK_SPACE) && Input::Get::Key::Press(VK_RIGHT)) {// rope jump right
		switch (e_PState)
		{
		case P_ROPE:
			e_PState = P_JUMP;
			touched_Ground = false;
			jump_Push_Power = 1.2f;
			jump_Count = 1;
			jump_Power = 2.0f;
			Jump.Play();
			break;
		}
	}


	if (Input::Get::Key::Press(VK_UP)) {
		if (rope_Available) {
			switch (e_PState)
			{
			case P_STAND: case P_WALK: case P_JUMP: case P_PRONE:
				e_PState = P_ROPE;
				e_PAirState = P_LANDING;
				jump_Power = 0;
				jump_Push_Power = 0;
				walk_Power = 0;
				Player_Animation->Location[0] = remember_Rope_X;
				is_Flipped = true;
				break;
			case P_ATTACK: 
				break;
			case P_ROPE:
				direction[1] += 1.0f;
				e_PAirState = P_LANDING;
				jump_Power = 0;
				break;
			}
		}
	}

	if (Input::Get::Key::Down(0x41)) {// A - MagnumPunch   

		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_JUMP: case P_PRONE:
		{
			e_PState = P_ATTACK;

			Player_Animation->Content = "Punch";
			Player_Animation->Playback = 0;
			Player_Animation->Duration = 0.5f;
			Player_Animation->Length = { 84, 74 };

			if (e_PAirState == P_LANDING) walk_Power = 0; // eliminate resting movements only on ground
			if (bullet_remainder >= 1) { 
				bullet_remainder--; 
				if (revolver_Stack < 6) revolver_Stack += 1;
			}
			auto skill = attack_SkillPooling->GetRecycledObject<MagnumPunch>("MagnumPunch");
			skill->set_Location(Player_Animation->Location, is_Flipped);

			skill->Effect_Sound.Play();
			mp -= 10;

			prev_Skill = "MagnumPunch";

			break;
		}
		case P_ROPE: case P_ATTACK:
			break;
		}
	}
	if (Input::Get::Key::Down(0x53)) {// S - DoublePang   

		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_JUMP: case P_PRONE: case P_ATTACK:
		{
			if (e_PState == P_ATTACK) {
				if (prev_Skill != "MagnumPunch") break;
			}

			e_PState = P_ATTACK;
			
			Player_Animation->Content = "Uppercut";
			Player_Animation->Playback = 0;
			Player_Animation->Duration = 0.75f;
			Player_Animation->Length = { 67, 77 };

			if (e_PAirState == P_LANDING) walk_Power = 0; // eliminate resting movements only on ground
			if (bullet_remainder >= 1) {
				bullet_remainder--;
				if (revolver_Stack < 6) revolver_Stack += 1;
			}

			auto skill = attack_SkillPooling->GetRecycledObject<DoublePang>("DoublePang");
			skill->set_Location(Player_Animation->Location, is_Flipped);

			skill->Effect_Sound.Play();
			mp -= 15;

			prev_Skill = "DoublePang";

			break;
		}
		case P_ROPE:
			break;
		}
	}

	if (Input::Get::Key::Press(0x44) && e_PState != P_DIE) {// D - Ducking charging
		Ducking_Animation->is_flipped = is_Flipped;
		charge_Time += Time::Get::Delta();// charging
		set_Charging_Sound();
		is_Charging_Sound = true;

		if (!is_Flipped) {
			Ducking_Animation->Location = Player_Animation->Location + vector<2>(50, -5);
		}
		else {
			Ducking_Animation->Location = Player_Animation->Location + vector<2>(-50, -5);
		}
		Ducking_Animation->Draw();
	}

	if (Input::Get::Key::Up(0x44) && e_PState != P_DIE) {// D - Ducking use

		Ducking_Charge.Stop();
		is_Charging_Sound = false;

		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_ATTACK:
		{
			if (e_PAirState == P_LANDING && e_PState != P_ROPE) {
				if (charge_Time < 0.25) { // not enough charging -> no cancel
					charge_Time = 0; // charging stopped anyway
					break;
				}
				is_Ducking = true;

				bullet_remainder += 2;
				if (bullet_remainder > 6) {
					bullet_remainder = 6;
				}

				e_PState = P_ATTACK;

				walk_Power = 0;
				charge_Time = 0; // charging stopped anyway
				Player_Animation->DelayTime = Player_Animation->Duration;// to cancel doublepang delay
				Player_Animation->Playback = 0;

				if (!is_Flipped) { dash_Push_Power = - 3; }
				else { dash_Push_Power = 3; }
				dash_Time = 0.3f;
				auto skill = none_Attack_SkillPooling->GetRecycledObject<Ducking>("Ducking");
				skill->set_Location(Player_Animation->Location, is_Flipped);
				skill->Effect_Sound.Play();
			}
			break;
		}
		case P_ROPE: case P_JUMP: case P_PRONE:
			break;
		}
	}

	if (Input::Get::Key::Press(0x46) && e_PState != P_DIE) {// F - Sway charging
		Sway_Animation->is_flipped = is_Flipped;
		charge_Time += Time::Get::Delta();// charging
		set_Charging_Sound();
		is_Charging_Sound = true;

		if (!is_Flipped) {
			Sway_Animation->Location = Player_Animation->Location + vector<2>(-50, 5);
		}
		else {
			Sway_Animation->Location = Player_Animation->Location + vector<2>(50, 5);
		}
		Sway_Animation->Draw();
	}

	if (Input::Get::Key::Up(0x46)) {// F - Sway use

		Ducking_Charge.Stop();
		is_Charging_Sound = false;

		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_ATTACK:
		{
			if (e_PAirState == P_LANDING && e_PState != P_ROPE) {
				if (charge_Time < 0.25) { // not enough charging -> no cancel
					charge_Time = 0; // charging stopped anyway
					break;
				}
				is_Ducking = true;

				bullet_remainder += 2;
				if (bullet_remainder > 6) {
					bullet_remainder = 6;
				}

				e_PState = P_ATTACK;

				walk_Power = 0;
				charge_Time = 0; // charging stopped anyway
				Player_Animation->DelayTime = Player_Animation->Duration;// to cancel doublepang delay
				Player_Animation->Playback = 0;

				if (!is_Flipped) { dash_Push_Power = 3; }
				else { dash_Push_Power = -3; }
				dash_Time = 0.3f;
				auto skill = none_Attack_SkillPooling->GetRecycledObject<Ducking>("Ducking");
				skill->set_Location(Player_Animation->Location, !is_Flipped);
				skill->Effect_Sound.Play();
			}
			break;
		}
		case P_ROPE: case P_JUMP: case P_PRONE:
			break;
		}
	}

	if (Input::Get::Key::Down(VK_SHIFT) && revolver_Stack == 6) {// Shift - PileBunker

		switch (e_PState)
		{
		case P_STAND: case P_WALK: case P_JUMP: case P_PRONE:
		{
			e_PState = P_ATTACK;
			Player_Animation->Content = "PileBunker";
			Player_Animation->Playback = 0;
			Player_Animation->Duration = 1.2f;
			Player_Animation->Length = { 63, 81 };

			if (e_PAirState == P_LANDING) walk_Power = 0; // eliminate resting movements only on ground
			revolver_Stack = 0;

			if (!is_Flipped) { dash_Push_Power = -1; }
			else { dash_Push_Power = 1; }
			dash_Time = 0.6f;

			auto skill = attack_SkillPooling->GetRecycledObject<ReleasePileBunker>("ReleasePileBunker");
			skill->set_Location(Player_Animation->Location, is_Flipped);

			skill->Effect_Sound.Play();
			break;
		}
		case P_ROPE: case P_ATTACK:// 밧줄타기, 이미 다른 공격 중엔 사용 불가능
			break;
		}
	}

	if (is_Dead()) {
		dash_Push_Power = 0; jump_Push_Power = 0; walk_Power = 0;stunned_Time = 0;
		if (jump_Power > 0) {
			jump_Power = 0;
		}
	}
	Player_Animation->Location += (direction + vector<2>(dash_Push_Power + jump_Push_Power + walk_Power, jump_Power)) * move_Speed * Time::Get::Delta();
	Player_Animation->is_flipped = is_Flipped;

	if (Player_Animation->Location[1] - prev_YLocation <= 0.01f) {
		is_falling = true;
	}
	else {
		is_falling = false;
	}
	prev_YLocation = Player_Animation->Location[1];

	if (hit_Recovery > 0) {
		hit_Recovery -= Time::Get::Delta();
	}//

	////////////////// PLAYER DRAWING PHASE ////////////////////
	draw_Player();

	////////////////// ATTACK //////////////////
	for (auto skill : attack_SkillPooling->PoolObjects) { skill.second->Update(); }

	////////////////// NONE ATTACK //////////////////
	for (auto skill : none_Attack_SkillPooling->PoolObjects) { skill.second->Update(); }

	////////////////// DAMAGE //////////////////
	for (auto damage : show_Damage_Pooling->vec_PoolObjects) { damage->Update(); }

	return true;
}

Collision::RectAngle Player::get_Rect() const
{
	vector<2> Length;
	float Angle = 0;
	vector<2> Location;

	Length = Player_Animation->Length;
	Location = Player_Animation->Location;

	Collision::RectAngle Rect = {Length, Angle, Location};

	return Rect;
}

Collision::Point Player::get_Player_Location()
{
	vector<2> Location;
	Location = Player_Animation->Location;
	Collision::Point point;
	point.Location = Location;

	return point;
}

void Player::set_Landing() { e_PAirState = P_LANDING; }
void Player::set_Gravity() { e_PAirState = P_GRAVITY; }
void Player::set_Rope_Available() { rope_Available = true; }
void Player::set_Player_YLocation(float y) { Player_Animation->Location[1] = y + Player_Animation->Length[1]/2; }

void Player::set_Player_XLocation(float x)
{
	Player_Animation->Location[0] = x;
}

void Player::set_Rope_Disable(){
	rope_Available = false;
	if (e_PState == P_ROPE) { e_PState = P_JUMP; }
}

void Player::set_Rope_X(float x) { remember_Rope_X = x; }
void Player::set_Hit(int damage_taken) {
	if (hit_Recovery < 0) {
		hp -= damage_taken;
		hit_Recovery = 1.0f;
		auto damage = show_Damage_Pooling->GetRecycledObject<Damage_Effect>();
		damage->set_Location(Player_Animation->Location + vector<2>(0, Player_Animation->Length[1] / 2), damage_taken, 0, true);
	}
}

void Player::set_Just_Hit(int damage_taken) {
	hp -= damage_taken;
	auto damage = show_Damage_Pooling->GetRecycledObject<Damage_Effect>();
	damage->set_Location(Player_Animation->Location + vector<2>(0, Player_Animation->Length[1] / 2), damage_taken, 0, true);
}

Rendering::Animation::Component* Player::get_Player_Animation() { return Player_Animation; }
void Player::set_Stunned(float stun_time) { stunned_Time = stun_time; }
void Player::set_Knockback(float horizontal, float vertical){
	touched_Ground = false;
	e_PAirState = P_GRAVITY;
	jump_Push_Power += horizontal;
	jump_Power		+= vertical;
}
void Player::set_Revive()
{
	if (e_PState == P_DIE) { e_PState = P_STAND; hp = max_hp; mp = max_mp; }
}
int Player::get_Damage() const { return attack_Damage; }
float Player::get_Bottom() const {
	float y = Player_Animation->Location[1] - Player_Animation->Length[1] / 2;//player foot
	return y;
}

float Player::get_Ignore_Collision_Time() const { return ignore_Collision_Time; }

void Player::set_Charging_Sound() {
	if (!is_Charging_Sound) {
		Ducking_Charge.Play();
	}
}

void Player::draw_Player()
{
	switch (e_PState)
	{
	case P_STAND:
		Player_Animation->Content = "Stand";
		Player_Animation->Length = { 55, 77 };
		Player_Animation->Duration = 4;
		Player_Animation->Draw();
		break;
	case P_ATTACK:
		if (Player_Animation->DelayTime < Player_Animation->Duration) {
			Player_Animation->DelayTime += Time::Get::Delta();
			Player_Animation->Draw();
		}
		else {
			Player_Animation->Playback = 0;
			Player_Animation->DelayTime = 0;
			e_PState = P_JUMP;
		}
		break;
	case P_WALK:
		Player_Animation->Content = "Walk";
		Player_Animation->Length = { 55.0f, 77.0f };
		Player_Animation->Duration = 0.3f;
		Player_Animation->Draw();
		break;
	case P_JUMP:
		Player_Animation->Content = "Jump";
		Player_Animation->Length = { 55.0f, 75.0f };
		Player_Animation->Draw();
		break;
	case P_PRONE:
		Player_Animation->Content = "Prone";
		Player_Animation->Length = { 64.0f, 77.0f };
		Player_Animation->Draw();
		break;
	case P_ROPE:
		Player_Animation->Content = "Rope";
		Player_Animation->Length = { 52, 78 };
		Player_Animation->Draw();
		break;
	case P_DIE:
		Player_Animation->Content = "Die";
		Player_Animation->Length = { 55, 74 };
		Player_Animation->Draw();
		break;
	}

}
