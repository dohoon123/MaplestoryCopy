#pragma once
#include "Engine/Game.h"
#include "Manager/UI.h"
#include "Manager/Player.h"

class Game final : public Engine::Game
{
public:
	void Start()	override;
	bool Update()	override;
	void End()		override;

public:
	bool Scene_Changed = false;
	Rendering::Camera* Camera;
	Rendering::Camera Camera_GUI;
	UI Player_UI;
	std::string prev_Scene = "";
};

