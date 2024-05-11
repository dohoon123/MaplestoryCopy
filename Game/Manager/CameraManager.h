#pragma once
#include "Manager/Singleton.h"

#define GetCameraManager CameraManager::GetInstance()

class CameraManager final : public Singleton<CameraManager>
{
private:
	Rendering::Image::Component			Background;
	Rendering::Animation::Component*	Player;
	Rendering::Camera*					Camera;
public:
	void Init(
		Rendering::Image::Component background,
		Rendering::Animation::Component* player,
		Rendering::Camera* camera
	);

	void DeInit();
	vector<2> get_Camera_Location() const;
	void CameraRange();
};

