#pragma once
#include "Util/IObjectPoolable.h"

class VellumTale final : public IObjectPoolable
{
public:
	Rendering::Animation::Component Vellum_Warning;
	Rendering::Animation::Component Vellum_Tale1;
	Rendering::Animation::Component Vellum_Tale2;
	Collision::RectAngle collide_region;

	Sound::Sound rise;
	Sound::Sound sink;

	Sound::Sound player_hit_sound;
public:
	void Set_Location(float x_loc);
	void Set_Rise_Sound();
	void Set_Sink_Sound();
	void Update();
public:
	bool is_Using = false;
private:
	bool is_Rise_Sound = false;
	bool is_Sink_Sound = false;
	bool is_Hit = false;
};

