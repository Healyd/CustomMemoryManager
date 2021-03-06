#pragma once

#ifdef _MEMDEBUG

//#include "imgui-1.61/imgui.h"
//#include <imgui.h>
#include <cstdint>
#include <unordered_map>
#include <set>
#include <deque>
#include "StopWatch.h"
#include "MemoryManager.h"

namespace CustomMemoryManager
{
	namespace Allocators
	{
		class IAllocator;
	}

	class MemoryGui final
	{
	public:
		MemoryGui(MemoryManager& manager);
		MemoryGui(const MemoryGui&) = delete;
		MemoryGui(MemoryGui&&) = delete;
		MemoryGui& operator=(const MemoryGui&) = delete;
		MemoryGui& operator=(MemoryGui&&) = delete;
		~MemoryGui() = default;

		void Init();
		void RunGui();
		void EndGui();

		//void SetMemoryManager(MemoryManager& manager);

	private:
		void ShowAllocatorSelector(const std::string& checkBoxName, bool& mShowWindow, std::vector<std::string> allocatorNames);

		void StackGuiWindow();
		void DoubleStackGuiWindow();
		void PoolGuiWindow();
		void HeapGuiWindow();
		void MallocGuiWindow();

		void ShowBaseData(const std::string& name, const Allocators::IAllocator* const allocator, const MemoryManager::Data* const data);

		void TestAllocationsIntsMalloc(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount);

		void GraphHistorgram(const std::string& name, std::deque<float>& floatData, float usedSpace, float scaleMax);
		
		struct MemoryGuiData
		{
			std::vector<std::uint32_t*> mIntVector;
			std::deque<float> mGraphData;

			float mAverageAllocationTime{ 0.0f };
			float mAverageDeallocationTime{ 0.0f };
			std::uint64_t mNumAllocations{ 0U };
			std::uint64_t mNumDeallocations{ 0U };

			std::vector<std::uint32_t*> mIntVector_Bottom;
			std::deque<float> mGraphData_Bottom;

			float mAverageAllocationTime_Bottom{ 0.0f };
			float mAverageDeallocationTime_Bottom{ 0.0f };
			std::uint64_t mNumAllocations_Bottom{ 0U };
			std::uint64_t mNumDeallocations_Bottom{ 0U };
		};

		std::unordered_map<std::string, MemoryGuiData> mData;

		std::unordered_map<std::string, std::deque<float>> mGraphData;

		// Window Bools
		bool mShowStackWindows{ false };
		bool mShowDoubleStackWindows{ false };
		bool mShowPoolWindows{ false };
		bool mShowMallocWindows{ false };
		bool mShowHeapWindows{ false };

		const std::string mMallocName = "MALLOC";
		bool doOnceFileOutput = true;

#ifdef _DEBUG
		std::set<std::string> mStackOverflowList;
#endif // _DEBUG



		// RANDOM VARIABLES that will be removed
		//float data[4] = { 1, 1, 1, 1 };
		//float data2[200];
		//float amount{ 0 };
		//std::uint32_t mUsedBytes_Stack{ 0 };

		MemoryManager* mMemoryManager{ nullptr };

		std::chrono::microseconds allocationSpeed;
		std::chrono::microseconds allocationSpeedOther;

		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> end;
	};
}
#endif // _MEMDEBUG
