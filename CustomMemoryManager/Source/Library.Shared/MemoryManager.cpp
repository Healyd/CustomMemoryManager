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
		else if (type == AllocType::DOUBLESTACK)
		{
			// If Not found: then add it.
			if (mDoubleStackAllocators.find(name) == mDoubleStackAllocators.end())
			{
				mDoubleStackAllocators.emplace(std::move(name), std::move(Allocators::DoubleEndedStackAllocator(size / 2, size / 2, 0U)));
				isCreated = true;
			}
		}

		return isCreated;
	}

	void* MemoryManager::Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type, const Allocators::Info info)
	{
		if (type == AllocType::STACK)
		{
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
				return it->second.allocate(allocAmount_bytes);
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
				return it->second.allocate(allocAmount_bytes, info);
			}
		}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
				return it->second->allocate(allocAmount_bytes);		// TODO: update, 'allocAmount_bytes' doesn't match with input actual of 'numObjects' (i hacked it)
			}
		}
		return nullptr;
	}

	//MemData MemoryManager::Allocate_GetData(std::size_t allocAmount_bytes, const std::string& name, const AllocType type)
	//{
	//	return MemData(Allocate(allocAmount_bytes, name, type), type, name, *this);
	//}

	void MemoryManager::Deallocate(void* ptr, const std::string& name, const AllocType type, const Allocators::Info info)
	{
		if (type == AllocType::STACK)
		{
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
				it->second.deallocate(ptr);
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
				it->second.deallocate(nullptr, info);
			}
		}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
				it->second->deallocate(ptr);
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
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
				return &(it->second);
			}
		}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
				return it->second;
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
		else if (type == AllocType::DOUBLESTACK)
		{
			for (const auto& dstack : mDoubleStackAllocators)
			{
				keys.push_back(dstack.first);
			}
		}
		else if (type == AllocType::POOL)
		{
			for (const auto& pool : mPoolAllocators)
			{
				keys.push_back(pool.first);
			}
		}
		return keys;
	}
}