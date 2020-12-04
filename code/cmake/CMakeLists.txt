# CMake最低版本要求
cmake_minimum_required(VERSION 3.0)

# 添加版本号
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

# 获取当前文件夹名
STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# 项目名称
project(${CURRENT_FOLDER})

# 添加函数检查功能
include(CheckFunctionExists)   
check_function_exists(printf HAVEPRINTF)
if(HAVEPRINTF)
  # 添加宏定义
  add_definitions(-DHAVEPRINTF)
endif()

# 自动添加当前源码目录和生成目录到包含目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 设置可执行文件的输出目录(经测试,linux环境有效)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)       

# 设置库文件的输出目录(经测试,linux环境有效)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)           

# 分别设置了Debug版本和Release版本可执行文件的输出目录(经测试,windows环境有效)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin) 

# 分别设置了Debug版本和Release版本库文件的输出目录(经测试,windows环境有效)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)

# 加入一个配置头文件用于处理CMake对源码的设置
configure_file(
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
  )

# 自定义编译选项
option(USESUBMODULE "use submodule" ON)
if (USESUBMODULE)
    # 设置变量
    set(SUBMODULE myfun)
    
    # 添加包含路径
    include_directories(${SUBMODULE})
 
    # 添加子目录
    # 必须放在aux_source_directory前,否则同名变量SRCS会冲突
    add_subdirectory(${SUBMODULE})

    # 设置附加库变量
    set(EXTRA_LIBS ${EXTRA_LIBS} ${SUBMODULE})
endif (USESUBMODULE)

# 查找当前目录下所有源文件并保存到变量
aux_source_directory(. SRCS)

# 指定生成目标
add_executable(${PROJECT_NAME} ${SRCS})

# 添加链接库
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

# 指定安装路径
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)

# 定义一个宏，用来简化测试工作
macro(do_test mycommand myret)
add_test(NAME test_${mycommand}_${myret} COMMAND ${mycommand} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# 检查测试输出是否包含"${myret}"
set_tests_properties(test_${mycommand}_${myret} PROPERTIES PASS_REGULAR_EXPRESSION ${myret})
endmacro(do_test)

# 启用测试
enable_testing()

# 测试程序
do_test(mydemo "cmake")

# 构建一个CPack安装包
include(InstallRequiredSystemLibraries)
# 设置安装包版本号
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
include(CPack)
