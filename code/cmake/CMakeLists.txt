# CMake最低版本要求
cmake_minimum_required(VERSION 3.0)

# 添加版本号
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

# 获取当前文件夹名
STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# 项目名称
project(${CURRENT_FOLDER} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# 处理版本文件
if(MSVC)
# PROJECT_SOURCE_DIR为包含PROJECT()的最近一个CMakeLists.txt文件所在的文件夹
    set(VERSIONINFO_RC "${PROJECT_SOURCE_DIR}/RC.rc")
    # 处理RC.rc.in中的${PROJECT_VERSION_MAJOR}、${PROJECT_VERSION_MINOR}、${PROJECT_VERSION_PATCH}成${VERSION_MAJOR}、${VERSION_MINOR}、${VERSION_PATCH}
    configure_file("${PROJECT_SOURCE_DIR}/RC.rc.in"
                   "${VERSIONINFO_RC}")
endif()

# 使用C++11
set(CMAKE_CXX_STANDARD 11)

# 添加函数检查功能
include(CheckFunctionExists)   
check_function_exists(printf HAVEPRINTF)
if(HAVEPRINTF)
  # 添加宏定义
  add_definitions(-DHAVEPRINTF)
endif()

# 共享变量(传递变量到子模块)
if(BUILDMD)
set(RUN_TIME "MD" CACHE INTERNAL "RUN_TIME")
else()
set(RUN_TIME "MT" CACHE INTERNAL "RUN_TIME")
endif()

# 目标平台判断
if(WIN32)
if(CMAKE_CL_64)
endif()
# 分别设置了Debug版本和Release版本可执行文件的输出目录(经测试,windows环境有效)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
# 分别设置了Debug版本和Release版本库文件的输出目录(经测试,windows环境有效)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
# 生成pdb
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/MP /${RUN_TIME} /Zi /GL")
set(CMAKE_CXX_FLAGS_DEBUG "/MP /${RUN_TIME}d /Zi /Od /Ob0")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
# 设置生成pdb路径
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
elseif(ANDROID)

# 设置可执行文件的输出目录(经测试,linux环境有效)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# 设置库文件的输出目录(经测试,linux环境有效)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,--strip-debug")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} -Wl,--strip-debug")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-debug")
#静态库使用
#set(CMAKE_SYSROOT "${TOOLSCHAIN_PATH}/sysroot")
#二进制执行模块使用
set(CMAKE_SYSROOT "$ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fstack-protector -fpic -frtti")
set(CMAKE_CXX_FLAGS_DEBUG "-g2 -gdwarf-2 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-gline-tables-only -O3 -ffunction-sections -fdata-sections")
#arm二进制执行模块使用
link_directories("$ENV{ANDROID_NDK_HOME}/platforms/android-22/arch-arm64/usr/lib")
link_directories("$ENV{ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++/libs/arm-v8a")
link_directories("$ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/aarch64-linux-android/22")
endif(ANDROID)

# 字符串变量判断
if(TEST STREQUAL "test")
endif()

# 自动添加当前源码目录和生成目录到包含目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

# 指定第三方库路径
link_directories("thirdparty")

# 查找当前目录下所有源文件并保存到变量
aux_source_directory(. SRCS)

# 搜索源码文件保存到变量
FILE(GLOB SRCS ./*.h ./*.c ./*.cpp)

# 指定生成可执行文件
add_executable(${PROJECT_NAME} ${SRCS})

# 指定生成静态库
add_library(${PROJECT_NAME} 
	STATIC
	${SRCS}
)

# 指定生成动态库
add_library(${PROJECT_NAME} 
	SHARED
	${SRCS}
)

# 添加链接库
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
target_link_libraries(${PROJECT_NAME} optimized "thirdparty" debug "thirdparty_d")

# 设置调试版本文件后缀名
set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX _d)

# 添加编译完成后脚本
if(WIN32)
add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND xcopy /S /Y /I test.h ..\\
	COMMAND xcopy /S /Y /I ${PROJECT_NAME}* ..\\
)
elseif(ANDROID)
add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND xcopy /S /Y /I test.h ..\\ && xcopy /S /Y /I ${PROJECT_NAME}* ..\\
)
endif(WIN32)

# 指定安装路径
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)
install(DIRECTORY "${PROJECT_BINARY_DIR}/lib/" DESTINATION lib)

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
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
include(CPack)
