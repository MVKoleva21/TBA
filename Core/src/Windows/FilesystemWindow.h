#pragma once
#include "pch.h"

namespace Core {
	class FilesystemWindow
	{
	public:
		static std::string OpenFile(const char* filter);
	};
}
