// MyOpenGLEngine.cpp : Defines the entry point for the application.
//

#include "MyOpenGLEngine.h"
//#include "test.h"
#include "Application.h"
//#include "test.h"

void IncrementInt(int& i)
{
	i = ++i;
}

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	//Test t;
	//t.MyTest();

	Application::get().Run();

	return 0;
}
