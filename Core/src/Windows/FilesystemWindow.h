#pragma once
#include "pch.h"

namespace Core {
	class FilesystemWindow
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
