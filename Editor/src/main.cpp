#include "Core.h"
#include "EditorLayer.h"

#include <imgui.h>

namespace Editor {
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