#pragma once

#include<iostream>
#include<vector>
#include<atomic>


#include "macros.h"
// this is not pure LFQueue if write more then size or writer is faster then reader then data may be overwrite or lose 
// so check please this , our project it work but real word not 
//solution use mutex , 2. Dynamically  grow deque, 3. Use a Bounded Lock-Free Queue + Wait Strategy(thread.yeild())
namespace Common
{
	template<typenameT>
	class LFQueue final // final key prevent class inharit from this but it can inherit from other classes
	{
		public:
	         explicit LFQueue(std::size_t size):vecStore(size,T()){}	
		     auto getNextToWrite() noexcept //noexcept with  function guarantees it will never throw an exception.
			 {
				 return &vecStore[aNext_write_index];
			 }
			 auto getNextToRead() noexcept {
				 return (size()?(&vecStore[aNext_read_index]):nullptr);
			 }
			 auto updateWriteIndex() noexcept {
				 aNext_write_index = (aNext_write_index + 1) % vecStore.size();
				 num_elements_++;
			 }
			 auto updateReadIndex() noexcept{
				 aNext_read_index = (aNext_read_index + 1) % vecStore.size();
				 ASSERT(num_elements_ != 0, "Read an invalid element in:" + std::to_string(pthread_self()));
                 num_elements_--;
			 }
			 //write more than vec.size() elements without reading, this will overwrite unread data — resulting in data corruption.
		     auto size() const noexcept //const means this method does not modify any member variables of the class.
			 {
				 return num_elements.load();
			 }
		     LFQueue() =  delete;
			 LFQueue(const LFQueue &) = delete;
			 LFQueue(const LFQueue &&) = delete;
			 LFQueue &operator = (const LFQueue &) = delete;
			 LFQueue &operator = (const LFQueue &&) = delete;
			 
		private:
		     std::vector<T> vecStore;
             std::atomic<size_t> aNext_write_index{0};
             std::atomic<size_t> aNext_read_index{0};
             std::atomic<size_t> aNum_elements{0};
		
	};
}

