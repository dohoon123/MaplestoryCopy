#include "stdafx.h"
#include "BossBullet.h"

void BossBullet::Set_Location(vector<2> monster_location, vector<2> location)
{
	is_Using = true;
	bullet.Content = "Bullet";
	bullet.Length = { 32, 32 };
	bullet.Location = monster_location + location;
}

void BossBullet::Update()
{
	IObjectPoolable::Update();

	if (is_Using) {
		bullet.Draw();

		if (shoot) {
			bullet.Location += normalize(destination - start_Location) * Time::Get::Delta() * move_Speed;
		}

		if (abs(bullet.Location[0]) > 1000 || abs(bullet.Location[1]) > 1000 || !is_Using) {
			SetRecycle();
			is_Using = false;
			shoot = false;
		}
	}
}

void BossBullet::Set_Destination(vector<2> dest)
{
	destination = dest;
	start_Location = bullet.Location;
	shoot = true;
}
