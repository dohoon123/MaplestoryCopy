#pragma once

#include "Manager/Scene.h"
#include "Engine/Collision.h"
#include "Creature/Monster.h"
#include "Engine/Sound.h"

class BGScene : public Scene
{
public:
	virtual void Start()	override;
	virtual bool Update()	override;
	virtual void End()		override;

public:
	void Set_BG(Rendering::Image::Component BackGround);
	void set_BGM(Sound::Sound bgm);

	void Add_Rope(Collision::RectAngle Rope);
	void Add_Tile(Collision::RectAngle Tile);
	void Add_Monster_Tile(Collision::RectAngle Tile);
	void Add_Monster_Wall(Collision::RectAngle Wall);
	void Add_Player_Wall(Collision::RectAngle Wall);
	void Add_Portal(std::string Destination, Rendering::Animation::Component Portal);

public:
	std::vector<Monster*> Monsters;
	bool is_Portal_Draw = false;
	bool is_Boss_Stage = false;
	int death_Count = 0;
public:
	const std::vector<Collision::RectAngle>& get_Ropes();
	const std::vector<Collision::RectAngle>& get_Tiles();
	const std::vector<Collision::RectAngle>& get_Monster_Walls();
	const std::vector<Collision::RectAngle>& get_Player_Walls();
	const std::map<std::string, Rendering::Animation::Component>& get_Portals();
	const Rendering::Image::Component& get_BG();

private:
	Rendering::Image::Component BG;
	Rendering::Image::Component Land;


	std::vector<Collision::RectAngle> Tiles;
	std::vector<Collision::RectAngle> Monster_Tiles;
	std::vector<Collision::RectAngle> Monster_Walls;//to block monsters escaping
	std::vector<Collision::RectAngle> Player_Walls;
	std::vector<Collision::RectAngle> Ropes;
	std::map<std::string, Rendering::Animation::Component> Portals;
	
	Sound::Sound BGM;
};

