##加入自定義標籤告知common.mk
USER_DEFINED_CXX_SRCS += 1


# 設定專案名稱，common.mk 會自動設定 TARGET = test_gpio_elf
PROGRAM = test_UART
#TARGET = test_gpio

#--------------------------------------------------
# 設定 BSP 路徑（這個必須在 include 前）
BSP_BASE = ../../bsp


#--------------------------------------------------
## 一定要在 include 前定義這些，不然會被 common.mk 中的 := 覆蓋掉
C_SRCS += ../../software/libraries/wiring_digital.c
C_SRCS += ../../software/libraries/wiring_analog.c
C_SRCS += ../../software/libraries/pins_T-core.c

# C_SRCS += ../../software/libraries/serial/serial.c
C_SRCS += ../../software/libraries/time_utils/time_utils.c

#--------------------------------------------------
## 明確列出連結的物件檔（補強 common.mk 的行為）
C_OBJS += ../../software/libraries/wiring_digital.o
C_OBJS += ../../software/libraries/wiring_analog.o
C_OBJS += ../../software/libraries/pins_T-core.o

C_OBJS += ../../software/libraries/serial/serial.o
C_OBJS += ../../software/libraries/time_utils/time_utils.o


#--------------------------------------------------
# common.mk 自動處理 主程式.cpp 的編譯與連結
CXX_SRCS += test_UART.cpp


# 加入 ros_lib 所需檔案
CXX_SRCS += ../../software/libraries/serial/serial.cpp
CXX_SRCS += ../../software/libraries/serial/Serial0.cpp
CXX_SRCS += ../../software/libraries/ros_lib/time.cpp
CXX_SRCS += ../../software/libraries/ros_lib/duration.cpp

#--------------------------------------------------
## 明確列出連結的物件檔（補強 common.mk 的行為）
CXX_SRCS += ../../software/libraries/serial/serial.o
CXX_SRCS += ../../software/libraries/serial/Serial0.o
CXX_OBJS += ../../software/libraries/ros_lib/time.o
CXX_OBJS += ../../software/libraries/ros_lib/duration.o



#--------------------------------------------------
# ELF = $(TARGET)_elf  # 新增這行解決無法找到 target 問題


#--------------------------------------------------
# Include paths
INCLUDES += -I$(abspath ../../software/libraries)
INCLUDES += -I$(abspath ../../software/libraries/ros_lib)
INCLUDES += -I$(abspath ../../software/libraries/ros_lib/ros)
INCLUDES += -I$(abspath ../../software/libraries/ros_lib/std_msgs)
INCLUDES += -I$(abspath ../../software/libraries/ros_lib/rosserial_msgs)
INCLUDES += -I$(abspath ../../software/libraries/serial)
INCLUDES += -I$(abspath ../../software/libraries/time_utils)
INCLUDES += -I$(BSP_BASE)/tcore-e203/env


#--------------------------------------------------
# 編譯參數
CFLAGS   += -O2 -fno-builtin-printf -DNO_INIT
## CXXFLAGS += -O2 -fno-builtin-printf -DNO_INIT -fno-exceptions -fno-rtti
CXXFLAGS += -O2 -fno-builtin-printf -DNO_INIT 
CFLAGS   += $(INCLUDES)
CXXFLAGS += $(INCLUDES)

#--------------------------------------------------
# 設定目標名稱
TARGET = $(PROGRAM)


#--------------------------------------------------
# 建構目標 (使用riscv32/64-unknown-elf需要定義)
# all: $(ELF) 
all: $(TARGET)


#--------------------------------------------------
# 最後 include，由common.mk處理所有建構規則
include $(BSP_BASE)/tcore-e203/env/common.mk


###說明--------------------------------------------------
#INCLUDES += ... ->編譯器的「搜尋路徑」設定，對應到 GCC / G++ 的 -I 參數（即 -I<path>），是設定 .h 標頭檔的搜尋路徑
#CXX_SRCS += ... ->要編譯哪些 C++ 原始檔，列出所有希望加進最終程式的 .cpp 檔案，這些檔案會編譯成 .o 物件檔，然後被連結起來產生 .elf 執行檔
