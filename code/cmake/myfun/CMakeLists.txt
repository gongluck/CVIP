# CMake最低版本要求
cmake_minimum_required(VERSION 3.0)

# 获取当前文件夹名
STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# 项目名称
project(${CURRENT_FOLDER})

# 查找当前目录下所有源文件并保存到变量
aux_source_directory(. SRCS)

# 生成链接库
add_library(${PROJECT_NAME} SHARED ${SRCS})

# 指定库的安装路径
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${PROJECT_NAME}.h DESTINATION include)