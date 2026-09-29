# 軟體目標名稱
TARGET = test_gpio

# 自訂 flag（選擇性）
CFLAGS += -O2 -fno-builtin-printf -DNO_INIT

# BSP 根目錄（會傳給 common.mk）
BSP_BASE = ../../bsp

# 設定自製函式庫的 C 檔來源（由 common.mk 編譯為 .o）
C_SRCS += ../../software/libraries/wiring_digital.c
C_SRCS += ../../software/libraries/wiring_analog.c
C_SRCS += ../../software/libraries/pins_T-core.c

# 設定標頭搜尋路徑 (自製函式庫來源)
INCLUDES += -I$(abspath ../../software/libraries)

# 指定要編譯的 C++ 檔
CXX_SRCS += test_gpio.cpp

LD_OBJS += test_gpio.cpp.o  # 加這行才會正確使用 .o 而不是直接傳 .cpp

# 匯入通用規則與變數
include $(BSP_BASE)/tcore-e203/env/common.mk

# # 設定自製函式庫來源檔與 include 目錄 
# INCLUDES += -I$(abspath ../../software/libraries) 
# TARGET = test_gpio
# CFLAGS += -O2 -fno-builtin-printf -DNO_INIT
# ##設定 include 搜尋路徑

# BSP_BASE = ../../bsp

# # 設定自製函式庫來源檔與 include 目錄 (不用設定 CXX_SRCS，讓 common.mk 自動抓取 *.cpp)
# C_SRCS += ../../software/libraries/wiring_digital.c
# C_SRCS += ../../software/libraries/wiring_analog.c
# C_SRCS += ../../software/libraries/pins_T-core.c


# include $(BSP_BASE)/tcore-e203/env/common.mk
