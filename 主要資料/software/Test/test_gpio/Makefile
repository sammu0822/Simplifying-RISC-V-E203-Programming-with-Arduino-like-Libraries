# Makefile copy (/test_gpio)
# 設定專案名稱，common.mk 會自動設定 TARGET = test_gpio_elf
PROGRAM = test_gpio
#TARGET = test_gpio

#--------------------------------------------------
# 設定 BSP 路徑（這個必須在 include 前）
BSP_BASE = ../../bsp


#--------------------------------------------------
## 一定要在 include 前定義這些，不然會被 common.mk 中的 := 覆蓋掉
C_SRCS += ../../software/libraries/wiring_digital.c
C_SRCS += ../../software/libraries/wiring_analog.c
C_SRCS += ../../software/libraries/pins_T-core.c


#--------------------------------------------------
## 明確列出連結的物件檔（補強 common.mk 的行為）
C_OBJS += ../../software/libraries/wiring_digital.o
C_OBJS += ../../software/libraries/wiring_analog.o
C_OBJS += ../../software/libraries/pins_T-core.o


#--------------------------------------------------
# common.mk 自動處理 test_gpio.cpp 的編譯與連結
CXX_SRCS := test_gpio.cpp


#--------------------------------------------------
# ELF = $(TARGET)_elf  # 新增這行解決無法找到 target 問題


#--------------------------------------------------
# Include paths
INCLUDES += -I$(abspath ../../software/libraries)
INCLUDES += -I$(BSP_BASE)/tcore-e203/env


#--------------------------------------------------
# 編譯參數
CFLAGS   += -O2 -fno-builtin-printf -DNO_INIT
CXXFLAGS += -O2 -fno-builtin-printf -DNO_INIT -fno-exceptions -fno-rtti
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
# 最後 include，處理所有建構規則
include $(BSP_BASE)/tcore-e203/env/common.mk
