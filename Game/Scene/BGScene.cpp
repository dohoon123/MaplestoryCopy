#include "stdafx.h"
#include "BGScene.h"

void BGScene::Start()
{
	Land.Content = "Point";
	
	Land.Length = { 50, 100 };
	Land.Location = { 460, 270 };
	BGM.loop = true;
	BGM.Play();
}

bool BGScene::Update()
{
	BG.Draw();
	//Land.Draw();

	std::vector<Collision::RectAngle>::const_iterator collide_iter;
	std::vector<Monster*>::iterator monster_iter;

	float foot = GetPlayer->get_Bottom();

	for (collide_iter = Ropes.begin(); collide_iter != Ropes.end(); ++collide_iter) {

		if (Collision::Collide(GetPlayer->get_Player_Location(), Collision::RectAngle{ collide_iter->Length, 0, collide_iter->Location }))//Collide of point and rectangle
		{
			GetPlayer->set_Rope_Available();
			GetPlayer->set_Rope_X(round(collide_iter->Location[0]));
			break;
		}
	}
	if (collide_iter == Ropes.end()) {
		GetPlayer->set_Rope_Disable();
	}

	////////////////////// TILE COLLISION CONTROL ////////////////////////

	for (collide_iter = Tiles.begin(); collide_iter != Tiles.end(); ++collide_iter) {
		if (Collision::Collide(GetPlayer->get_Rect(), Collision::RectAngle{ collide_iter->Length, 0, collide_iter->Location }))//Collide
		{
			if (foot >= collide_iter->Location[1] && GetPlayer->is_Falling() && GetPlayer->get_Ignore_Collision_Time() <= 0) {//Collide of rectangle and rectangle
				GetPlayer->set_Landing();
				if (GetPlayer->is_Hanging()) {
					GetPlayer->set_Player_YLocation(collide_iter->Location[1] + round(collide_iter->Length[1] / 2));
				}
				break;
			}
		}
	}

	if (collide_iter == Tiles.end()) {
		GetPlayer->set_Gravity();
	}

	GetPlayer->set_Down_Jump_Available(false);

	for (collide_iter = Tiles.begin(); collide_iter != Tiles.end(); ++collide_iter) {//down jump check

		Collision::Point p_loc = GetPlayer->get_Player_Location();
		float tile_left = collide_iter->Location[0] - collide_iter->Length[0] / 2;
		float tile_right = collide_iter->Location[0] + collide_iter->Length[0] / 2;

		if (foot - 20 >= collide_iter->Location[1] && p_loc.Location[0] >= tile_left && p_loc.Location[0] <= tile_right) {
			GetPlayer->set_Down_Jump_Available(true);
		}
	}
	////////////////////// PLAYER WALL COLLISION CONTROL ////////////////////////

	for (collide_iter = Player_Walls.begin(); collide_iter != Player_Walls.end(); ++collide_iter) {
		float wall_min = collide_iter->Location[0] - (collide_iter->Length[0] / 2);
		float wall_max = collide_iter->Location[0] + (collide_iter->Length[0] / 2);
		//Player
		if (Collision::Collide(GetPlayer->get_Rect(), Collision::RectAngle{ collide_iter->Length, 0, collide_iter->Location })) {//Collide
			float p_min = GetPlayer->get_Rect().Location[0] - (GetPlayer->get_Rect().Length[0] / 2);
			float p_max = GetPlayer->get_Rect().Location[0] + (GetPlayer->get_Rect().Length[0] / 2);

			if (p_min < wall_min) { GetPlayer->set_Player_Move_Horizontal(-1.0f); }
			if (p_max > wall_max) { GetPlayer->set_Player_Move_Horizontal(1.0f); }
		}
	}

	////////////////////// TILE & MONSTER COLLISION CONTROL ////////////////////////

	for (collide_iter = Monster_Tiles.begin(); collide_iter != Monster_Tiles.end(); ++collide_iter) {
		for (monster_iter = Monsters.begin(); monster_iter != Monsters.end(); ++monster_iter) {
			if ((*monster_iter)->is_Land()) {// land monster
				if (Collision::Collide((*monster_iter)->get_Rect(), Collision::RectAngle{ collide_iter->Length, 0, collide_iter->Location })) {
					(*monster_iter)->set_Landing();
					(*monster_iter)->set_Monster_YLocation(collide_iter->Location[1] + (collide_iter->Length[1] / 2) + ((*monster_iter)->get_Rect().Length[1] / 2));
				}
				else {
					(*monster_iter)->set_Falling();
				}
			}
		}
	}

	////////////////////// WALL & MONSTER COLLISION CONTROL ////////////////////////

	for (collide_iter = Monster_Walls.begin(); collide_iter != Monster_Walls.end(); ++collide_iter) {
		float wall_min = collide_iter->Location[0] - (collide_iter->Length[0] / 2);
		float wall_max = collide_iter->Location[0] + (collide_iter->Length[0] / 2);
		
		for (monster_iter = Monsters.begin(); monster_iter != Monsters.end(); ++monster_iter) {
			if (Collision::Collide((*monster_iter)->get_Rect(), Collision::RectAngle{ collide_iter->Length, 0, collide_iter->Location })) {
				if (!(*monster_iter)->is_Dead()) {
					float m_min = (*monster_iter)->get_Rect().Location[0] - ((*monster_iter)->get_Rect().Length[0] / 2);
					float m_max = (*monster_iter)->get_Rect().Location[0] + ((*monster_iter)->get_Rect().Length[0] / 2);

					if (m_min < wall_min) { (*monster_iter)->set_Monster_Move_Horizontal(-1.0f); }
					if (m_max > wall_max) { (*monster_iter)->set_Monster_Move_Horizontal(1.0f); }
				}
			}
		}
	}

	for (monster_iter = Monsters.begin(); monster_iter != Monsters.end(); ++monster_iter) {
		if (!(*monster_iter)->is_Dead()) {// alive
			break;
		}
	}

	std::map<std::string, Rendering::Animation::Component>::iterator iter;
	if (monster_iter == Monsters.end()) { // all dead
		for (iter = Portals.begin(); iter != Portals.end(); ++iter) {
			iter->second.Draw();
		}
		is_Portal_Draw = true;
	}

	return true;
}

void BGScene::End() { BGM.Stop(); }

void BGScene::Set_BG(Rendering::Image::Component BackGround)	{ BG = BackGround; }
void BGScene::set_BGM(Sound::Sound bgm)							{ BGM = bgm; }
void BGScene::Add_Rope(Collision::RectAngle Rope)				{ Ropes.push_back(Rope); }
void BGScene::Add_Tile(Collision::RectAngle Tile)				{ Tiles.push_back(Tile); }
void BGScene::Add_Monster_Tile(Collision::RectAngle Tile)		{ Monster_Tiles.push_back(Tile); }
void BGScene::Add_Monster_Wall(Collision::RectAngle Wall)		{ Monster_Walls.push_back(Wall); }
void BGScene::Add_Player_Wall(Collision::RectAngle Wall)		{ Player_Walls.push_back(Wall); }
void BGScene::Add_Portal(std::string Destination, Rendering::Animation::Component Portal){ Portals.try_emplace(Destination, Portal); }

const std::vector<Collision::RectAngle>& BGScene::get_Tiles()			{ return Tiles; }
const std::vector<Collision::RectAngle>& BGScene::get_Monster_Walls()	{ return Monster_Walls; }
const std::vector<Collision::RectAngle>& BGScene::get_Player_Walls()	{ return Player_Walls; }
const std::vector<Collision::RectAngle>& BGScene::get_Ropes()			{ return Ropes; }
const Rendering::Image::Component& BGScene::get_BG()					{ return BG; }
const std::map<std::string, Rendering::Animation::Component>& BGScene::get_Portals(){ return Portals; }