#include "MemoryManager.h"
#include "StackAllocator.h"
#include "StopWatch.h"
#include <ctime>
#include <time.h>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace CustomMemoryManager
{
#ifdef _MEMDEBUG
	MemoryManager::MemoryManager()
	{
#if !defined(_NOOUTPUTFILE) || defined(_STACK) || defined(_DSTACK) || defined(_POOL) || defined(_HEAP) || (ALLOCATIONS_FOR_FILEOUTPUT > 0)
#ifdef _DEBUG
		_wmkdir(L"Logs");
		_wmkdir(L"Logs\\Debug");

		auto time1 = std::chrono::system_clock::now();
		std::time_t time2 = std::chrono::system_clock::to_time_t(time1);
		char carray[100];
		ctime_s(carray, sizeof(carray), &time2);
		mDateTimeStamp = carray;

		std::replace(mDateTimeStamp.begin(), mDateTimeStamp.end(), ':', '-');
		std::replace(mDateTimeStamp.begin(), mDateTimeStamp.end(), '\n', '_');

		char carray2[300] = "Logs\\Debug\\";
		strcat_s(carray2, mDateTimeStamp.c_str());

		wchar_t wcarray[300];
		std::size_t num = 0U;
		mbstowcs_s(&num, wcarray, carray2, 300);
		_wmkdir(wcarray);

		mOutputDirectory = "Logs\\Debug\\" + mDateTimeStamp + "\\";
#endif // _DEBUG

#ifdef  _DEBUGOPTIMIZED
		_wmkdir(L"Logs");
		_wmkdir(L"Logs\\DebugOptimized");

		auto time1 = std::chrono::system_clock::now();
		std::time_t time2 = std::chrono::system_clock::to_time_t(time1);
		char carray[100];
		ctime_s(carray, sizeof(carray), &time2);
		mDateTimeStamp = carray;

		std::replace(mDateTimeStamp.begin(), mDateTimeStamp.end(), ':', '-');
		std::replace(mDateTimeStamp.begin(), mDateTimeStamp.end(), '\n', '_');

		char carray2[300] = "Logs\\DebugOptimized\\";
		strcat_s(carray2, mDateTimeStamp.c_str());

		wchar_t wcarray[300];
		std::size_t num = 0U;
		mbstowcs_s(&num, wcarray, carray2, 300);
		_wmkdir(wcarray);

		mOutputDirectory = "Logs\\DebugOptimized\\" + mDateTimeStamp + "\\";
#endif
#endif
	}
#endif // _MEMDEBUG

	MemoryManager::~MemoryManager()
	{
		if (!mPoolAllocators.empty())
		{
			for (auto& it : mPoolAllocators)
			{
				delete it.second;
			}
		}
		if (!mHeapAllocators.empty())
		{
			for (auto& it : mHeapAllocators)
			{
				delete it.second;
			}
		}
		mStackAllocators.clear();
		mDoubleStackAllocators.clear();
		mPoolAllocators.clear();
		mHeapAllocators.clear();
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
#ifdef _MEMDEBUG
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
#ifdef _MEMDEBUG
				mAllocatorData.emplace(std::move(name), Data());
#endif // _MEMDEBUG
				isCreated = true;
			}
		}
		else if (type == AllocType::HEAP)
		{
			// If Not found: then add it.
			if (mHeapAllocators.find(name) == mHeapAllocators.end())
			{
				mHeapAllocators.emplace(std::move(name), std::move(new Allocators::HeapAllocator(size)));
#ifdef _MEMDEBUG
				mAllocatorData.emplace(std::move(name), Data());
#endif // _MEMDEBUG
				isCreated = true;
			}
		}

		return isCreated;
	}

	void* MemoryManager::Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type, const std::size_t alignment, std::string fileName, const std::size_t lineNumber, Allocators::Info info)
	{
		if (type == AllocType::STACK)
		{
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
				void* ptr = it->second.allocate(allocAmount_bytes, alignment);
				stopWatch.Stop();
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					data.mSumAllocationTime += stopWatch.Elapsed().count();
					++(data.mNumAllocations);
				}
#if (defined(_DEBUG) || defined(_OUTPUTFILE)) && !defined(_NOOUTPUTFILE) || defined(_STACK)
				OutputFile(name, fileName, lineNumber, allocAmount_bytes, ptr);
#else
				fileName; lineNumber;
#endif // _DEBUG
				return ptr;
#else
				fileName; lineNumber;
				return it->second.allocate(allocAmount_bytes, alignment);
#endif // _MEMDEBUG
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;

				stopWatch.Start();
				void* ptr = it->second.allocate(allocAmount_bytes, alignment, info);
				stopWatch.Stop();

				// if the ptr was actually allocated
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					if (info == Allocators::Info::TOP)
					{
						data.mSumAllocationTime += stopWatch.Elapsed().count();
						++(data.mNumAllocations);
					}
					else if (info == Allocators::Info::BOTTOM)
					{
						data.mSumAllocationTime_Bottom += stopWatch.Elapsed().count();
						++(data.mNumAllocations_Bottom);
					}
				}
#if (defined(_DEBUG) || defined(_OUTPUTFILE)) && !defined(_NOOUTPUTFILE) || defined(_DSTACK)
				OutputFile(name, fileName, lineNumber, allocAmount_bytes, ptr);
#endif // _DEBUG
				return ptr;
#else
				return it->second.allocate(allocAmount_bytes, alignment, info);
#endif // _MEMDEBUG
			}
		}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
				void* ptr = it->second->allocate(allocAmount_bytes, alignment);
				stopWatch.Stop();
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					data.mSumAllocationTime += stopWatch.Elapsed().count();
					++(data.mNumAllocations);
				}
#if (defined(_DEBUG) || defined(_OUTPUTFILE)) && !defined(_NOOUTPUTFILE) || defined(_POOL)
				OutputFile(name, fileName, lineNumber, allocAmount_bytes, ptr);
#endif // _DEBUG
				return ptr;
#else
				return it->second->allocate(allocAmount_bytes, alignment);		// TODO: update, 'allocAmount_bytes' doesn't match with input actual of 'numObjects' (i hacked it)
#endif // _MEMDEBUG
			}
		}
		else if (type == AllocType::HEAP)
		{
			const std::unordered_map<std::string, Allocators::HeapAllocator*>::iterator it = mHeapAllocators.find(name);
			if (it != mHeapAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
				void* ptr = it->second->allocate(allocAmount_bytes, alignment, info);
				stopWatch.Stop();
				if (ptr != nullptr)
				{
					Data& data = mAllocatorData.find(name)->second;
					data.mSumAllocationTime += stopWatch.Elapsed().count();
					++(data.mNumAllocations);
					//if (data.mNumAllocations < 20000)
					//	std::cout << data.mSumAllocationTime / data.mNumAllocations << std::endl;
				}
#if (defined(_DEBUG) || defined(_OUTPUTFILE)) && !defined(_NOOUTPUTFILE) || defined(_HEAP)
				OutputFile(name, fileName, lineNumber, allocAmount_bytes, ptr);
#endif // _DEBUG
				return ptr;
#else
				return it->second->allocate(allocAmount_bytes, alignment, info);
#endif // _MEMDEBUG
			}
		}
		return nullptr;
	}

	//MemData MemoryManager::Allocate_GetData(std::size_t allocAmount_bytes, const std::string& name, const AllocType type)
	//{
	//	return MemData(Allocate(allocAmount_bytes, name, type), type, name, *this);
	//}

	void MemoryManager::Deallocate(void* ptr, const std::string& name, const AllocType type, const std::string fileName, const std::size_t lineNumber, const Allocators::Info info)
	{
		lineNumber;
		if (type == AllocType::STACK)
		{
			const std::unordered_map<std::string, Allocators::StackAllocator>::iterator it = mStackAllocators.find(name);
			if (it != mStackAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // _MEMDEBUG
				it->second.deallocate(ptr);
#ifdef _MEMDEBUG
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				data.mSumDeallocationTime += stopWatch.Elapsed().count();
				++(data.mNumDeallocations);
#endif // _MEMDEBUG
			}
		}
		else if (type == AllocType::DOUBLESTACK)
		{
			const std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator>::iterator it = mDoubleStackAllocators.find(name);
			if (it != mDoubleStackAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // _MEMDEBUG
				it->second.deallocate(nullptr, info);
#ifdef _MEMDEBUG
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				if (info == Allocators::Info::TOP)
				{
					data.mSumDeallocationTime += stopWatch.Elapsed().count();
					++(data.mNumDeallocations);
				}
				else if (info == Allocators::Info::BOTTOM)
				{
					data.mSumDeallocationTime_Bottom += stopWatch.Elapsed().count();
					++(data.mNumDeallocations_Bottom);
				}
#endif // _MEMDEBUG
			}
		}
		else if (type == AllocType::POOL)
		{
			const std::unordered_map<std::string, Allocators::IAllocator*>::iterator it = mPoolAllocators.find(name);
			if (it != mPoolAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // _MEMDEBUG
				it->second->deallocate(ptr);
#ifdef _MEMDEBUG
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				data.mSumDeallocationTime += stopWatch.Elapsed().count();
				++(data.mNumDeallocations);
#endif // _MEMDEBUG
			}
		}
		else if (type == AllocType::HEAP)
		{
			const std::unordered_map<std::string, Allocators::HeapAllocator*>::iterator it = mHeapAllocators.find(name);
			if (it != mHeapAllocators.end())
			{
#ifdef _MEMDEBUG
				Library::StopWatch stopWatch;
				stopWatch.Start();
#endif // _MEMDEBUG
				it->second->deallocate(ptr);
#ifdef _MEMDEBUG
				stopWatch.Stop();
				Data& data = mAllocatorData.find(name)->second;
				data.mSumDeallocationTime += stopWatch.Elapsed().count();
				++(data.mNumDeallocations);
#endif // _MEMDEBUG
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
		else if (type == AllocType::HEAP)
		{
			const std::unordered_map<std::string, Allocators::HeapAllocator*>::iterator it = mHeapAllocators.find(name);
			if (it != mHeapAllocators.end())
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
		else if (type == AllocType::HEAP)
		{
			for (const auto& heap : mHeapAllocators)
			{
				keys.push_back(heap.first);
			}
		}
		return keys;
	}

#ifdef _MEMDEBUG
	const MemoryManager::Data* const MemoryManager::GetData(const std::string& name)
	{
		std::unordered_map<std::string, Data>::iterator it = mAllocatorData.find(name);
		if (it == mAllocatorData.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	void MemoryManager::OutputFile(const std::string& allocatorName, const std::string& fileName, const std::size_t lineNumber, std::size_t allocationSize_bytes, void* ptr)
	{
		std::string dir(mOutputDirectory + allocatorName + ".txt");
		FILE* outfile;
		fopen_s(&outfile, dir.c_str(), "a+");
		if (outfile != nullptr)
		{
#ifdef _WIN64
			fprintf_s(outfile, "%s\tLine: %zu\t%zu\t%llX\n", fileName.c_str(), lineNumber, allocationSize_bytes, reinterpret_cast<std::intptr_t>(ptr));
#else	// _WIN32
			fprintf_s(outfile, "%s\tLine: %u\t%u\t%X\n", fileName.c_str(), lineNumber, allocationSize_bytes, reinterpret_cast<std::intptr_t>(ptr));
#endif
		}
		fclose(outfile);
	}

	void MemoryManager::OutputFileAverages()
	{
		std::string directory(mOutputDirectory + std::to_string(ALLOCATIONS_FOR_FILEOUTPUT) + "_Data.txt");
		FILE* outfile;
		fopen_s(&outfile, directory.c_str(), "w+");
		if (outfile != nullptr)
		{
			for (const auto& data : mAllocatorData)
			{
				const Data& d = data.second;
				fprintf_s(outfile,
					"%s:\n\tAverage Allocation Time:\t%f\n\tAverage Deallocation Time:\t%f\n\tNumber of Allocations:\t\t%lld\n\tNumber of Deallocations:\t%lld\n\n"
					, data.first.c_str(), 
					d.mSumAllocationTime / d.mNumAllocations, 
					d.mSumDeallocationTime / d.mNumDeallocations, 
					d.mNumAllocations, 
					d.mNumDeallocations);
			}
		}
		fclose(outfile);
	}

	bool MemoryManager::AreAllAllocationsOver() const
	{
#if ALLOCATIONS_FOR_FILEOUTPUT > 0
		bool isGood = false;
		for (const auto& data : mAllocatorData)
		{
			if (data.second.mNumAllocations < ALLOCATIONS_FOR_FILEOUTPUT)
			{
				isGood = false;
				break;
			}
			isGood = true;
		}
		return isGood;
#else
		return false;
#endif
	}
#endif // _MEMDEBUG
}