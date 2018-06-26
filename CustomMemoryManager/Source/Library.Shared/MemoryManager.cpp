#include "MemoryManager.h"
#include "StackAllocator.h"
#include "StopWatch.h"

#ifdef _DEBUGOPTIMIZED

#endif // _DEBUGOPTIMIZED


namespace CustomMemoryManager
{
	MemoryManager::~MemoryManager()
	{
		if (!mPoolAllocators.empty())
		{
			for (auto& it : mPoolAllocators)
			{
				delete it.second;
			}
		}
		mStackAllocators.clear();
		mPoolAllocators.clear();
		mDoubleStackAllocators.clear();
	}

	bool MemoryManager::CreateAllocator(const std::string& name, const std::size_t size, const AllocType type)
	{
		bool isCreated = false;

		if (type == AllocType::STACK)
		{
			// If Not found: then add it.
			if (mStackAllocators.find(name) == mStackAllocators.end())
			{
				mStackAllocators.emplace(std::move(name), std::move(Allocators::StackAllocator(size)));
#ifdef MemDebug
				mAllocatorData.emplace(std::move(name), Data());
#endif // MemDeug
				isCreated = true;
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			// If Not found: then add it.
			if (mDoubleStackAllocators.find(name) == mDoubleStackAllocators.end())
			{
				mDoubleStackAllocators.emplace(std::move(name), std::move(Allocators::DoubleEndedStackAllocator(size / 2, size / 2, 0U)));
#ifdef MemDebug
				mAllocatorData.emplace(std::move(name), Data());
#endif // MemDebug
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
#ifdef MemDebug
				Library::StopWatch stopWatch;
				stopWatch.Start();
				void* ptr = it->second.allocate(allocAmount_bytes);
				stopWatch.Stop();
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					data.mAverageAllocationTime += stopWatch.Elapsed().count();
					++(data.mNumAllocations);
				}
				return ptr;
#else
				return it->second.allocate(allocAmount_bytes);
#endif // MemDebug
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
#ifdef MemDebug
				Library::StopWatch stopWatch;
				
				stopWatch.Start();
				void* ptr = it->second.allocate(allocAmount_bytes, info);
				stopWatch.Stop();

				// if the ptr was actually allocated
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					if (info == Allocators::Info::TOP)
					{
						data.mAverageAllocationTime += stopWatch.Elapsed().count();
						++(data.mNumAllocations);
					}
					else if (info == Allocators::Info::BOTTOM)
					{
						data.mAverageAllocationTime_Bottom += stopWatch.Elapsed().count();
						++(data.mNumAllocations_Bottom);
					}
				}
				return ptr;
#else
				return it->second.allocate(allocAmount_bytes, info);
#endif // MemDebug
					}
				}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
#ifdef MemDebug
				Library::StopWatch stopWatch;
				stopWatch.Start();
				void* ptr = it->second->allocate(allocAmount_bytes);
				stopWatch.Stop();
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					data.mAverageAllocationTime += stopWatch.Elapsed().count();
					++(data.mNumAllocations);
				}
				return ptr;
#else
				return it->second->allocate(allocAmount_bytes);		// TODO: update, 'allocAmount_bytes' doesn't match with input actual of 'numObjects' (i hacked it)
#endif // MemDebug
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
#ifdef MemDebug
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // MemDebug
				it->second.deallocate(ptr);
#ifdef MemDebug
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				data.mAverageDeallocationTime += stopWatch.Elapsed().count();
				++(data.mNumDeallocations);
#endif // MemDebug
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
#ifdef MemDebug
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // MemDebug
				it->second.deallocate(nullptr, info);
#ifdef MemDebug
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				if (info == Allocators::Info::TOP)
				{
					data.mAverageDeallocationTime += stopWatch.Elapsed().count();
					++(data.mNumDeallocations);
				}
				else if (info == Allocators::Info::BOTTOM)
				{
					data.mAverageDeallocationTime_Bottom += stopWatch.Elapsed().count();
					++(data.mNumDeallocations_Bottom);
				}
#endif // MemDebug
			}
		}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
#ifdef MemDebug
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // MemDebug
				it->second->deallocate(ptr);
#ifdef MemDebug
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				data.mAverageDeallocationTime += stopWatch.Elapsed().count();
				++(data.mNumDeallocations);
#endif // MemDebug
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

#ifdef MemDebug
	const MemoryManager::Data* const MemoryManager::GetData(const std::string& name)
	{
		std::unordered_map<std::string, Data>::iterator it = mAllocatorData.find(name);
		if (it == mAllocatorData.end())
		{
			return nullptr;
		}
		return &(it->second);
	}
#endif // MemDebug
		}