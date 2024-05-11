#pragma once

namespace Sound
{
	class Sound final
	{
	public:
		void Play();
		void Stop();
		void Pause();

	private:
		bool  pause  = false;
	
	public:
		char const* Content = nullptr;

	public:
		float volume = 0.0f;
		bool  loop   = false;
	};

	void EndPlay();
}