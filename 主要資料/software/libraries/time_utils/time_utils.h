// time_utils.h
#ifndef _TIME_UTILS_H_
#define _TIME_UTILS_H_

#include <stdint.h>

#ifndef RTC_FREQ
#define RTC_FREQ 32768  // 或依實際使用的頻率定義，例如 16000000
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint32_t millis(void);
uint64_t micros(void);

#ifdef __cplusplus
}
#endif

#endif /*_TIME_UTILS_H_*/