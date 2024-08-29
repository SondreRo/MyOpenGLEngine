// MyOpenGLEngine.cpp : Defines the entry point for the application.
//

#include "MyOpenGLEngine.h"
#include "application.h"
#include "test.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	Application::Hello();

	test t;

	t.MyTest();


	return 0;
}
