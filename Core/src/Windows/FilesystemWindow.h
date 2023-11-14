#pragma once
#include "pch.h"

namespace Core {
	/// @brief A window to open and save files
	class FilesystemWindow
	{
	public:
		/// @brief Open a file
		/// @param filter The filter to use
		/// @return The path to the file
		static std::string OpenFile(const char* filter);

		/// @brief Save a file
		/// @param filter The filter to use
		/// @return The path to the file
		static std::string SaveFile(const char* filter);
	};
}
