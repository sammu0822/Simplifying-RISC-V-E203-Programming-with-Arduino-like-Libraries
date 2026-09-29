#include <stdint.h>
#include "time_utils.h"
#include "encoding.h"
#include "platform.h"

/* 請根據實際硬體的 RTC_FREQ 設定數值 */
//如果你使用 16MHz 的 clock，請把 RTC_FREQ 改成 16000000
//如果你使用 32.768kHz 的 RTC（常見於低功耗裝置），就保留 32768

#define MTIME      ((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIME))
// RTC_FREQ 預設為 32768（請注意實際使用的頻率）
#ifndef RTC_FREQ
#define RTC_FREQ   32768
#endif

uint32_t millis(void)
{
    uint64_t now_ticks = *MTIME;              // 獲取當前 mtime 計時值（以 ticks 為單位）
    uint32_t ms = now_ticks * 1000 / RTC_FREQ; // 換算成毫秒（以 uint32_t 回傳）
    return ms;
}

//增加時間解析度或使用微秒（micros()）
uint64_t micros(void)
{
    uint64_t ticks = *MTIME;
    return ticks * 1000000 / RTC_FREQ;
}

//---------------------------------------------------------
// // 模擬 64-bit 硬體計時器暫存器
// #define MTIME_LOW   (*(volatile uint32_t*)(0x0200BFF8))
// #define MTIME_HIGH  (*(volatile uint32_t*)(0x0200BFFC))

// uint64_t read_mtime() {
//     uint32_t hi, lo;
//     do {
//         hi = MTIME_HIGH;
//         lo = MTIME_LOW;
//     } while (hi != MTIME_HIGH); // 防止 high/low 不一致
//     return ((uint64_t)hi << 32) | lo;
// }

// uint32_t millis(void) {
//     uint64_t ticks = read_mtime();  // 單位：clock ticks
//     uint32_t ms = ticks / 16000;    // 16,000 ticks = 1 ms @ 16 MHz
//     return ms;
// }

// //如果使用 get_cpu_freq() 也可以這樣換算：
// return (uint32_t)(ticks / (get_cpu_freq() / 1000));