#include "MemoryManager.h"
#include "StackAllocator.h"

namespace CustomMemoryManager
{
	bool MemoryManager::CreateAllocator(const std::string& name, const std::size_t size, const AllocType type)
	{
		bool isCreated = false;

		if (type == AllocType::STACK)
		{
			// If Not found: then add it.
			if (mStackAllocators.find(name) == mStackAllocators.end())
			{
				mStackAllocators.emplace(std::move(name), std::move(Allocators::StackAllocator(size)));
				isCreated = true;
			}
		}

		return isCreated;
	}

	void* MemoryManager::Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type)
	{
		if (type == AllocType::STACK)
		{
			if (mStackAllocators.find(name) != mStackAllocators.end())
			{
				return mStackAllocators.find(name)->second.allocate(allocAmount_bytes);
			}
		}
		return nullptr;
	}

	void MemoryManager::Deallocate(void* ptr, const std::string& name, const AllocType type)
	{
		if (type == AllocType::STACK)
		{
			std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
				ptr;
				it->second.deallocate();
			}
		}
	}
}