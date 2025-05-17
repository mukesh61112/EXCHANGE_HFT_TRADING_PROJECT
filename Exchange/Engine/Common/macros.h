/*
 * macros.h
 *
 *  Created on: May 6, 2025
 *      Author: mukesh
 */

#ifndef MACROS_H_
#define MACROS_H_
#include <iostream>
#include<cstring>
#define LIKELY(x) _builtin_expect(!!(x),1)
#define UNLIKELY(x) _builtin_expect(!!(x),0)
inline auto ASSERT(bool cond, const std::string &msg) noexcept{
			if(UNLIKELY(!cond)){
				std::cerr<<msg<<std::endl;
				exit(EXIT_FAILURE);
			}
		}
inline auto FATEL(const std::string &msg) noexcept{
	std::cerr<<msg<<std::endl;
	exit(EXIT_FAILURE);
}
#endif /* MACROS_H_ */
