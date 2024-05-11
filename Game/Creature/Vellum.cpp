#include "stdafx.h"
#include "Vellum.h"

void Vellum::Start()
{
	move_Speed = 90;
	max_Hp = 200000;
	current_Hp = 200000;
	is_Boss = true;
	dying_time = 2;

	Tale_Pooling = new ObjectPool();
	Stone_Pooling = new ObjectPool();
	show_Damage_Pooling = new ObjectPool();

	//// ANIMATION ////
	Monster_Move.Content = "VellumMove";
	Monster_Move.Length = { 412, 206 };
	Monster_Move.Location = { 0, -150 };
	Monster_Move.Repeatable = true;
	Monster_Move.Duration = 2;

	Vellum_Warning.Content = "VellumWarning";
	Vellum_Warning.Length = { 996, 776 };
	Vellum_Warning.Repeatable = true;
	Vellum_Warning.Duration = 0.6f;

	collide_region.Length = { 400, 400 };
	collide_region.Location[1] = -100;
	attack_region.Length  = { 302, 400 };
	attack_region.Location[1] = -100;

	e_VState = E_VSTATE::V_MOVE;// start as move

	//// SOUND ////
	rise.Content			= "VellumShortNeck";
	rise.volume				= 1.0f;
	longneck_Rise.Content	= "VellumLongNeckRise";
	longneck_Rise.volume	= 1.0f;
	fireball.Content		= "VellumFireball";
	fireball.volume			= 1.0f;
	sink.Content			= "VellumTaleSink";
	sink.volume				= 1.0f;

}

bool Vellum::Update()
{
	set_Player_Location(GetPlayer->get_Player_Location().Location);//find player location
	collide_region.Location[0] = Vellum_Warning.Location[0];
	attack_region.Location[0] = Vellum_Warning.Location[0] + 35;
	attack_region.Length = { 300, 400 };

	////// STONE TIMER //////
	stone_Time -= Time::Get::Delta();
	if (stone_Time < 0 && e_VState != E_VSTATE::V_DIE) {
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 5);
		int first_stone_loc = dis(gen);
		stone_Time = STONE_TIME;
		for (int i = 0; i < 7; i++) {
			auto stone = Stone_Pooling->GetRecycledObject<VellumStone>();
			stone->Set_Location(i * 300.0f + loc_Arr[first_stone_loc]);
		}
	}
	////// PATTERN TIMER //////
	switch (e_VState)
	{
	case E_VSTATE::V_DIE:
		if (Vellum_Warning.DelayTime < 2.0f) {
			Vellum_Warning.Length = { 996, 776 };
			Vellum_Warning.DelayTime += Time::Get::Delta();
			Vellum_Warning.Draw();
			Monster_Move.Playback = 0;
			Monster_Move.DelayTime = 0;
			neck_stage = 0;
			break;
		}
		start_motion = true;
		Monster_Move.DelayTime += Time::Get::Delta();

		switch (neck_stage)
		{
		case 0://rise
			Monster_Move.Content = "VellumLongNeckRise";
			Monster_Move.Length = { 996, 776 };
			Monster_Move.Duration = 0.7f;
			if (Monster_Move.DelayTime < 0.7f) {
				break;
			}
			set_Neck_Stage();
			break;
		case 1://die
			Monster_Move.Content = "VellumDie";
			Monster_Move.Length = { 1008, 793 };
			Monster_Move.Duration = 2.0f;
			if (Monster_Move.DelayTime < 2.0f) {
				break;
			}
			// change to move motion
			start_motion = false;
			break;
		}
		is_Hp_Bar = false;
		break;
	case E_VSTATE::V_MOVE:
		if (current_Hp <= 0) {
			e_VState = E_VSTATE::V_DIE;
			Monster_Move.Playback = 0;
			Monster_Move.DelayTime = 0;
			neck_stage = 0;
		}
		
		Monster_Move.DelayTime += Time::Get::Delta();
		Vellum_Warning.Location = vector<2>(Monster_Move.Location[0], 40);
		chase_Player();// follow player when selecting pattern
		pattern_Time -= Time::Get::Delta();
		if (pattern_Time < 0) {
			pattern_Time = PATTERN_TIME;
			Monster_Move.DelayTime = 0;
			e_VState = E_VSTATE::V_SKILL;
			select_Pattern();
		}
		break;
	case E_VSTATE::V_SKILL:
		Monster_Move.DelayTime += Time::Get::Delta();
		Vellum_Warning.DelayTime += Time::Get::Delta();
		switch (e_VSkillState)
		{
		case E_VSKILLSTATE::V_FIREBALL:

			tale_Time -= Time::Get::Delta();//tale pattern
			if (tale_Time < 0) {
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(0, 2);
				int tale_interval = dis(gen);
				tale_Time = TALE_TIME + tale_interval_Arr[tale_interval];
				auto tale = Tale_Pooling->GetRecycledObject<VellumTale>();
				tale->Set_Location(player_Location[0]);
			}

			switch (neck_stage)
			{
			case 0://rise - hit player
				Monster_Move.Length = { 996, 776 };
				if (Vellum_Warning.DelayTime < 2.0f) {
					Vellum_Warning.Draw();
					Monster_Move.Playback = 0;
					Monster_Move.DelayTime = 0;
					break;
				}
				Set_LongNeck_Sound();
				start_motion = true;
				Monster_Move.Content = "VellumLongNeckRise";
				Monster_Move.Duration = 0.7f;
				if (Monster_Move.DelayTime < 0.7f) {
					if (Monster_Move.DelayTime < Monster_Move.Duration / 2) {
						if (!is_Player_Hit && Collision::Collide(GetPlayer->get_Rect(), attack_region)) {// collide with player when rise
							GetPlayer->set_Just_Hit(300);
							if (player_Location[0] < attack_region.Location[0]) { GetPlayer->set_Knockback(-KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							if (player_Location[0] >= attack_region.Location[0]) { GetPlayer->set_Knockback(KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							is_Player_Hit = true;
						}
					}
					break;
				}
				set_Neck_Stage();
				is_Rise_Sound = false;
				is_Player_Hit = false;
				break;
			case 1://stand
				if (Monster_Move.DelayTime < 2.0f) {
					Monster_Move.Content = "VellumLongNeckStandNew";
					Monster_Move.Length = { 996, 776 };
					Monster_Move.Duration = 1;
					break;
				}
				set_Neck_Stage();
				break;
			case 2://fireball start
				if (Monster_Move.is_flipped) { Monster_Move.Location = Vellum_Warning.Location + vector<2>(START_CORRECTION, START_CORRECTION_Y); }
				if (!Monster_Move.is_flipped) { Monster_Move.Location = Vellum_Warning.Location + vector<2>(-START_CORRECTION, START_CORRECTION_Y); }
				
				Monster_Move.Length = { 1665, 875 };
				Monster_Move.Content = "VellumFireballStart1";
				Monster_Move.Duration = 0.4f;
				Set_Fireball_Sound();

				if (Monster_Move.DelayTime < 0.4f) break;
				set_Neck_Stage();
				break;
			case 3://start2
				Monster_Move.Content = "VellumFireballStart2";
				
				if (Monster_Move.DelayTime < 0.4f) break;
				set_Neck_Stage();
				break;
			case 4://short1
				Monster_Move.Length = { 1220, 868 };
				if (Monster_Move.is_flipped) { 
					Monster_Move.Location = Vellum_Warning.Location + vector<2>(SHORT_CORRECTION, SHORT_CORRECTION_Y); 
					attack_region.Location[0] = Vellum_Warning.Location[0] + 3 * SHORT_CORRECTION / 2;
				}
				if (!Monster_Move.is_flipped) { 
					Monster_Move.Location = Vellum_Warning.Location + vector<2>(-SHORT_CORRECTION, SHORT_CORRECTION_Y); 
					attack_region.Location[0] = Vellum_Warning.Location[0] - 3 * SHORT_CORRECTION / 2;
				}
				
				Monster_Move.Content = "VellumFireballShort1";
				Monster_Move.Duration = FIREBALL_TIME;

				if (Monster_Move.DelayTime < FIREBALL_TIME) {
					if (Monster_Move.DelayTime < 2 * (FIREBALL_TIME / 3) && Monster_Move.DelayTime > (FIREBALL_TIME / 3)) {
						if (!is_Player_Hit && Collision::Collide(GetPlayer->get_Rect(), attack_region)) {// collide with player when rise
							GetPlayer->set_Just_Hit(1000);
							if (player_Location[0] < attack_region.Location[0]) { GetPlayer->set_Knockback(-KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							if (player_Location[0] >= attack_region.Location[0]) { GetPlayer->set_Knockback(KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							is_Player_Hit = true;
						}
					}
					break;
				}
				set_Neck_Stage();
				is_Player_Hit = false;
				break;
			case 5://short2
				if (Monster_Move.is_flipped) { Monster_Move.Location = Vellum_Warning.Location + vector<2>(SHORT_CORRECTION, SHORT_CORRECTION_Y); }
				if (!Monster_Move.is_flipped) { Monster_Move.Location = Vellum_Warning.Location + vector<2>(-SHORT_CORRECTION, SHORT_CORRECTION_Y); }

				Monster_Move.Content = "VellumFireballShort2";
				if (Monster_Move.DelayTime < FIREBALL_TIME) break; 
				set_Neck_Stage();
				break;
			case 6://sink
				Monster_Move.Length = { 996, 776 };
				Monster_Move.Location = Vellum_Warning.Location;
				Monster_Move.Content = "VellumLongNeckSink";
				Monster_Move.Duration = 1;
				Set_Sink_Sound();

				if (Monster_Move.DelayTime < 1.0f) break;
				set_Vellum_Move();
				is_Fireball_Sound = false;
				is_Sink_Sound = false;
				is_LongRise_Sound = false;
				break;
			}
			break;
		case E_VSKILLSTATE::V_DIVE:
			Monster_Move.Length = { 1696, 720 };
			Vellum_Warning.Length = { 3984, 776 };
			Vellum_Warning.Location = vector<2>(Monster_Move.Location[0], 40);
			attack_region.Location[0] = Vellum_Warning.Location[0] + 160;
			attack_region.Length = { 1200, 400 };
			switch (neck_stage)
			{
			case 0://rise
				if (Vellum_Warning.DelayTime < 2.0f) {
					Vellum_Warning.Draw();
					Monster_Move.Playback = 0;
					Monster_Move.DelayTime = 0;
					break;
				}
				start_motion = true;
				Set_Rise_Sound();
				
				Monster_Move.Content = "VellumDive1";
				Monster_Move.Duration = 1.0f;
				if (Monster_Move.DelayTime < 1.0f) {
					if (!is_Player_Hit && Collision::Collide(GetPlayer->get_Rect(), attack_region)) {// collide with player when rise
						GetPlayer->set_Just_Hit(700);
						if (!Monster_Move.is_flipped) { GetPlayer->set_Knockback(-KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
						if (Monster_Move.is_flipped) { GetPlayer->set_Knockback(KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
						is_Player_Hit = true;
					}
					break;
				}
				set_Neck_Stage();
				is_Player_Hit = false;
				break;
			case 1://sink
				if (Monster_Move.DelayTime < 1.0f) {
					Monster_Move.Content = "VellumDive2";
					Monster_Move.Duration = 1;
					break;
				}
				// change to move motion
				set_Vellum_Move();
				is_Sink_Sound = false;
				is_Rise_Sound = false; 
				break;
			}
			break;
		case E_VSKILLSTATE::V_LONGNECK:

			tale_Time -= Time::Get::Delta();//tale pattern
			if (tale_Time < 0) {
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(0, 2);
				int tale_interval = dis(gen);
				tale_Time = TALE_TIME + tale_interval_Arr[tale_interval];
				auto tale = Tale_Pooling->GetRecycledObject<VellumTale>();
				tale->Set_Location(player_Location[0]);
			}

			Monster_Move.Length = { 996, 776 };
			Vellum_Warning.Length = { 996, 776 };
			Vellum_Warning.Location = vector<2>(Monster_Move.Location[0], 40);

			switch (neck_stage)
			{
			case 0://rise
				if (Vellum_Warning.DelayTime < 2.0f) {
					Vellum_Warning.Draw();
					Monster_Move.DelayTime = 0;
					Monster_Move.Playback = 0;
					break;
				}

				start_motion = true;
				Set_LongNeck_Sound();

				Monster_Move.Content = "VellumLongNeckRise";
				Monster_Move.Duration = 0.7f;
				
				if (Monster_Move.DelayTime < 0.7f) {
					if (Monster_Move.DelayTime < Monster_Move.Duration / 2) {
						if (!is_Player_Hit && Collision::Collide(GetPlayer->get_Rect(), attack_region)) {// collide with player when rise
							GetPlayer->set_Just_Hit(300);
							if (player_Location[0] < attack_region.Location[0])  { GetPlayer->set_Knockback(-KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							if (player_Location[0] >= attack_region.Location[0]) { GetPlayer->set_Knockback(KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							is_Player_Hit = true;
						}
					}
					break;
				}
				set_Neck_Stage();
				is_Player_Hit = false;
				break;
			case 1://stand
				if (Monster_Move.DelayTime < 2.0f) {
					Monster_Move.Content = "VellumLongNeckStandNew";
					Monster_Move.Duration = 1;
					break;
				}
				set_Neck_Stage();
				break;
			case 2://sink
				Set_Sink_Sound();
				if (Monster_Move.DelayTime < 1.0f) {
					Monster_Move.Content = "VellumLongNeckSink";
					Monster_Move.Duration = 1;
					break;
				}
				// change to move motion
				set_Vellum_Move();
				is_LongRise_Sound = false;
				is_Sink_Sound = false;
				break;
			}

			break;
		case E_VSKILLSTATE::V_SHORTNECK:

			tale_Time -= Time::Get::Delta();//tale pattern
			if (tale_Time < 0) {
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(0, 2);
				int tale_interval = dis(gen);
				tale_Time = TALE_TIME + tale_interval_Arr[tale_interval];
				auto tale = Tale_Pooling->GetRecycledObject<VellumTale>();
				tale->Set_Location(player_Location[0]);
			}

			Monster_Move.Length = { 996, 720 };
			Vellum_Warning.Length = { 996, 776 };
			Vellum_Warning.Location = vector<2>(Monster_Move.Location[0], 40);

			switch (neck_stage)
			{
			case 0://rise
				if (Vellum_Warning.DelayTime < 2.0f) {
					Vellum_Warning.Draw();
					Monster_Move.Playback = 0;
					Monster_Move.DelayTime = 0;
					break;
				}
				start_motion = true;
				Set_Rise_Sound();

				Monster_Move.Content = "VellumShortNeckRise";
				Monster_Move.Duration = 0.75f;

				if (Monster_Move.DelayTime < 0.75f) {
					if (Monster_Move.DelayTime < Monster_Move.Duration / 2) {
						if (!is_Player_Hit && Collision::Collide(GetPlayer->get_Rect(), attack_region)) {// collide with player when rise
							GetPlayer->set_Just_Hit(300);
							if (player_Location[0] < attack_region.Location[0]) { GetPlayer->set_Knockback(-KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							if (player_Location[0] >= attack_region.Location[0]) { GetPlayer->set_Knockback(KNOCKBACK_HORIZONTAL, KNOCKBACK_VERTICAL); }
							is_Player_Hit = true;
						}
					}
					break;
				}
				set_Neck_Stage();
				is_Player_Hit = false;
				break;
			case 1://stand
				if (Monster_Move.DelayTime < 2.0f) {
					Monster_Move.Content = "VellumShortNeckStand";
					Monster_Move.Duration = 1;
					break;
				}
				set_Neck_Stage();
				break;
			case 2://sink
				Set_Sink_Sound();
				if (Monster_Move.DelayTime < 0.8f) {
					Monster_Move.Content = "VellumShortNeckSink";
					Monster_Move.Duration = 0.8f;
					break;
				}
				// change to move motion
				set_Vellum_Move();
				is_Sink_Sound = false;
				is_Rise_Sound = false;
				break;
			}
			break;
		}
		break;
	}

	////// DRAWING PHASE ////// 

	Monster_Move.Location[1] -= 1;
	if (start_motion) { Monster_Move.Draw(); }
	for (auto elem : Stone_Pooling->vec_PoolObjects)		 { static_cast<VellumStone*>(elem)->Update(); }
	for (auto elem : Tale_Pooling->vec_PoolObjects)			 { static_cast<VellumTale*>(elem)->Update(); }
	for (auto damage : show_Damage_Pooling->vec_PoolObjects) { damage->Update(); }
	return true;
}

void Vellum::End()
{
	delete show_Damage_Pooling;
	delete Stone_Pooling;
	delete Tale_Pooling;
}

void Vellum::select_Pattern()
{
	switch (pattern_num % 5)
	{
	case 0:
		e_VSkillState = E_VSKILLSTATE::V_FIREBALL;
		break;
	case 1:
		e_VSkillState = E_VSKILLSTATE::V_LONGNECK;
		break;
	case 2:
		e_VSkillState = E_VSKILLSTATE::V_SHORTNECK;
		break;
	case 3:
		e_VSkillState = E_VSKILLSTATE::V_DIVE;
		break;
	case 4:
		e_VSkillState = E_VSKILLSTATE::V_SHORTNECK;
		break;
	}
	pattern_num++;
}

void Vellum::set_Vellum_Move()
{
	e_VState = E_VSTATE::V_MOVE;
	Monster_Move.Content = "VellumMove";
	Monster_Move.Length = { 412, 206 };
	Monster_Move.Duration = 2;
	Monster_Move.DelayTime = 0;
	Monster_Move.Playback = 0;
	Monster_Move.Location[1] = -300.0f;
	Monster_Move.Duration = 2;

	Vellum_Warning.DelayTime = 0;

	start_motion = false;

	neck_stage = 0;
}

void Vellum::set_Hit(int damage_taken, int hit_num, int change_rate)
{
	if (e_VState == E_VSTATE::V_MOVE || !start_motion) {
		return;
	}
	int rate = damage_taken / change_rate;
	std::mt19937 gen(rd());
	is_Hp_Bar = true;
	for (int i = 0; i < hit_num; ++i) {
		std::uniform_int_distribution<int> dis(damage_taken - rate, damage_taken + rate);
		auto damage = show_Damage_Pooling->GetRecycledObject<Damage_Effect>();
		int new_damage = dis(gen);
		damage->set_Location(Monster_Move.Location + vector<2>(0, i * 26), new_damage, i, false);
		current_Hp -= new_damage;
	}
}

bool Vellum::is_Dead() const
{
	if (e_VState == E_VSTATE::V_DIE) { return true; }
	else { return false; }
}

void Vellum::set_Neck_Stage()
{
	Monster_Move.DelayTime = 0;
	Monster_Move.Playback = 0;
	neck_stage++;
}

void Vellum::Set_Rise_Sound()	  { if (!is_Rise_Sound)		rise.Play();		  is_Rise_Sound = true; }
void Vellum::Set_Sink_Sound()	  { if (!is_Sink_Sound)		sink.Play();		  is_Sink_Sound = true; }
void Vellum::Set_LongNeck_Sound() { if (!is_LongRise_Sound) longneck_Rise.Play(); is_LongRise_Sound = true; }
void Vellum::Set_Fireball_Sound() { if (!is_Fireball_Sound) fireball.Play();	  is_Fireball_Sound = true; }