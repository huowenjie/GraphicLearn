
BIN_PATH := .
INC_PATH := .
SRC_PATH := . mem math
LIB_PATH :=
LIB_LIST := SDL2 dl m
DEF_LIST := GR_MEMORY_DEBUG

VER_MAJ := 1
VER_MIN := 0.0
VERSION := $(VER_MAJ).$(VER_MIN)

# 输出文件、soname 设置
LINK_NAME := GraphicRender.out
TARGET := $(BIN_PATH)/$(LINK_NAME)

# 搜索源文件并获取 .c 文件名列表
SRC_FILES := $(foreach dir,$(SRC_PATH),$(wildcard $(dir)/*.c))

# 将源文件名称替换为 .o 名称
OBJ_FILES := $(patsubst %.c,%.o,$(SRC_FILES))

# 将源文件名称替换为 .d 名称
DEP_FILES := $(patsubst %.c,%.d,$(SRC_FILES))

# 将目录分解替换为 -Ixxx -Ixxx
INC_PATHS := $(foreach inc,$(INC_PATH),$(addprefix -I,$(inc)))

# 将附加依赖目录分解替换为 -Lxxx -Lxxx
LIB_PATHS := $(foreach lib,$(LIB_PATH),$(addprefix -L,$(lib)))

# 将依赖库名称分解为 -lxxx
LNK_LIST := $(foreach so,$(LIB_LIST),$(addprefix -l,$(so)))

# 在所有预处理标签前面加-D
DEF_NAMES := $(foreach def,$(DEF_LIST),$(addprefix -D,$(def)))

# 搜索源文件
vpath %.c $(SRC_PATH)

# 搜索头文件
vpath %.h $(INC_PATH)

# 定义模式编译规则
%.o:%.c %.d
	gcc -g -c $< -o $@ $(INC_PATHS) $(DEF_NAMES) -Wall -Wformat=0

# 定义模式规则，生成依赖文件
%.d:%.c
	gcc $^ -M -MF $@ $(INC_PATHS)

# 显式定义目标规则，链接生成最终的可执行程序
$(TARGET): $(OBJ_FILES)
	gcc $^ -o $@ $(LIB_PATHS) $(LNK_LIST)

# 自动生成依赖，将所有的 .d 文件的内容包含在这里
ifeq ($(MAKECMDGOALS),)
include $(DEP_FILES)
endif

.PHONY: clean show
clean:
	rm -f $(DEP_FILES) $(OBJ_FILES) $(TARGET) 

show:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILES)
	@echo $(DEP_FILES)
	@echo $(INC_PATHS)
	@echo $(LIB_PATHS)
	@echo $(LNK_LIST)
