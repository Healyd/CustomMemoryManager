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
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
				return it->second.allocate(allocAmount_bytes);
			}
		}
		return nullptr;
	}

	//MemData MemoryManager::Allocate_GetData(std::size_t allocAmount_bytes, const std::string& name, const AllocType type)
	//{
	//	return MemData(Allocate(allocAmount_bytes, name, type), type, name, *this);
	//}

	void MemoryManager::Deallocate(void* ptr, const std::string& name, const AllocType type)
	{
		if (type == AllocType::STACK)
		{
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
				ptr;
				it->second.deallocate();
			}
		}
	}

	Allocators::IAllocator* MemoryManager::Get(const std::string& name, const AllocType type)
	{
		if (type == AllocType::STACK)
		{
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
				return &(it->second);
			}
		}
		return nullptr;
	}

	std::vector<std::string> MemoryManager::Get(const AllocType type)
	{
		std::vector<std::string> keys;
		if (type == AllocType::STACK)
		{
			for (const auto& stack : mStackAllocators)
			{
				keys.push_back(stack.first);
			}
		}
		return keys;
	}
}