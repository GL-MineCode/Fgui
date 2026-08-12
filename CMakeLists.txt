cmake_minimum_required(VERSION 3.16)
project(fast_gui2 LANGUAGES CXX)

# ============================================================
# C++ 标准
# ============================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# 编译选项开关
#   FGUI_BUILD_TEST     : 编译测试程序 test/main.cpp -> main.exe（默认 ON）
#   FGUI_BUILD_EXAMPLES : 编译 example/ 目录下的示例程序（默认 OFF）
# 用法示例：
#   cmake -DFGUI_BUILD_TEST=OFF -DFGUI_BUILD_EXAMPLES=ON ...
# ============================================================
option(FGUI_BUILD_TEST     "编译测试程序 test/main.cpp（输出 main.exe）" ON)
option(FGUI_BUILD_EXAMPLES "编译 example/ 目录下的示例程序"               OFF)

# ============================================================
# 编译器标志（兼容原 Makefile 配置）
# ============================================================
add_compile_options(
    -fexec-charset=UTF-8
    -fdiagnostics-color=always
    -Wno-narrowing
    -Wno-conversion-null
)

# 静态链接 C++/GCC 运行时
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

# ============================================================
# 输出目录：可执行文件统一生成在 bin 文件夹
# ============================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# ============================================================
# SDL2 依赖路径（使用 dependency 文件夹中的新版本）
# ============================================================
set(SDL2_DIR      "${CMAKE_SOURCE_DIR}/dependency/SDL2-2.32.10")
set(SDL2TTF_DIR   "${CMAKE_SOURCE_DIR}/dependency/SDL2_ttf-2.24.0")
set(SDL2IMAGE_DIR "${CMAKE_SOURCE_DIR}/dependency/SDL2_image-2.8.12")

# ============================================================
# 外部工具库目录（非 SDL 官方依赖）
#   优先使用本机开发路径；若不存在（例如他人克隆本仓库，或换机）
#   则自动回退到仓库内的 otherlibs/ 目录。
#   首次在新机器上构建时，请把依赖头文件复制到 otherlibs/ 下，
#   清单见 docs/wiki/01-构建与项目结构.md。
# ============================================================

# Fgui 依赖的工具头文件（SmoothCpp/SDLWindowStuff/decode/FontEx 等）
if(EXISTS "D:/AppInstallers/RecentlyC++Programs")
    set(FGUI_EXT_LIBS_DIR "D:/AppInstallers/RecentlyC++Programs")
else()
    set(FGUI_EXT_LIBS_DIR "${CMAKE_SOURCE_DIR}/otherlibs")
endif()

# GL_Commdlg 对话框库（示例程序 001_TextEditor 需要）
if(EXISTS "D:/Desktop/Git/GL_Commdlg/include")
    set(GL_COMMDLG_DIR "D:/Desktop/Git/GL_Commdlg/include")
else()
    set(GL_COMMDLG_DIR "${CMAKE_SOURCE_DIR}/otherlibs")
endif()

# ============================================================
# 头文件搜索路径
# ============================================================
include_directories(
    ${SDL2_DIR}/include
    ${SDL2TTF_DIR}/include
    ${SDL2IMAGE_DIR}/include
    ${GL_COMMDLG_DIR}
    ${FGUI_EXT_LIBS_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# ============================================================
# 库文件搜索路径
# ============================================================
link_directories(
    ${SDL2_DIR}/lib/x64
    ${SDL2TTF_DIR}/lib/x64
    ${SDL2IMAGE_DIR}/lib/x64
)

# ============================================================
# 外部 C++ 源文件（QR-Code-generator）
#   qrcodegen 已随仓库自带于 include/Fgui/QR-Code-generator/，
#   无需再从外部目录引用
# ============================================================
set(EXT_SRCS
    "${CMAKE_SOURCE_DIR}/include/Fgui/QR-Code-generator/qrcodegen.cpp"
)

# ============================================================
# 公共链接库列表（测试程序与示例程序共用）
# 注意：SDL2main 必须在 SDL2 之前链接（MinGW 要求）
# 已移除 FFmpeg 相关库（冗余）和 SDL2_mixer（未使用）
# ============================================================
set(FGUI_LINK_LIBS
    SDL2main
    SDL2
    SDL2_image
    SDL2_ttf
    winmm
    ws2_32
    user32
    gdi32
    msimg32
    imm32
    psapi
    comdlg32
    dwmapi
    ole32
    uuid
    shell32
    ksuser
)

# ============================================================
# 测试程序目标（test/main.cpp）-> main.exe
# 由 FGUI_BUILD_TEST 开关控制
# ============================================================
if(FGUI_BUILD_TEST)
    add_executable(fast_gui2 ${CMAKE_SOURCE_DIR}/test/main.cpp ${EXT_SRCS})
    # 输出文件名设为 main.exe（而非默认的 fast_gui2.exe）
    set_target_properties(fast_gui2 PROPERTIES OUTPUT_NAME "main")
    target_link_libraries(fast_gui2 ${FGUI_LINK_LIBS})
endif()

# ============================================================
# 示例程序目标（example/*.cpp）
# 由 FGUI_BUILD_EXAMPLES 开关控制，每个示例生成独立可执行文件
# ============================================================
if(FGUI_BUILD_EXAMPLES)
    file(GLOB EXAMPLE_SRCS ${CMAKE_SOURCE_DIR}/example/*.cpp)
    set(EXAMPLE_TARGETS "")
    foreach(example_src ${EXAMPLE_SRCS})
        get_filename_component(example_name ${example_src} NAME_WE)
        set(example_target "example_${example_name}")
        add_executable(${example_target} ${example_src} ${EXT_SRCS})
        set_target_properties(${example_target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        )
        target_link_libraries(${example_target} ${FGUI_LINK_LIBS})
        list(APPEND EXAMPLE_TARGETS ${example_target})
    endforeach()
    # 汇总目标：cmake --build ... --target examples 可一次编译全部示例
    if(EXAMPLE_TARGETS)
        add_custom_target(examples DEPENDS ${EXAMPLE_TARGETS})
    endif()
endif()
