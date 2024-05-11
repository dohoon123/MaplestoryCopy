#pragma once

namespace Engine
{
	class Game abstract
	{
	public:
		virtual void Start()	abstract;
		virtual bool Update()	abstract;
		virtual void End()		abstract;
	};

	Game* Initialize();
}

