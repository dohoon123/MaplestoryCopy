#include "stdafx.h"
#include "CameraManager.h"
void CameraManager::Init(Rendering::Image::Component background, Rendering::Animation::Component* player, Rendering::Camera* camera)
{
	Background	= background;
	Player		= player;
	Camera		= camera;
}

void CameraManager::DeInit()
{
	Player = nullptr;
	Camera = nullptr;
}

vector<2> CameraManager::get_Camera_Location() const
{
	return Camera->Location;
}

void CameraManager::CameraRange()
{
	if (Player->Angle != 0.0f and Background.Angle != 0.0f) return;

	vector<2> const min[2]
	{
		Player->Location - Player->Length / 2,
		Background.Location - Background.Length / 2
	};

	vector<2> const max[2]
	{
		Player->Location + Player->Length / 2,
		Background.Location + Background.Length / 2
	};
	if (min[0][0] <= min[1][0]) Player->Location[0] += 2.0f;
	if (min[0][1] <= min[1][1]) Player->Location[1] += 2.0f;
	if (max[0][0] >= max[1][0]) Player->Location[0] -= 2.0f;
	if (max[0][1] >= max[1][1]) Player->Location[1] -= 2.0f;

	float cx = Player->Location[0] - Camera->Location[0];
	float cy = Player->Location[1] - Camera->Location[1];

	// (플레이어 좌표와 카메라 좌표의 간격) * 시간 * 보정값 만큼 보간하기
	Camera->Location[0] += (cx * Time::Get::Delta() * 10);
	Camera->Location[1] += (cy * Time::Get::Delta() * 10);


	float sl = length(vector<2>(cx, cy));
	//for (int i = 4; i > 1; --i) {
	//	if (sl > i) {// 보간 거리 최소, 최대값 설정
	//		// (플레이어 좌표와 카메라 좌표의 간격) * 시간 * 거리에 따른 보정값 만큼 보간하기
	//		Camera->Location[0] += (cx * Time::Get::Delta() * i);
	//		Camera->Location[1] += (cy * Time::Get::Delta() * i);
	//		break;
	//	}
	//}

	if (sl < 1) { Camera->Location = Player->Location; }

	if (Camera->Location[0] <= min[1][0] + 640.0f) Camera->Location[0] = min[1][0] + 640.0f;
	if (Camera->Location[1] <= min[1][1] + 360.0f) Camera->Location[1] = min[1][1] + 360.0f;
	if (Camera->Location[0] >= max[1][0] - 640.0f) Camera->Location[0] = max[1][0] - 640.0f;
	if (Camera->Location[1] >= max[1][1] - 360.0f) Camera->Location[1] = max[1][1] - 360.0f;
}
