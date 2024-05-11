#include "stdafx.h"
#include "SceneManager.h"
#include "Scene/BGScene.h"
#include "Creature/Stump.h"
#include "Creature/Stompy.h"
#include "Creature/DeformedStompy.h"
#include "Creature/Vellum.h"

SceneManager::SceneManager()
{
	Scenes.try_emplace("SELLAS", new BGScene);
	Scenes.try_emplace("SELLASFIELD", new BGScene);
	Scenes.try_emplace("SINKINGTHINGS", new BGScene);
	Scenes.try_emplace("SINKINGTHINGSFIELD", new BGScene);
	Scenes.try_emplace("WHALEBELLY", new BGScene);
	Scenes.try_emplace("WHALEBELLYFIELD", new BGScene);
	Scenes.try_emplace("ABYSSCAVE", new BGScene);//last Scene!

	Now = "ABYSSCAVE";
	int i = -5;

	Sound::Sound bgm;
	bgm.Content = "WhereStarsRest";
	bgm.volume = 0.1f;
	static_cast<BGScene*>(Scenes.at("SELLAS"))->set_BGM(bgm);//set bgm before Scene()->Start();

	bgm.Content = "WhereStarsRest";
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->set_BGM(bgm);

	bgm.Content = "SinkingThings";
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->set_BGM(bgm);

	bgm.Content = "SinkingThings";
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->set_BGM(bgm);

	bgm.Content = "WhaleBelly";
	static_cast<BGScene*>(Scenes.at("WHALEBELLY"))->set_BGM(bgm);

	bgm.Content = "WhaleBelly";
	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->set_BGM(bgm);

	bgm.Content = "AbyssCave";
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->set_BGM(bgm);
	Scenes.at(Now)->Start();

	std::vector<Monster*>::iterator iter;

	//////////   0 : SELLAS      //////////

	Rendering::Image::Component BG;
	BG.Content = "WhereStarsRest";
	BG.Length = { 2100, 1080 };

	static_cast<BGScene*>(Scenes.at("SELLAS"))->Set_BG(BG);

	///////// TILE OBJECT /////////
	Collision::RectAngle Tile1;

	Tile1.Length = { 346, 10 };
	Tile1.Location = { 245, -250 };
	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Tile(Tile1);

	Tile1.Length = { 1550, 10 };
	Tile1.Location = { -25, -384 };
	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Tile(Tile1);

	Tile1.Length = { 346, 10 };
	Tile1.Location = { 485, 48 };
	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Tile(Tile1);

	///////// ROPE OBJECT /////////
	Collision::RectAngle Rope1;
	Rope1.Length = { 16, 424 };
	Rope1.Location = { 480, -122 };

	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Rope(Rope1);

	///////// PORTAL OBJECT /////////
	Rendering::Animation::Component portal;
	portal.Content = "Portal";
	portal.Length = { 89, 170 };
	portal.Location = { 640, -300 };
	portal.Repeatable = true;
	portal.Duration = 1;
	portal.is_flipped = false;

	///////// INVISIBLE WALL OBJECT /////////
	Tile1.Length = { 10, 900 };
	Tile1.Location = { 800, -100 };
	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Player_Wall(Tile1);
	
	Tile1.Length = { 10, 900 };
	Tile1.Location = { -850, -100 };
	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Player_Wall(Tile1);

	///////// BGM SETTING /////////
	static_cast<BGScene*>(Scenes.at("SELLAS"))->Add_Portal("SELLASFIELD", portal);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SELLASFIELD

	BG.Content = "WhereStarsRestField1";
	BG.Length = { 2276, 1125 };

	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Set_BG(BG);

	///////// TILE OBJECT /////////
	Tile1.Length = { 740, 10 };
	Tile1.Location = { -445, -83 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 554, 10 };
	Tile1.Location = { 375, -83 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 2276, 10 };//bottom
	Tile1.Location = { 0, -372 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 966, 10 };
	Tile1.Location = { 372, 200 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 566, 10 };
	Tile1.Location = { -507, 200 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Tile(Tile1);

	///////// ROPE OBJECT /////////
	Rope1.Length = { 16, 245 };
	Rope1.Location = { -535, -165 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Rope(Rope1);

	Rope1.Length = { 16, 270 };
	Rope1.Location = { 480, 105 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Rope(Rope1);

	///////// PORTAL OBJECT /////////
	portal.Length = { 89, 170 };
	portal.Location = { 740, -288 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Portal("SINKINGTHINGS", portal);

	portal.Length = { 89, 170 };
	portal.Location = { -740, -288 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Portal("SELLAS", portal);

	///////// INVISIBLE WALL OBJECT /////////
	Tile1.Length = { 10, 100 };
	Tile1.Location = { -40, -10 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Wall(Tile1);

	Tile1.Length = { 10, 100 };
	Tile1.Location = { -850, -10 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Wall(Tile1);

	Tile1.Length = { 10, 100 };
	Tile1.Location = { 50, -10 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Wall(Tile1);

	Tile1.Length = { 60, 100 };
	Tile1.Location = { -170, 270 };
	static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Add_Monster_Wall(Tile1);
	///////// MONSTER SETTING /////////

	for (int i = 0; i < 10; i++) {
		static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stump));
	}

	i = -5;
	for (iter = static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Monsters.begin(); iter != static_cast<BGScene*>(Scenes.at("SELLASFIELD"))->Monsters.end(); ++iter) {
		(*iter)->Start();
		(*iter)->set_Start_Location(i);
		i++;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//1 : SINKING THINGS   

	BG.Content = "SinkingThings";
	BG.Length = { 1789, 821 };

	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Set_BG(BG);

	///////// TILE OBJECT /////////
	Tile1.Length = { 1370, 10 };
	Tile1.Location = { 65, -280 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Tile(Tile1);

	Tile1.Length = { 330, 10 };
	Tile1.Location = { 300, 193 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Tile(Tile1);

	Tile1.Length = { 366, 10 };
	Tile1.Location = { 312, 10 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Tile(Tile1);

	///////// ROPE OBJECT /////////
	Rope1.Length = { 16, 176 };
	Rope1.Location = { 299, 145 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Rope(Rope1);

	Rope1.Length = { 16, 270 };
	Rope1.Location = { 437, -85 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Rope(Rope1);

	///////// PORTAL OBJECT /////////
	portal.Location = { -440, -200 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Portal("SELLASFIELD", portal);

	portal.Location = { 640, -200 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGS"))->Add_Portal("SINKINGTHINGSFIELD", portal);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2 : SINKINGTHINGSFIELD

	BG.Content = "SinkingThingsField1";
	BG.Length = { 1974, 1121 };

	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Set_BG(BG);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	///////// TILE OBJECT /////////
	Tile1.Length = { 1974, 10 };
	Tile1.Location = { 0, -380 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 515, 10 };
	Tile1.Location = { -520, -100 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 648, 10 };
	Tile1.Location = { 144, -100 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 160, 10 };
	Tile1.Location = { 694, -100 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 670, 10 };
	Tile1.Location = { 76, 180 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 350, 10 };
	Tile1.Location = { 694, 180 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	Tile1.Length = { 160, 10 };
	Tile1.Location = { -446, 200 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Tile(Tile1);

	///////// ROPE OBJECT /////////
	Rope1.Length = { 16, 248 };
	Rope1.Location = { -412, -185 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Rope(Rope1);

	Rope1.Length = { 16, 248 };
	Rope1.Location = { 328, 95 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Rope(Rope1);


	///////// INVISIBLE WALL OBJECT /////////
	Tile1.Length = { 60, 100 };
	Tile1.Location = { -320, 270 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Wall(Tile1);

	Tile1.Length = { 30, 100 };
	Tile1.Location = { -225, -10 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Wall(Tile1);

	Tile1.Length = { 60, 100 };
	Tile1.Location = { 515, -10 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Wall(Tile1);

	Tile1.Length = { 50, 100 };
	Tile1.Location = { 460, 270 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Monster_Wall(Tile1);

	///////// PORTAL OBJECT /////////
	portal.Location = { -540, -300 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Portal("SINKINGTHINGS", portal);

	portal.Location = { 640, -300 };
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Add_Portal("WHALEBELLY", portal);


	///////// MONSTER OBJECT /////////
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stump));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stump));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stump));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stump));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stompy));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stompy));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stompy));
	static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new Stompy));

	i = -5;
	for (iter = static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.begin(); iter != static_cast<BGScene*>(Scenes.at("SINKINGTHINGSFIELD"))->Monsters.end(); ++iter) {
		(*iter)->Start();
		(*iter)->set_Start_Location(i);
		i++;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	//2 : WHALEBELLY
	BG.Content = "WhaleBelly";
	BG.Length = { 1570, 1165 };

	static_cast<BGScene*>(Scenes.at("WHALEBELLY"))->Set_BG(BG);

	///////// TILE OBJECT /////////

	Tile1.Length = { 1570, 10 };
	Tile1.Location = { 0, -384 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLY"))->Add_Tile(Tile1);

	///////// PORTAL OBJECT /////////
	portal.Location = { -440, -300 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLY"))->Add_Portal("SINKINGTHINGSFIELD", portal);

	portal.Location = { 440, -300 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLY"))->Add_Portal("WHALEBELLYFIELD", portal);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2 : WHALEBELLYFIELD
	BG.Content = "WhaleBellyField";
	BG.Length = { 1570, 1165 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Set_BG(BG);

	///////// TILE OBJECT /////////
	Tile1.Length = { 1570, 10 };
	Tile1.Location = { 0, -384 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Add_Tile(Tile1);
	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Add_Monster_Tile(Tile1);

	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Monsters.push_back(dynamic_cast<Monster*>(new DeformedStompy));

	for (iter = static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Monsters.begin(); iter != static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Monsters.end(); ++iter) {
		(*iter)->Start();
	}

	///////// PORTAL OBJECT /////////
	portal.Location = { 640, -300 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Add_Portal("ABYSSCAVE", portal);

	portal.Location = { -640, -300 };
	static_cast<BGScene*>(Scenes.at("WHALEBELLYFIELD"))->Add_Portal("WHALEBELLY", portal);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2 : ABYSSCAVE

	BG.Content = "AbyssCave";
	BG.Length = { 3055, 910 };
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Set_BG(BG);

	Tile1.Length = { 4000, 10 };
	Tile1.Location = { 0, -300 };
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Add_Tile(Tile1);

	Tile1.Length = { 4000, 10 };
	Tile1.Location = { 0, -350 };
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Add_Monster_Tile(Tile1);

	///////// MONSTER OBJECT /////////
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Monsters.push_back(dynamic_cast<Monster*>(new Vellum));
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->is_Boss_Stage = true;
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->death_Count = 5;
	for (iter = static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Monsters.begin(); iter != static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Monsters.end(); ++iter) {
		(*iter)->Start();
	}

	///////// PORTAL OBJECT /////////
	portal.Location = { -1340, -220 };
	static_cast<BGScene*>(Scenes.at("ABYSSCAVE"))->Add_Portal("WHALEBELLYFIELD", portal);
}

SceneManager::~SceneManager(){ for (auto scene : Scenes) delete scene.second; }

void SceneManager::ChangeScene(std::string SceneName)
{
	Scenes.at(Now)->End();
	Scenes.at(SceneName)->Start();
	Now = SceneName;
}

std::string SceneManager::get_Now() const
{
	return Now;
}
