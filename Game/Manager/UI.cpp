#include "stdafx.h"
#include "Scene/BGScene.h"
#include "UI.h"
#define _USE_MATH_DEFINES
#include <math.h>
void UI::Init(float max_hp, float max_mp)
{
	max_Hp = max_hp;
	max_Mp = max_mp;

	Player_Status.Content = "HpMpBar";
	Player_Status.Length = { 174, 70 };
	
	HP.Content = "Hp";
	HP.Length = { bar_length, 13 };

	MP.Content = "Mp";
	MP.Length = { bar_length, 13 };

	Deathcount_Board.Content = "Deathcount";
	Deathcount_Board.Duration = 1;
	Deathcount_Board.Length = { 89, 56 };

	Set_Deathcount_Single_Digit();
	Deathcount_Single_Digit.Length = { 12, 17 };

	// BOSS HP BAR
	boss_hp_bar.Content = "Boss_Hp";
	boss_hp_bar.Length = { 801, 40 };

	boss_hp_red_bar.Content = "Hp";
	boss_hp_red_bar.Length = { 760, 13 };


	// BLASTER UI

	BlasterUIBG1.Content = "BlasterUIBG1";
	BlasterUIBG1.Length = { 106, 121.0f };

	BlasterUIBG2.Content = "BlasterUIBG2";
	BlasterUIBG2.Length = { 90, 58.0f };

	BlasterUIBG3.Content = "BlasterUIBG3";
	BlasterUIBG3.Length = { 126, 126 };
	BlasterUIBG3.Duration = 1;
	BlasterUIBG3.Repeatable = true;
	BlasterUIBG3.is_flipped = false;
	BlasterUIBG3.DelayTime = 0;

	BlasterUIBG4.Content = "BlasterUIBG4";
	BlasterUIBG4.Length = { 58, 56.0f };

	for (int i = 0; i < 6; i++) {
		Rendering::Image::Component bullet;
		bullet.Content = "BlasterBullet";
		bullet.Length = { 13, 13 };

		float const radian = static_cast<float>(M_PI) / -180.0f * (((360 / 6) * i - 60));
		vector<2> location = { cos(radian) * BULLETLENGTH, sin(radian) * BULLETLENGTH };
		bullet.Location = location + vector<2>(-145, -306.5);
		BlasterBullet.push_back(bullet);

		Rendering::Image::Component stack_light;
		stack_light.Content = "ChargeStack";
		stack_light.Length = { 8, 11 };
		stack_light.Location = { -171 + (i * 6), -258 };
		LightStack.push_back(stack_light);
	}

	bullet_capacity.Content = "BulletCapacity";
	bullet_capacity.Length = { 7, 5 };
	bullet_capacity.Location = { -120, -258 };

	charge_num.Length = { 9, 7 };
	charge_num.Location = { -128, -258 };
}

void UI::Update(float hp, float mp)
{
	///////////////// PLAYER LOCATION & LENGTH SETTING PHASE /////////////////
	HP.Location = vector<2>(0, -330 + (HP.Length[1] / 2));
	MP.Location = vector<2>(0, -348 + (MP.Length[1] / 2));
	Player_Status.Location = vector<2>(0, -360 + Player_Status.Length[1] / 2);

	float new_Hp = (hp / max_Hp) * bar_length;
	float new_Location = -((bar_length - new_Hp)) / 2;
	HP.Length = { new_Hp, 13 };
	HP.Location = vector<2>(new_Location + 8 + 0.5f, -330 + HP.Length[1] / 2 - 2 + 0.5f);

	float new_Mp = (mp / max_Mp) * bar_length;
	new_Location = -((bar_length - new_Mp)) / 2;
	MP.Length = { new_Mp, 13 };
	MP.Location = vector<2>(new_Location + 8 + 0.5f, -348 + MP.Length[1] / 2 + 0.5f);

	// BOSS STAGE

	if (is_Boss_Stage) {
		Set_Deathcount_Single_Digit();
		Deathcount_Board.Location = vector<2>(Deathcount_Board.Length[0] - 535 + 0.5f, Deathcount_Board.Length[1] / 2 + 300 + 0.5f);
		Deathcount_Single_Digit.Location = Deathcount_Board.Location + vector<2>(25.5f, -10 + 0.5f);

		Deathcount_Board.Draw();
		Deathcount_Single_Digit.Draw();


		std::vector<Monster*>::iterator monster_iter;

		for (monster_iter = static_cast<BGScene*>(GetSceneManager->GetNowScene())->Monsters.begin(); monster_iter != static_cast<BGScene*>(GetSceneManager->GetNowScene())->Monsters.end(); monster_iter++) {
			if ((*monster_iter)->get_Is_Hp_Bar()) {
				boss_hp_bar.Location = vector<2>(0.5f, 360 - (boss_hp_bar.Length[1] / 2));
				boss_hp_red_bar.Location = vector<2>(18, 358 - (boss_hp_red_bar.Length[1] / 2));

				float new_Hp = ((*monster_iter)->get_Current_Hp() / (*monster_iter)->get_Max_Hp()) * 760;
				boss_hp_red_bar.Length[0] = new_Hp;

				float new_Location = ((760 - new_Hp)) / 2;
				boss_hp_red_bar.Location[0] -= new_Location + 0.5f;
				boss_hp_red_bar.Draw();
				boss_hp_bar.Draw();
			}
		}
	}
	//BLASTER UI

	BlasterUIBG1.Location = vector<2>(-145, -360 + BlasterUIBG1.Length[1] / 2);
	BlasterUIBG1.Location = vector<2>(BlasterUIBG1.Location[0], BlasterUIBG1.Location[1]);
	BlasterUIBG2.Location = BlasterUIBG1.Location + vector<2>(0, 15);
	BlasterUIBG3.Location = BlasterUIBG1.Location + vector<2>(0, -8);
	BlasterUIBG4.Location = BlasterUIBG1.Location + vector<2>(0, -7.5);

	if (GetPlayer->get_Revolver_Stack() == 6) {
		BlasterUIBG3.Content = "BlasterFullCharge";
	}
	else {
		BlasterUIBG3.Content = "BlasterUIBG3";
	}

	// BULLET

	bullet_num	 = GetPlayer->get_Bullet_Num();
	light_num	 = GetPlayer->get_Revolver_Stack();

	///////////////// DRAWING PHASE /////////////////
	HP.Draw();
	MP.Draw();
	Player_Status.Draw();

	BlasterUIBG1.Draw();
	BlasterUIBG2.Draw();
	BlasterUIBG3.Draw();
	BlasterUIBG4.Draw();

	for (int i = 0; i < bullet_num; i++) {
		BlasterBullet[i].Draw();
	}

	for (int i = 0; i < light_num; i++) {
		LightStack[i].Draw();
	}
	if (light_num < 6) {
		bullet_capacity.Draw();
		if (light_num > 0) {
			switch (light_num)
			{
			case 1:
				charge_num.Content = "BlasterUINum1";
				break;
			case 2:
				charge_num.Content = "BlasterUINum2";
				break;
			case 3:
				charge_num.Content = "BlasterUINum3";
				break;
			case 4:
				charge_num.Content = "BlasterUINum4";
				break;
			case 5:
				charge_num.Content = "BlasterUINum5";
				break;
			}
			charge_num.Draw();
		}
	}
}

void UI::Set_Deathcount_Single_Digit()
{
	switch (death_Count)
	{
	case 5:
		Deathcount_Single_Digit.Content = "500_600";
		break;
	case 4:
		Deathcount_Single_Digit.Content = "400_500";
		break;
	case 3:
		Deathcount_Single_Digit.Content = "300_400";
		break;
	case 2:
		Deathcount_Single_Digit.Content = "200_300";
		break;
	case 1:
		Deathcount_Single_Digit.Content = "100_200";
		break;
	case 0:
		Deathcount_Single_Digit.Content = "0_100";
		break;
	}
}
