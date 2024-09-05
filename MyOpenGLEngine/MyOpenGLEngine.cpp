// MyOpenGLEngine.cpp : Defines the entry point for the application.
//

#include "MyOpenGLEngine.h"
//#include "test.h"
#include "Application.h"
//#include "test.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;


	//Test t;
	//t.MyTest();

	Application::Get()->Run();

	return 0;
}
