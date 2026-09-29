# See LICENSE for license details.

ifndef _TCORE_MK_COMMON
_TCORE_MK_COMMON := # defined

.PHONY: all
all: $(TARGET) $(CXX_OBJS)
	@echo ">>> ALL is triggered"
# 	$(CXX) -c main.cpp -o main.cpp.o

# 預設下載方式（可被 override）
FLASHXIP    := flashxip
FLASH       := flash
ITCM        := itcm
DOWNLOAD    := flash

# 路徑定義
HEAD_DIR = $(BSP_BASE)/$(BOARD)/include
DRIVER_DIR = $(BSP_BASE)/$(BOARD)/drivers
ENV_DIR = $(BSP_BASE)/$(BOARD)/env
STUB_DIR = $(BSP_BASE)/$(BOARD)/stubs

# 彙編檔案
ASM_SRCS += $(ENV_DIR)/start.S
ASM_SRCS += $(ENV_DIR)/entry.S

ASM_OBJS := $(ASM_SRCS:.S=.o)

# 自動收集 C++ 檔案
# CXX_SRCS  := $(wildcard *.cpp) # ?= 若外部已定義就不覆蓋
# # CXX_SRCS += $(shell find . -name "*.cpp")
# CXX_OBJS  := $(CXX_SRCS:.cpp=.cpp.o)

# C 檔案
C_SRCS += $(ENV_DIR)/init.c \
          $(STUB_DIR)/close.c \
          $(STUB_DIR)/_exit.c \
          $(STUB_DIR)/write_hex.c \
          $(STUB_DIR)/fstat.c \
          $(STUB_DIR)/isatty.c \
          $(STUB_DIR)/lseek.c \
          $(STUB_DIR)/read.c \
          $(STUB_DIR)/sbrk.c \
          $(STUB_DIR)/write.c

ifeq ($(REPLACE_PRINTF),1) 
C_SRCS += $(STUB_DIR)/malloc.c \
          $(STUB_DIR)/printf.c
endif

# # C++ 檔案（只支援單一 cpp，否則報錯）
# ifeq ($(words $(CXX_SRCS)),1)
# CXX_OBJ := $(PROGRAM).o
# else ifneq ($(CXX_SRCS),)
# $(error [ERROR] CXX_SRCS 設定了多個 .cpp，請確認僅有一個並符合 $(PROGRAM).cpp)
# endif

# 自動收集當前資料夾下的多個 .cpp 檔案，並偵測是否加入使用者自定的 CXX_SRCS 中（定義在各專案的Makefile中）
ifdef USER_DEFINED_CXX_SRCS
CXX_SRCS  += $(wildcard *.cpp)
CXX_OBJS  += $(CXX_SRCS:.cpp=.o)
else
$(info USER_DEFINED_CXX_SRCS undefined in "../software/$(PROGRAM)/Makefile",using default settings)
CXX_SRCS  := $(wildcard *.cpp)
CXX_OBJS  := $(CXX_SRCS:.cpp=.o)
endif

# .dump / .verilog 額外輸出
DUMP_OBJS     := $(CXX_OBJ:.o=.dump)
VERILOG_OBJS  := $(CXX_OBJ:.o=.verilog)

# LINK_OBJS  := $(sort $(ASM_OBJS) $(C_OBJS) $(CXX_OBJ))
#連結多c++物件
LINK_OBJS := $(sort $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS)) 
LINK_DEPS  :=

# 連結檔 Linker script 選擇
ifeq ($(DOWNLOAD),${FLASH}) 
LINKER_SCRIPT := $(ENV_DIR)/link_flash.lds
endif
ifeq ($(DOWNLOAD),${ITCM}) 
LINKER_SCRIPT := $(ENV_DIR)/link_itcm.lds
endif
ifeq ($(DOWNLOAD),${FLASHXIP}) 
LINKER_SCRIPT := $(ENV_DIR)/link_flashxip.lds
endif

LINK_DEPS += $(LINKER_SCRIPT)

# Include paths，新增 user libraries 搜尋路徑
INCLUDES += -I$(STUB_DIR)
INCLUDES += -I$(DRIVER_DIR)
INCLUDES += -I$(ENV_DIR)
INCLUDES += -I$(HEAD_DIR)
INCLUDES += -I$(abspath ../../software/libraries)  # <<<< 加上這行解 wiring_xxx.h 無法找到的問題
INCLUDES += -I$(abspath ../../software/libraries/serial)  # <<<< 加上這行解 serial.hpp 無法找到的問題
INCLUDES += -I$(abspath ../../software/libraries/ros_lib)
INCLUDES += -I$(abspath ../../software/libraries/ros_lib/ros)
INCLUDES += -I$(abspath ../../software/libraries/time_utils)


# Flags 設定 （使用 C++ 時關閉例外處理與 RTTI，如果需要可以改掉）
## 如果使用 new 或 std::string 等，請刪除 -fno-exceptions 和 -fno-rtti，並確認連結 -lstdc++ 沒有漏掉
CFLAGS += -g -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) \
          -ffunction-sections -fdata-sections -fno-common \
# 		  -fno-exceptions -fno-rtti

ifeq ($(REPLACE_PRINTF),1) 
CFLAGS +=  -fno-builtin-printf -fno-builtin-malloc 
endif

CFLAGS   += $(INCLUDES)
CXXFLAGS += $(CFLAGS)

# 強制啟用 C++11 ABI
CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=1


LDFLAGS += -T $(LINKER_SCRIPT)  -nostartfiles \
			-Wl,--check-sections \
		    -L$(ENV_DIR) -lstdc++ \
			-Wl,--gc-sections  \
			-Wl,--trace-symbol=normalizeSecNSec # debug test_ROS，追蹤哪個 .o 檔正在嘗試解析 normalizeSecNSec

# C++ 標準庫(如果要使用try/catch typeid或dynamic_cast就要去除"CFLAGS += -fno-exceptions -fno-rtti")
# Wl,--gc-sections 可能移除未直接引用的函數

ifeq ($(USE_NANO),1) 
LDFLAGS += --specs=nano.specs 
endif
ifeq ($(NANO_PFLOAT),1) 
LDFLAGS += -u _printf_float 
endif
ifeq ($(REPLACE_PRINTF),1) 
LDFLAGS += -Wl,--wrap=malloc -Wl,--wrap=printf 
endif


# ASM_OBJS := $(ASM_SRCS:.S=.o)
# C_OBJS := $(C_SRCS:.c=.o)
# DUMP_OBJS := $(C_SRCS:.c=.dump)
# VERILOG_OBJS := $(C_SRCS:.c=.verilog)
# DUMP_OBJS += $(CXX_SRCS:.cpp=.cpp.dump)
# VERILOG_OBJS += $(CXX_SRCS:.cpp=.cpp.verilog)
# ASM_OBJS    := $(ASM_SRCS:.S=.o)
# C_OBJS      := $(C_SRCS:.c=.o)
# DUMP_OBJS   += $(CXX_SRCS:.cpp=.cpp.dump)
# VERILOG_OBJS += $(CXX_SRCS:.cpp=.cpp.verilog)

# LINK_OBJS   += $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS)
# LINK_OBJS   := $(sort $(LINK_OBJS)) #去重
# CXX_SRCS += $(filter-out $(C_SRCS:.c=.cpp), $(wildcard *.cpp))

# # 新增，支援 C++ 檔案------
# # CXX_SRCS += $(wildcard *.cpp) #CXX_SRCS += $(shell find . -name "*.cpp")
# # CXX_OBJS := $(CXX_SRCS:.cpp=.o)
# CXX_OBJS := $(CXX_SRCS:.cpp=.cpp.o)
# #-------------------


# Clean 所需移除的物件
CLEAN_OBJS  += $(TARGET) $(C_OBJS) $(CXX_OBJS) $(ASM_OBJS) $(DUMP_OBJS) $(VERILOG_OBJS)


#====================
# 建構規則
#====================

# Link rule
$(TARGET): $(LINK_OBJS) $(LINK_DEPS)
	@echo "[Linking] $@"
	$(CXX) $(CFLAGS) $(LINK_OBJS) -o $@ $(LDFLAGS)
	$(info LINK_OBJS = $(LINK_OBJS))
	@echo "[Success] ELF generated: $(TARGET)"

# 彙編編譯規則
%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo "[ASM] $< → $@"

# C 編譯規則
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo "[C] $< → $@"

# C++ 編譯規則（強制產出為 $(PROGRAM).o）
# $(CXX_OBJ): $(CXX_SRCS)
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<
# 	@echo "[C++] $< → $@"
# 	@echo "Using compiler: $(CXX)"
# C++ 編譯規則
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "[C++] $< → $@"

# 使用$(CC)C編譯器做連結。
# 當使用C++像有new,std::string，C++ constructor等，可能會出現 undefined reference to __gxx_personality_v0 等錯
# Link rule
# $(TARGET): $(LINK_OBJS) $(LINK_DEPS)
# 	$(CXX) $(CFLAGS) $(INCLUDES) $(LINK_OBJS) -o $@ $(LDFLAGS)
# 	@echo "C_OBJS = $(C_OBJS)"
# 	@echo "CXX_OBJS = $(CXX_OBJS)"
# 	@echo "LINK_OBJS = $(LINK_OBJS)"
# 	@echo "TARGET_CXX = $(CXX)"
# 	@echo "TARGET = $(TARGET), LINK_OBJS = $(LINK_OBJS)"
# $(TARGET): $(LINK_OBJS) $(LINK_DEPS)
# 	@echo "CXX_OBJS = $(CXX_OBJS)"
# 	@echo "CXX_SRCS = $(CXX_SRCS)"
# 	@echo "LINK_OBJS = $(LINK_OBJS)"
# 	@echo "[Rebuilding] $@"
# 	@echo "Building TARGET: $@"
# 	@echo "[Linking ELF] $(TARGET)"
# 	@set -e
# 	$(CXX) $(CFLAGS) $(INCLUDES) $(LINK_OBJS) -o $@ $(LDFLAGS)
# 	RET=$$?
# 	if [ $$RET -ne 0 ] then 
# 		echo "[ERROR] Linking failed with return code $$RET"
# 		exit $$RET
# 	fi
# 	@echo "[Success] ELF generated: $(TARGET)"
# 	@echo "C_OBJS = $(C_OBJS)"
# 	@echo "CXX_OBJS = $(CXX_OBJS)"
# 	@echo "LINK_OBJS = $(LINK_OBJS)"
# 	@echo "TARGET_CXX = $(CXX)"
# 	@echo "TARGET = $(TARGET), LINK_OBJS = $(LINK_OBJS)"
# # 編譯規則
# $(ASM_OBJS): %.o: %.S $(HEADERS)
# 	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
# 	@echo "ASM_OBJS_CC = $(CC)"
	
# # $(CXX_OBJS): %.o: %.cpp $(HEADERS)
# %.cpp.o: %.cpp $(HEADERS)
# 	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $<
# 	@echo "CXX_OBJS_CXX = $(CXX)"
	

.PHONY: clean
clean:
	rm -f $(CLEAN_OBJS)
	@echo "[Clean] done."

endif # _TCORE_MK_COMMON
