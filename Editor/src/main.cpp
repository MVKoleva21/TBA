#include <iostream>
#include "Core.h"
#include <imgui.h>

class EditorApplication : public Core::Application
{
public:
	EditorApplication()
	{

	}

	~EditorApplication()
	{
	
	}
};

class EditorLayer : public Core::Layer
{
public:
	void OnUpdate()
	{	
		ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Test Window");

		ImGui::Text("This is a test window");

		ImGui::End();
	}
};

int main()
{
	EditorApplication* app = new EditorApplication;
	std::shared_ptr<EditorLayer> editorLayer = std::shared_ptr<EditorLayer>(new EditorLayer);

	app->Init({ 1270, 720 }, "TBA");
	app->PushLayer(editorLayer);

	app->Run();
}