//============================================================================
// Name        : Chapter4LLSystemTopics.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<cstring>
#define LIKELY(x) __builtin_expect(!!(x),1)
#define UNLIKELY(x) __builtin_expect(!!(x),0)


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


