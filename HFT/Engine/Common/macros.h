/*
 * macros.h
 *
 *  Created on: May 6, 2025
 *      Author: mukesh
 */

#ifndef MACROS_H_
#define MACROS_H_
#include <iostream>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

inline auto ASSERT(bool cond, const std::string &msg) noexcept {
  if (UNLIKELY(!cond)) {
    std::cerr << "ASSERT : " << msg << std::endl;

    exit(EXIT_FAILURE);
  }
}

inline auto FATAL(const std::string &msg) noexcept {
  std::cerr << "FATAL : " << msg << std::endl;

  exit(EXIT_FAILURE);
}
#endif /* MACROS_H_ */
