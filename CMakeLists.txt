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
# SDL3 依赖路径（使用 dependency 文件夹中的新版本）
# ============================================================
set(SDL3_DIR      "${CMAKE_SOURCE_DIR}/dependency/SDL3-3.4.14")
set(SDL3TTF_DIR   "${CMAKE_SOURCE_DIR}/dependency/SDL3_ttf-3.2.2")
set(SDL3IMAGE_DIR "${CMAKE_SOURCE_DIR}/dependency/SDL3_image-3.4.4")

# ============================================================
# 外部工具库目录（非 SDL 官方依赖）
#   使用仓库内的 otherlibs/ 目录（已随仓库自带 SDL3 迁移版头文件）。
# ============================================================

# Fgui 依赖的工具头文件（SmoothCpp/SDL_WindowIM/decode/FontEx 等）
set(FGUI_EXT_LIBS_DIR "${CMAKE_SOURCE_DIR}/otherlibs")

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
    ${SDL3_DIR}/include
    ${SDL3TTF_DIR}/include
    ${SDL3IMAGE_DIR}/include
    ${GL_COMMDLG_DIR}
    ${FGUI_EXT_LIBS_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# ============================================================
# 库文件搜索路径
# ============================================================
link_directories(
    ${SDL3_DIR}/lib/x64
    ${SDL3TTF_DIR}/lib/x64
    ${SDL3IMAGE_DIR}/lib/x64
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
# SDL3 在 Windows 上直接链接 SDL3.lib 即可（SDL3 已处理 main 入口）
# 已移除 FFmpeg 相关库（冗余）和 SDL_mixer（未使用）
# ============================================================
set(FGUI_LINK_LIBS
    SDL3
    SDL3_image
    SDL3_ttf
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
