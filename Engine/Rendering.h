#pragma once
#include "vector.h"

namespace Rendering
{
	class Camera final {
	public:
		void Set() const;
	public:
		vector<2> Length	= { 1280, 720 };
		float	  Angle		= { 0 };
		vector<2> Location	= { 0, 0 };
	};

	namespace Text
	{
		class Component final
		{
		public:
			void Draw();
		public:
			char const* str = nullptr;

			struct
			{
				char const* Name = nullptr;
				unsigned	Size = 0;
				bool		Bold = false;
				bool		Italic = false;
				bool		Underlined = false;
				bool		StrikeThrough = false;
			}Font;

			struct
			{
				unsigned char Red = 0;
				unsigned char Green = 0;
				unsigned char Blue = 0;
			}Color;
		public:
			vector<2> Length = { 0, 0 };
			vector<2> Location = { 0, 0 };
		};
	}

	namespace Image
	{
		class Component final
		{
		public:
			void Draw();
		public:
			char const* Content = nullptr;//image name
		public:
			vector<2> Length	= { 0, 0 };
			float	  Angle		= { 0 };
			vector<2> Location	= { 0, 0 };
			bool is_flipped = false;
		};
	}
	namespace Animation
	{
		class Component final
		{
		public:
			void Draw();
		public:
			char const* Content = nullptr;//image name

			float	DelayTime = 0;
			float	Playback = 0;
			float	Duration = 0;// playback으로 가기까지의 시간
			bool	Repeatable = false;
		public:
			vector<2> Length = { 0, 0 };
			float	  Angle = { 0 };
			vector<2> Location = { 0, 0 };
			bool is_flipped = false;
		};
	}
}

