add_library(MibletsEngine
  Core/Engine.cpp
  Platform/GLFWWindow.cpp
  RHI/Vulkan/VulkanContext.cpp
  Renderer/Renderer.cpp
  UI/NoesisSystem.cpp
  ECS/Components.h
  ECS/Registry.h
)

find_program(GLSLC_COMPILER glslc)
if(GLSLC_COMPILER)
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/triangle.vert.spv
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
        COMMAND ${GLSLC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/RHI/Vulkan/Shaders/triangle.vert -o ${CMAKE_BINARY_DIR}/shaders/triangle.vert.spv
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/RHI/Vulkan/Shaders/triangle.vert
    )
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/triangle.frag.spv
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
        COMMAND ${GLSLC_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/RHI/Vulkan/Shaders/triangle.frag -o ${CMAKE_BINARY_DIR}/shaders/triangle.frag.spv
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/RHI/Vulkan/Shaders/triangle.frag
    )
    add_custom_target(CompileShaders ALL DEPENDS
        ${CMAKE_BINARY_DIR}/shaders/triangle.vert.spv
        ${CMAKE_BINARY_DIR}/shaders/triangle.frag.spv
    )
    add_dependencies(MibletsEngine CompileShaders)
else()
    message(WARNING "glslc not found. Cannot compile shaders.")
endif()

target_include_directories(MibletsEngine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Noesis Integration Settings
set(NOESIS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/NoesisGUI")
if(EXISTS "${NOESIS_DIR}/Include")
    message(STATUS "NoesisGUI SDK found! Compiling UI Module.")
    target_compile_definitions(MibletsEngine PUBLIC USING_NOESIS)
    target_include_directories(MibletsEngine PUBLIC ${NOESIS_DIR}/Include)
    
    # Platform specific link paths
    target_link_directories(MibletsEngine PUBLIC ${NOESIS_DIR}/Lib/windows_x86_64)
    target_link_libraries(MibletsEngine PUBLIC Noesis NoesisApp)
else()
    message(WARNING "NoesisGUI SDK (C++) NOT found at ThirdParty/NoesisGUI. \
    Please download from noesisengine.com and extract to compile real UI components.")
endif()

target_link_libraries(MibletsEngine PUBLIC
  glfw
  glm
  Vulkan::Vulkan
)
