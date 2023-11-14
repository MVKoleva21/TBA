#include "Core.h"
#include "EditorLayer.h"

#include <imgui.h>

namespace Editor {
	/// @brief The editor application
	///
	/// @details This is the main application for the editor
	/// @see Core::Application
	class EditorApplication : public Core::Application
	{
	public:
		EditorApplication()
		{
			PushLayer(std::shared_ptr<EditorLayer>(new EditorLayer));
		}

		~EditorApplication()
		{

		}
	};
}

int main()
{
	Editor::EditorApplication* app = new Editor::EditorApplication;
	app->Init({ 1270, 720 }, "TBA");
	app->Run();
}