# CMakeList.txt : CMake project for MyOpenGLEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MyOpenGLEngine")

include_directories(includes)
include_directories(libs)
include_directories(core)
include_directories(includes/imgui)

#Add IMGUI headers
set( IMGUI_HEADERS
    includes/imgui/imgui.h
    includes/imgui/imconfig.h
    includes/imgui/imgui_impl_opengl3.h
    includes/imgui/imgui_impl_glfw.h
    includes/imgui/imgui_impl_opengl3_loader.h
)

#Add IMGUI sources
set( IMGUI_SOURCE
    includes/imgui/imgui.cpp
    includes/imgui/imgui_demo.cpp
    includes/imgui/imgui_draw.cpp
    includes/imgui/imgui_widgets.cpp
    includes/imgui/imgui_impl_opengl3.cpp
    includes/imgui/imgui_impl_glfw.cpp
    includes/imgui/imgui_tables.cpp
)


# HeaderFiles
set (HEADER_FILES
 "core/Application.h" "core/Test.h" "core/Framebuffer.h" "core/ShaderProgram.h" "core/Mesh.h" "core/Vertex.h" "core/Camera.h" "core/Transform.h" "core/Window.h" "core/Scene.h" "core/MeshGenerator.h" "core/LineMesh.h" "core/Material.h" "core/Ball.h" "core/Collision/Colliders.h" "core/Collision/OcTree.h" "core/Collision/CollisionManager.h" "core/Timer.h" "core/Collision/OcTreeNode.h" "core/Actor.h" "ActorComponent.h"  "core/ECS/Entity.h" "core/ECS/Components/TransformComponent.h" "core/ECS/Components/VelocityComponent.h" "core/ECS/Systems/SystemBase.h" "core/ECS/Systems/MovementSystem.h" "core/ECS/Components/MeshComponent.h" "core/MeshBase.h" "core/ECS/Systems/RenderSystem.h" "core/ECS/Systems/AICombatSystem.h" "core/ECS/Components/CombatComponent.h" "core/ECS/Components/InputComponent.h"  "core/PunktSky/PunktSky.h" "core/BSpline/BSplineSurface.h" "core/Landscape/Landscape.h" "core/Landscape/Chunk.h")
 
 # SourceFiles
set (SRC_FILES
includes/glad.c "core/application.cpp"   "core/Test.cpp" "core/Framebuffer.cpp" "core/ShaderProgram.cpp" "core/Mesh.cpp" "core/Camera.cpp" "core/Transform.cpp" "core/Window.cpp" "core/Scene.cpp" "core/MeshGenerator.cpp" "core/LineMesh.cpp" "core/Material.cpp" "core/Ball.cpp" "core/Collision/OcTree.cpp" "core/Collision/CollisionManager.cpp" "core/Collision/Colliders.cpp"  "core/ECS/ECSManager.cpp" "core/ECS/Entity.cpp" "core/PunktSky/PunktSky.cpp" "core/BSpline/BSplineSurface.cpp" "core/Landscape/Landscape.cpp")








add_subdirectory(libs/glfw-3.3.9)

# Add source to this project's executable.
add_executable (MyOpenGLEngine ${IMGUI_HEADERS} ${IMGUI_SOURCE} ${HEADER_FILES} ${SRC_FILES} "MyOpenGLEngine.cpp" "MyOpenGLEngine.h")

target_link_libraries(MyOpenGLEngine glfw)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MyOpenGLEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
