#pragma once
#include "Manager/Scene.h"
#include "Singleton.h"

#define GetSceneManager SceneManager::GetInstance()

class SceneManager final : public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

public:
	void ChangeScene(std::string SceneName);

public:
	inline Scene* GetNowScene() const
	{
		return Scenes.at(Now);
	}

	std::string get_Now() const;
private:
	std::map<std::string, class Scene*> Scenes;
	std::string Now = "";
};

