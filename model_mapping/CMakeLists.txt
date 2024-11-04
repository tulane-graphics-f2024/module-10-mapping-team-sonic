cmake_minimum_required(VERSION 2.8)
PROJECT(MODEL_MAPPING)
SET(CMAKE_BUILD_TYPE "Release")

if (!MSVC)
SET(CMAKE_CXX_FLAGS "-Wno-deprecated")
endif()

#Compile and Link GLFW
ADD_SUBDIRECTORY(glfw-3.3.7)
link_libraries(glfw)
include_directories(${glfw_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/glfw-3.3.7/deps")

add_library(glad "${CMAKE_SOURCE_DIR}/glfw-3.3.7/deps/glad/gl.h"
         		  "${CMAKE_SOURCE_DIR}/glfw-3.3.7/deps/glad_gl.c"
				     "${CMAKE_SOURCE_DIR}/glfw-3.3.7/deps/glad_vulkan.c"
				     "${CMAKE_SOURCE_DIR}/glfw-3.3.7/deps/tinycthread.c")	 
					  
link_libraries(glad)

SET(MY_SOURCE_PATH ${CMAKE_SOURCE_DIR})
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/source/utils/SourcePath.cpp.in ${CMAKE_SOURCE_DIR}/source/utils/SourcePath.cpp)	

include_directories(${CMAKE_SOURCE_DIR}/source
          ${CMAKE_SOURCE_DIR}/source/utils
					${CMAKE_SOURCE_DIR}/shaders)
add_executable(model_mapping WIN32 MACOSX_BUNDLE 
	source/model_mapping.cpp 
	source/utils/CubeMap.cpp
	source/utils/CubeMap.h
	source/utils/common.h
	source/utils/CheckError.h
	source/utils/mat.h
	source/utils/ObjMesh.cpp
	source/utils/ObjMesh.h
	source/utils/SourcePath.cpp
	source/utils/SourcePath.h
	source/utils/Trackball.cpp
	source/utils/Trackball.h
	source/utils/u8names.cpp
	source/utils/u8names.h
	source/utils/vec.h
  source/utils/lodepng.cpp
  source/utils/lodepng.h
	shaders/fshader.glsl
    shaders/vshader.glsl
	 shaders/skybox_fshader.glsl
	 shaders/skybox_vshader.glsl)

#Windows cleanup
if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(model_mapping PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

#Apple cleanup
if (APPLE)
    set_target_properties(model_mapping PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "simple")
	
    set_target_properties(model_mapping PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING ${GLFW_VERSION}
                          MACOSX_BUNDLE_LONG_VERSION_STRING ${GLFW_VERSION_FULL}
    					  MACOSX_BUNDLE_ICON_FILE glfw.icns)				  
endif()
