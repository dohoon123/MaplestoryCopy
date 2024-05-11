#include <Windows.h>

#define _USE_MATH_DEFINES

#include <map>
#include <string>

#include "FreeImage.h"
#include "Pipeline.h"
#include "Rendering.h"

#include "Time.h"
#include "Resource.h"
#include <math.h>

namespace Rendering 
{
	namespace 
	{
		inline matrix<4, 4> Scale(vector<2> const length)
		{
			return matrix<4, 4>
			{
				length[0],	0,			0, 0,
				0,			length[1],	0, 0,
				0,			0,			1, 0,
				0,			0,			0, 1
			};
		}
		inline matrix<4, 4> Rotation(float angle)
		{
			float const radian = static_cast<float>(M_PI) / 180.0f * angle;

			return matrix<4, 4>
			{
				cos(radian), -sin(radian), 0, 0,
				sin(radian),  cos(radian), 0, 0,
				0,			  0,		   1, 0,
				0,			  0,		   0, 1
			};
		}
		inline matrix<4, 4> Translation(vector<2> const location)
		{
			return matrix<4, 4>
			{
				1, 0, 0, location[0],
				0, 1, 0, location[1],
				0, 0, 1,		   0,
				0, 0, 0,		   1
			};
		}
	}

	void Camera::Set() const
	{
		using namespace Pipeline;

		matrix<4, 4> const projection = Scale(vector<2>(2.0f / Length[0], 2.0f / Length[1]));
		matrix<4, 4> const view = Rotation(-Angle) * Translation(-Location);
		matrix<4, 4> const latter = projection * view;

		Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::matrix const&>(latter));
	}

	namespace Text
	{
		void Import(std::string const& file)
		{
			AddFontResourceEx(file.data(), FR_PRIVATE | FR_NOT_ENUM, nullptr);
		}
		void Component::Draw()
		{
			LOGFONT descriptor = LOGFONT();

			descriptor.lfHeight = Font.Size;
			descriptor.lfWeight = Font.Bold ? FW_BOLD : FW_NORMAL;
			descriptor.lfItalic = Font.Italic;
			descriptor.lfUnderline = Font.Underlined;
			descriptor.lfStrikeOut = Font.StrikeThrough;
			descriptor.lfCharSet = DEFAULT_CHARSET;

			strcpy_s(descriptor.lfFaceName, LF_FACESIZE, Font.Name);// 이부분 다시

			HFONT const font = CreateFontIndirect(&descriptor);

			SIZE const area = { static_cast<LONG>(Length[0]), static_cast<LONG>(Length[1]) };
			POINT const center = { static_cast<LONG>(Location[0]), static_cast<LONG>(Location[1]) };

			Pipeline::String::Render(font, str, RGB(Color.Red, Color.Green, Color.Blue), area, center);
		
			DeleteObject(font);
		}
	}

	namespace Image
	{
		struct Descriptor final
		{
		public:
			Pipeline::Texture::Handle* Handle = nullptr;
		public:
			SIZE Size = SIZE();
		};

		std::map<std::string, Descriptor> Storage;

		void Import(std::string const& file) {
			{
				FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(file.data()), file.data());
				//Image loading using FreeImage library

				FreeImage_FlipVertical(bitmap);
				// create new 32bit image if bitmap is not 32bit image, delete previous one.

				if (FreeImage_GetBPP(bitmap) != 32) {
					FIBITMAP* const previous = bitmap;
					bitmap = FreeImage_ConvertTo32Bits(bitmap);
					FreeImage_Unload(previous);
				}

				Image::Descriptor descriptor = Image::Descriptor();
				{
					descriptor.Size.cx = FreeImage_GetWidth(bitmap);
					descriptor.Size.cy = FreeImage_GetHeight(bitmap);
					//get bitmap x, y size
					Pipeline::Texture::Create(descriptor.Handle, descriptor.Size, FreeImage_GetBits(bitmap));
					//create image using descriptor and bitmap info
				}
				{
					size_t const x = file.find_first_of('/') + sizeof(char);
					size_t const y = file.find_last_of('.');
					//get Key using image file name(not including directory name and .png)
					Image::Storage.try_emplace(file.substr(x, y - x), descriptor);
					//save to Storage using Key & value pair
				}
				FreeImage_Unload(bitmap);
			}
		}

		void Component::Draw()
		{
			using namespace Pipeline;
			{
				matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);
				Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));
			}
			{
				Descriptor const& image = Storage.at(Content);

				Texture::Render(image.Handle, { 0 , 0 , image.Size.cx, image.Size.cy }, is_flipped);
			}
		}
	}

	namespace Animation
	{
		struct Descriptor final
		{
		public:
			Pipeline::Texture::Handle* Handle = nullptr;
		public:
			UINT Motion = UINT();
			SIZE Frame = SIZE();
		};

		std::map<std::string, Descriptor> Storage;

		void Import(std::string const& file) {
			{
				FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(file.data()), file.data());
				//Image loading using FreeImage library
				FreeImage_FlipVertical(bitmap);
				// create new 32bit image if bitmap is not 32bit image, delete previous one.
				if (FreeImage_GetBPP(bitmap) != 32) {
					FIBITMAP* const previous = bitmap;
					bitmap = FreeImage_ConvertTo32Bits(bitmap);
					FreeImage_Unload(previous);
				}

				Animation::Descriptor descriptor = Animation::Descriptor();
				{
					descriptor.Frame.cx = FreeImage_GetWidth(bitmap);
					descriptor.Frame.cy = FreeImage_GetHeight(bitmap);

					Pipeline::Texture::Create(descriptor.Handle, descriptor.Frame, FreeImage_GetBits(bitmap));
					//create image using descriptor and bitmap info
				}
				{
					size_t const x = file.find_first_of('/') + sizeof(char);
					size_t const y = file.find_last_of('[');
					size_t const z = file.find_last_of(']');

					descriptor.Motion = atoi(file.substr(y + sizeof(char), z - (y + sizeof(char))).data());
					descriptor.Frame.cx /= descriptor.Motion;

					//get Key using image file name(not including directory name and .png)
					Animation::Storage.try_emplace(file.substr(x, y - x), descriptor);

					//save to Storage using Key & value pair
				}
				FreeImage_Unload(bitmap);
			}
		}

		void Component::Draw()
		{
			using namespace Pipeline;
			{
				matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);
				Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::matrix const&>(world));
			}
			{
				Descriptor const& descriptor = Storage.at(Content);

				LONG const progress = static_cast<LONG>((Playback / Duration) * descriptor.Motion);

				RECT const area{
					descriptor.Frame.cx * (progress + 0), (descriptor.Frame.cy * 0),
					descriptor.Frame.cx * (progress + 1), (descriptor.Frame.cy * 1)
				};

				Texture::Render(descriptor.Handle, area, is_flipped);

				float const delta = Time::Get::Delta();

				Playback += delta;

				if (Duration <= Playback) {
					if (Repeatable) Playback  = fmod(Playback, Duration);
					else			Playback -= delta;
				}
			}
		}
	}

	namespace Pipeline 
	{ void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter); }
	
	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
		case WM_CREATE:
		{
			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

			Resource::Import("Font", Text::Import);

			FreeImage_Initialise();
			{
				Resource::Import("Image", Image::Import);
				Resource::Import("Animation", Animation::Import);
			}
			
			FreeImage_DeInitialise();

			return ;
		}
		case WM_APP:
		{

			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);
			return  ;
		}
  
		case WM_DESTROY:
		{

			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

			for (auto const& pair : Image::Storage) {
				Pipeline::Texture::Delete(pair.second.Handle);
			}

			for (auto const& pair : Animation::Storage) {
				Pipeline::Texture::Delete(pair.second.Handle);
			}

			return  ;
		}
		case WM_SIZE:
		{
			Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);
			return  ;
		}
 
		}
	}

}

