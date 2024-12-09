// MyOpenGLEngine.cpp : Defines the entry point for the application.
//

#include "MyOpenGLEngine.h"
//#include "test.h"
#include "Application.h"
//#include "test.h"
#include "tempTest.h"

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
	//glm::vec3 Velocity = tempTest::GetNormalAfterCollision({ 2,1,0 }, { -0.8, -0.6, 0 });
	//tempTest::GetNormalballToball();
	//std::cout << Velocity.x << " " << Velocity.y << " " << Velocity.z;

	//return 0;

	Application::get().Run();

	return 0;
}
