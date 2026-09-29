#ifndef ROS_TIME_H_
#define ROS_TIME_H_

#include <stdint.h>

// 防止漏用 ros::
inline void normalizeSecNSec(uint32_t, uint32_t)
  __attribute__((error("You must use ros::normalizeSecNSec(...)")));

namespace ros {
__attribute__((noinline))
void normalizeSecNSec(uint32_t& sec, uint32_t& nsec);
}

#endif
