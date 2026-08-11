CXX = g++.exe

TARGET = main.exe

BUILD_DIR = build
BIN_DIR = .
SRC_DIR = src

# 项目内部源码（自动扫描 src/ 目录）
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# ═══════════════════════════════════════════
# 外源C++设置区 —— 在此添加项目目录外部的.cpp文件
# ═══════════════════════════════════════════
# 用法：每行一个文件路径，用反斜杠续行，路径支持正斜杠/
EXT_SRCS = \
    D:/AppInstallers/RecentlyC++Programs/Fgui/QR-Code-generator/qrcodegen.cpp
    # 以后在此添加更多外部源码（注意行尾续行符 \）

# 自动生成外源文件的目标文件列表
EXT_OBJS = $(foreach src,$(EXT_SRCS),$(BUILD_DIR)/$(notdir $(src:.cpp=.o)))

$(info [内部源码] $(SRCS))
$(info [外源文件] $(EXT_SRCS))

# 合并所有目标文件
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
OBJS += $(EXT_OBJS)

DEPS = $(OBJS:.o=.d)

MODE ?= debug

BASE_CXXFLAGS = -fexec-charset=UTF-8 \
                -fdiagnostics-color=always \
                -Wno-narrowing \
                -Wno-conversion-null \
                -static-libstdc++ \
                -static-libgcc \
                -MMD -MP

DEBUG_CXXFLAGS = $(BASE_CXXFLAGS) \
                 -g \
                 -g3 \
                 -O0 \
                 -DDEBUG

RELEASE_CXXFLAGS = $(BASE_CXXFLAGS) \
                   -O2 \
                   -s \
                   -DNDEBUG

CXXFLAGS = $(if $(filter release, $(MODE)), $(RELEASE_CXXFLAGS), $(DEBUG_CXXFLAGS))

INCLUDES = -I"F:/SDL2-2.24.0/include" \
           -I"F:/SDL2_ttf-2.20.1/include" \
           -I"F:/SDL2_mixer-2.6.2/include" \
           -I"F:/SDL2_image-2.6.2/include" \
           -I"E:/InstallessApps/FFmpeg/ffmpeg-master-latest-win64-gpl-shared/ffmpeg-master-latest-win64-gpl-shared/include" \
           -I"D:/AppInstallers/RecentlyC++Programs" \
           -I$(SRC_DIR)

LDFLAGS = -L"F:/SDL2-2.24.0/lib/x64" \
          -L"F:/SDL2_ttf-2.20.1/lib/x64" \
          -L"F:/SDL2_mixer-2.6.2/lib/x64" \
          -L"F:/SDL2_image-2.6.2/lib/x64" \
          -L"E:/InstallessApps/FFmpeg/ffmpeg-master-latest-win64-gpl-shared/ffmpeg-master-latest-win64-gpl-shared/lib"

LIBS = -lwinmm \
       -lSDL2 \
       -lSDL2main \
       -lSDL2_image \
       -lSDL2_ttf \
       -lSDL2_mixer \
       -lws2_32 \
       -luser32 \
       -lgdi32 \
       -lmsimg32 \
       -limm32 \
       -lpsapi \
       -lcomdlg32 \
       -lavcodec \
       -lavformat \
       -lavutil \
       -lswscale \
       -ldwmapi \
       -lole32 \
       -lksuser

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^ $(LIBS) $(LDFLAGS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 外源文件编译规则（自动为 EXT_SRCS 中每个文件生成）
define COMPILE_EXT
$$(BUILD_DIR)/$(notdir $(1:.cpp=.o)): $(1) | $$(BUILD_DIR)
	$$(CXX) $$(CXXFLAGS) $$(INCLUDES) -c "$(1)" -o "$$@"
endef
$(foreach src,$(EXT_SRCS),$(eval $(call COMPILE_EXT,$(src))))

$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

clean:
	@echo Clean up...
	@if exist $(BUILD_DIR)\*.o del /f /q $(BUILD_DIR)\*.o
	@if exist $(BUILD_DIR)\*.d del /f /q $(BUILD_DIR)\*.d  # 新增：清理依赖文件
	@if exist $(BIN_DIR)\*.exe del /f /q $(BIN_DIR)\*.exe
	@echo Cleaning Completed!

.PHONY: all clean debug release

-include $(DEPS)

debug:
	$(MAKE) MODE=debug

release:
	$(MAKE) MODE=release
