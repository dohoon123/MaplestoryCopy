#include "stdafx.h"
#include "Damage_Effect.h"
#include <algorithm>

void Damage_Effect::Update()
{
	IObjectPoolable::Update();
	std::vector<Rendering::Image::Component>::iterator iter;

	if (delay_Time > 0){// not yet
		delay_Time -= Time::Get::Delta();
		return;
	}

	if (display_Time > 0) {
		display_Time -= Time::Get::Delta();
		for (iter = damage_Image.begin(); iter != damage_Image.end(); ++iter) {
			iter->Draw();
			iter->Location[1] += 50.0f * Time::Get::Delta();
		}
	}
	else {
		damage_Image.clear();
		SetRecycle();
	}
}

void Damage_Effect::set_Location(vector<2> location, int damage, int delayTime, bool is_player)
{
	display_Time = 2;
	delay_Time = delayTime * 0.08f;// µå¸£¸¤ Ç¥½Ã

	start_location = location;
	float i = 0;
	while (damage > 0) {
		Rendering::Image::Component image;

		image.Length = { 31 , 33 };
		image.Location = location - vector<2>(i * 26, 0);

		int num = damage % 10;

		if (is_player) {
			switch (num)
			{
			case 0:
				image.Content = "V0";
				break;
			case 1:
				image.Content = "V1";
				break;
			case 2:
				image.Content = "V2";
				break;
			case 3:
				image.Content = "V3";
				break;
			case 4:
				image.Content = "V4";
				break;
			case 5:
				image.Content = "V5";
				break;
			case 6:
				image.Content = "V6";
				break;
			case 7:
				image.Content = "V7";
				break;
			case 8:
				image.Content = "V8";
				break;
			case 9:
				image.Content = "V9";
				break;
			}
		}
		else {
			switch (num)
			{
			case 0:
				image.Content = "0";
				break;
			case 1:
				image.Content = "1";
				break;
			case 2:
				image.Content = "2";
				break;
			case 3:
				image.Content = "3";
				break;
			case 4:
				image.Content = "4";
				break;
			case 5:
				image.Content = "5";
				break;
			case 6:
				image.Content = "6";
				break;
			case 7:
				image.Content = "7";
				break;
			case 8:
				image.Content = "8";
				break;
			case 9:
				image.Content = "9";
				break;
			}
		}
		
		damage /= 10;
		damage_Image.push_back(image);
		i++;
	}
}
