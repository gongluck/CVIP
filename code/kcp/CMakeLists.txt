cmake_minimum_required(VERSION 3.15)

# 获取当前文件夹名
STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

##############################################################################################
project(${CURRENT_FOLDER}_server)

# 分别设置了Debug版本和Release版本可执行文件的输出目录(经测试,windows环境有效)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release) 

# 分别设置了Debug版本和Release版本库文件的输出目录(经测试,windows环境有效)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release)

# 添加源码文件到项目
#aux_source_directory(. SRCS)
# 搜索源码文件保存到变量
FILE(GLOB SRCS ./*.h ./*.c ./test_server.cpp)
add_executable(${PROJECT_NAME} ${SRCS})

# 使用DEBUG_POSTFIX指定输出文件结尾对executable target(可执行文件)无效
#set (CMAKE_DEBUG_POSTFIX d)
#set_target_properties (${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _d)

##############################################################################################
project(${CURRENT_FOLDER}_client)

# 分别设置了Debug版本和Release版本可执行文件的输出目录(经测试,windows环境有效)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release) 

# 分别设置了Debug版本和Release版本库文件的输出目录(经测试,windows环境有效)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release)

# 添加源码文件到项目
#aux_source_directory(. SRCS)
# 搜索源码文件保存到变量
FILE(GLOB SRCS ./*.h ./*.c ./test_client.cpp)
add_executable(${PROJECT_NAME} ${SRCS})

# 使用DEBUG_POSTFIX指定输出文件结尾对executable target(可执行文件)无效
#set (CMAKE_DEBUG_POSTFIX d)
#set_target_properties (${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _d)