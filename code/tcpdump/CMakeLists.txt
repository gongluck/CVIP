cmake_minimum_required(VERSION 3.15)

# 获取当前文件夹名
STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

project(${CURRENT_FOLDER})

# 包含路径
include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})

if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin/windows/debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin/windows/release)
  if(CMAKE_CL_64)
  else()
  endif()
elseif(ANDROID)
  #set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin/android/debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin/android/release)
elseif(UNIX)
  set(COMPILEPARAMS "-O0 -Wall -Wno-int-to-pointer-cast")
  set(CMAKE_C_FLAGS_DEBUG "${COMPILEPARAMS} -g")
  set(CMAKE_C_FLAGS_RELEASE "${COMPILEPARAMS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${COMPILEPARAMS} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${COMPILEPARAMS}")
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin/linux)
endif()

# 添加源码文件到项目
FILE(GLOB header ./*.h)
aux_source_directory(. SRCS)
add_executable(${PROJECT_NAME} ${header} ${SRCS})

if(WIN32)
elseif(ANDROID)
elseif(UNIX)
endif()

#windows
if(WIN32)
  if(CMAKE_CL_64)
    set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _x64_d)
    set_property(TARGET ${PROJECT_NAME} PROPERTY RELEASE_POSTFIX _x64_r)
  else()
    set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _win32_d)
    set_property(TARGET ${PROJECT_NAME} PROPERTY RELEASE_POSTFIX _win32_r)
  endif()
#android
elseif(ANDROID)
  set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _${ANDROID_ABI}_d)
  set_property(TARGET ${PROJECT_NAME} PROPERTY RELEASE_POSTFIX _${ANDROID_ABI}_r)
#linux
elseif(UNIX)
  set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _d)
  set_property(TARGET ${PROJECT_NAME} PROPERTY RELEASE_POSTFIX _r)
endif()
