#CFLAGS += -I$(abspath ../../libraries)
#CFLAGS += -I$(abspath ../../software/libraries)
TARGET = test_ros_serial
CFLAGS += -O2 -fno-builtin-printf -DNO_INIT
##設定 include 搜尋路徑
#CFLAGS += -I$(abspath ../../libraries)
#CFLAGS += -I/home/roger/Desktop/T-core/TCORE-RISCV-E203-V1.2/TCORE-RISCV-E203/software/libraries

BSP_BASE = ../../bsp

# 設定自製函式庫來源檔與 include 目錄
CXX_SRCS += test_ros_serial.cpp
#C_SRCS += ../../libraries/wiring_digital.c
CXX_SRCS += ../../software/test_rosserial/hardware_serial.cpp
CXX_SRCS += ../../software/test_rosserial/test_ros_serial.cpp
#C_SRCS += ../../software/test_rosserial/pins_T-core.c

include $(BSP_BASE)/tcore-e203/env/common.mk
