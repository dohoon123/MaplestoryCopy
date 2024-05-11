#pragma once
namespace Input 
{
	namespace Get
	{
		namespace Key
		{
			[[nodiscard]]bool Down(size_t const code);
			[[nodiscard]]bool Press(size_t const code);
			[[nodiscard]]bool Up(size_t const code);
		}

		namespace Cursor
		{
			[[nodiscard]] long X();
			[[nodiscard]] long Y();
		}

		namespace Wheel
		{
			[[nodiscard]] long H();
			[[nodiscard]] long V();
		}
	}
}