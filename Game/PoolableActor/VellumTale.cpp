#include "stdafx.h"
#include "VellumTale.h"
#include "Manager/Player.h"

void VellumTale::Set_Location(float x_loc)
{
	Vellum_Warning.Content = "VellumWarning";
	Vellum_Warning.Length = { 996, 776 };
	Vellum_Warning.Location = { x_loc, 40 };
	Vellum_Warning.Repeatable = true;
	Vellum_Warning.Duration = 0.6f;

	Vellum_Tale1.Content = "VellumTale1";
	Vellum_Tale1.Length = { 923, 717 };
	Vellum_Tale1.Location = { x_loc + 50, 5 };
	Vellum_Tale1.Repeatable = true;
	Vellum_Tale1.Duration = 0.8f;
	Vellum_Tale1.is_flipped = false;

	Vellum_Tale2.Content = "VellumTale2";
	Vellum_Tale2.Length = { 923, 717 };
	Vellum_Tale2.Location = { x_loc + 50, 5 };
	Vellum_Tale2.Repeatable = true;
	Vellum_Tale2.Duration = 0.8f;
	Vellum_Tale2.is_flipped = false;

	// SOUND //
	rise.Content = "VellumTaleRise";
	rise.volume = 1.0f;
	sink.Content = "VellumTaleSink";
	sink.volume = 1.0f;

	player_hit_sound.Content = "HitSound";
	player_hit_sound.volume = 1.0f;

	collide_region.Length = { 280, 300 };
	collide_region.Location = vector<2>(x_loc + 30, -160);
	is_Using = true;
}

void VellumTale::Update()
{
	IObjectPoolable::Update();// check this object is recyclable
	if (is_Using) {
		if (Vellum_Warning.DelayTime < 2.0f) {
			Vellum_Warning.DelayTime += Time::Get::Delta();
			Vellum_Warning.Draw();
			return;
		}
		if (Vellum_Tale1.DelayTime < Vellum_Tale1.Duration) {
			Vellum_Tale1.DelayTime += Time::Get::Delta();
			Vellum_Tale1.Draw();

			if (!is_Hit) { 
				if (Vellum_Tale1.DelayTime < Vellum_Tale1.Duration / 2) {

					if (Collision::Collide(GetPlayer->get_Rect(), collide_region) && !GetPlayer->is_Dead()) {
						GetPlayer->set_Just_Hit(2000);
						player_hit_sound.Play();
						is_Hit = true;
					}
				}
			}

			return;
			Set_Rise_Sound();
			return;
		}
		if (Vellum_Tale2.DelayTime < Vellum_Tale2.Duration) {
			Vellum_Tale2.DelayTime += Time::Get::Delta();
			Vellum_Tale2.Draw();
			Set_Sink_Sound();
			return;
		}

		is_Using = false;
		Vellum_Warning.DelayTime = 0;
		Vellum_Warning.Playback = 0;
		Vellum_Tale1.DelayTime = 0;
		Vellum_Tale1.Playback = 0;
		Vellum_Tale2.DelayTime = 0;
		Vellum_Tale2.Playback = 0;
		is_Rise_Sound = false;
		is_Sink_Sound = false;
		is_Hit = false;
		SetRecycle();
	}
}

void VellumTale::Set_Rise_Sound() { if (!is_Rise_Sound) rise.Play(); is_Rise_Sound = true; }
void VellumTale::Set_Sink_Sound() { if (!is_Sink_Sound) sink.Play(); is_Sink_Sound = true; }