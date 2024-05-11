#include "stdafx.h"
#include "Game.h"
#include "Scene/BGScene.h"
#include "Creature/Monster.h"
#include "Creature/DeformedStompy.h"

void Game::Start()
{
	SceneManager::GetInstance();// created as singleton
	Player::GetInstance();

	Camera = new Rendering::Camera();
	//Camera_GUI = new Rendering::Camera();

	Player_UI.Init(GetPlayer->get_Max_Hp(), GetPlayer->get_Max_Mp());
	GetCameraManager->Init(static_cast<BGScene*>(GetSceneManager->GetNowScene())->get_BG(), GetPlayer->get_Player_Animation(), Camera);

}

bool Game::Update()
{
	std::map<std::string, Rendering::Animation::Component>::const_iterator portal_iter;

	if (Scene_Changed) 
	{
		for (portal_iter = (static_cast<BGScene*>(GetSceneManager->GetNowScene())->get_Portals()).begin(); portal_iter != (static_cast<BGScene*>(GetSceneManager->GetNowScene())->get_Portals()).end(); ++portal_iter)
		{
			if ((*portal_iter).first == prev_Scene) {
				GetPlayer->set_Player_XLocation((*portal_iter).second.Location[0]);
				GetPlayer->set_Player_YLocation((*portal_iter).second.Location[1] + 10);
				break;
			}
			else {
				GetPlayer->set_Player_XLocation(0);
				GetPlayer->set_Player_YLocation(0);
			}
		}
		GetCameraManager->Init(static_cast<BGScene*>(GetSceneManager->GetNowScene())->get_BG(), GetPlayer->get_Player_Animation(), Camera);
		GetPlayer->set_Gravity();
		Scene_Changed = false;
	}

	////////////////////// UPDATE CAMERA ////////////////////////
	Camera->Set();
	GetCameraManager->CameraRange();

	if (GetSceneManager->GetNowScene()->Update()) {
		////////////////////// SCENE CONTROL ////////////////////////

		std::vector<Monster*>::iterator monster_iter;

		for (monster_iter = static_cast<BGScene*>(GetSceneManager->GetNowScene())->Monsters.begin(); monster_iter != static_cast<BGScene*>(GetSceneManager->GetNowScene())->Monsters.end(); ++monster_iter) {
			(*monster_iter)->Update();
		}

		GetPlayer->Update();
		
		////////////////////// ROPE COLLISION CONTROL ////////////////////////

		std::vector<Collision::RectAngle>::const_iterator collide_iter;

		////////////////////// MONSTER & SKILL COLLISION CONTROL ////////////////////////
		// do this in Game.cpp for Skill's Now_Hit check
		std::multimap<std::string, IObjectPoolable*>::const_iterator skill_iter;
		for (skill_iter = GetPlayer->attack_SkillPooling->PoolObjects.begin(); skill_iter != GetPlayer->attack_SkillPooling->PoolObjects.end(); ++skill_iter) {
			if (static_cast<Skill*>(skill_iter->second)->is_Using && !static_cast<Skill*>(skill_iter->second)->is_Hit) {
				for (monster_iter = static_cast<BGScene*>(GetSceneManager->GetNowScene())->Monsters.begin(); monster_iter != static_cast<BGScene*>(GetSceneManager->GetNowScene())->Monsters.end(); ++monster_iter) {
					if (!(*monster_iter)->is_Dead()) {
						if (Collision::Collide((*monster_iter)->collide_region, Collision::RectAngle{ static_cast<Skill*>(skill_iter->second)->Skill_Animation.Length, 0, static_cast<Skill*>(skill_iter->second)->Skill_Animation.Location }))//Collide
						{
							(*monster_iter)->set_Hit(GetPlayer->get_Damage(), static_cast<Skill*>(skill_iter->second)->hit_Num, GetPlayer->get_Change_Rate());
						}
					}
				}
				static_cast<Skill*>(skill_iter->second)->set_Now_Hit();
			}
		}

		////////////////////// PORTAL COLLISION CONTROL ////////////////////////
		// do this in Game.cpp for Scene control
		for (portal_iter = (static_cast<BGScene*>(GetSceneManager->GetNowScene())->get_Portals()).begin(); portal_iter != (static_cast<BGScene*>(GetSceneManager->GetNowScene())->get_Portals()).end(); ++portal_iter)
		{
			if (Collision::Collide(GetPlayer->get_Player_Location(), Collision::RectAngle{ portal_iter->second.Length, 0, portal_iter->second.Location }))//Collide
			{
				if (Input::Get::Key::Down(VK_UP) && static_cast<BGScene*>(GetSceneManager->GetNowScene())->is_Portal_Draw) {
					prev_Scene = GetSceneManager->get_Now();
					SceneManager::GetInstance()->ChangeScene(portal_iter->first);
					Scene_Changed = true;
					break;
				}
			}
		}
		////////////////////// PLAYER REVIVAL ////////////////////////
		if (Input::Get::Key::Down(VK_F5) && GetPlayer->is_Dead()) {//
			GetPlayer->set_Revive();
			static_cast<BGScene*>(GetSceneManager->GetNowScene())->death_Count--;

			if (static_cast<BGScene*>(GetSceneManager->GetNowScene())->death_Count == 0) {
				SceneManager::GetInstance()->ChangeScene("SELLAS");
				Scene_Changed = true;
			}
		}

		////////////////////// UI DRAW ////////////////////////
		//Camera->Set();
		Camera_GUI.Set();
		Player_UI.death_Count = static_cast<BGScene*>(GetSceneManager->GetNowScene())->death_Count;
		static_cast<BGScene*>(GetSceneManager->GetNowScene())->is_Boss_Stage ? Player_UI.is_Boss_Stage = true : Player_UI.is_Boss_Stage = false;
		Player_UI.Update(GetPlayer->get_Hp(), GetPlayer->get_Mp());

		return true; 
	}
	return false;
}

void Game::End()
{ 
	GetSceneManager->DeleteSingleton();
	GetCameraManager->DeleteSingleton();
	GetPlayer->DeleteSingleton();
}

Engine::Game* Engine::Initialize()
{
	return new ::Game();
}
