#pragma once
#include <string>

namespace Resource
{
	void Import(std::string const directory, void (* const function)(std::string const& file));
}

