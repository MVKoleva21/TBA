#include <iostream>
#include "Core.h"

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

int main()
{
	EditorApplication* app = new EditorApplication;
	app->Init({ 1270, 720 }, "TBA");
	app->Run();
}