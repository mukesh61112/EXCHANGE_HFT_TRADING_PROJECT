
#ifndef MEMORY_POOLS_H_
#define MEMORY_POOLS_H_
#pragma once
#include<cstdint>
#include<vector>
#include<string>
#include"../header/macros.h"

namespace Common{
template<typename T>
class MemPool final
{
private:
	struct ObjectBlock
	{
		T object_;
		bool  is_free_ = true;

	};
	std::vector<ObjectBlock>  store_;
	size_t next_free_index_ = 0;
};



#endif /* MEMORY_POOLS_H_ */
