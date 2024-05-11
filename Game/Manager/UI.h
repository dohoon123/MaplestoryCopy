#pragma once

const float BULLETLENGTH = 18.4f;

class UI final
{
public:
	Rendering::Image::Component Player_Status;
	Rendering::Image::Component HP;
	Rendering::Image::Component MP;
	Rendering::Image::Component Bullet;

	///////////// REVOLVE UI ////////////
	Rendering::Image::Component BlasterUIBG1;
	Rendering::Image::Component BlasterUIBG2;
	Rendering::Animation::Component BlasterUIBG3;
	Rendering::Image::Component BlasterUIBG4;

	/////////// DEATCOUNT UI ///////////
	Rendering::Animation::Component Deathcount_Board;
	Rendering::Image::Component Deathcount_Single_Digit;
	/////////////////////////////////////

	float max_Hp;
	float max_Mp;
	float bar_length = 145;
	int bullet_num = 0;
	int light_num = 0;

	bool is_Boss_Stage = false;
	bool is_boss_hp_bar = false;
	int death_Count = 0;
public:
	void Init(float max_hp, float max_mp);
	void Update(float hp, float mp);
private:

	Rendering::Image::Component boss_hp_bar;
	Rendering::Image::Component boss_hp_red_bar;

	Rendering::Image::Component bullet_capacity;
	Rendering::Image::Component charge_num;
	std::vector<Rendering::Image::Component> BlasterBullet;
	std::vector<Rendering::Image::Component> LightStack;
	void Set_Deathcount_Single_Digit();
};

