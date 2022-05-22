cmake_minimum_required(VERSION 3.0.0)
project(learn-opengl VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

# Thirdparty
set(THIRDPARTY_DIR thirdparty)

# GLFW
set(GLFW_NAME glfw)
set(GLFW_DIR ${THIRDPARTY_DIR}/${GLFW_NAME})
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
set(GLFW_DEPS_INCLUDE_DIR ${GLFW_DIR}/deps)
set(GLFW_GLAD_GL ${GLFW_DEPS_INCLUDE_DIR}/glad/gl.h ${GLFW_DEPS_INCLUDE_DIR}/glad_gl.c)

add_subdirectory(${GLFW_DIR})

add_executable(learn-opengl WIN32 src/main.c ${GLFW_GLAD_GL})

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${GLFW_DEPS_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_NAME})

# Main
if (MSVC)
    # Tell MSVC to use main instead of WinMain
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif (CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    # Tell Clang using MS CRT to use main instead of WinMain
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,/entry:mainCRTStartup")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
