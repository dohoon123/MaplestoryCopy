#pragma once

template <typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static T* Instance = new T;
		return Instance;
	}

	void DeleteSingleton()
	{
		delete GetInstance();
	}
};