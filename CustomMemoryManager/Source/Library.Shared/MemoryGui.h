#pragma once

#ifdef MemDebug

#include "imgui-1.61/imgui.h"
#include <cstdint>
#include <unordered_map>
#include <set>
#include <deque>
#include "StopWatch.h"

namespace CustomMemoryManager
{
	class MemoryManager;

	class MemoryGui final
	{
	public:
		MemoryGui(MemoryManager& manager);
		MemoryGui(const MemoryGui&) = delete;
		MemoryGui(MemoryGui&&) = delete;
		MemoryGui& operator=(const MemoryGui&) = delete;
		MemoryGui& operator=(MemoryGui&&) = delete;
		~MemoryGui() = default;

		void InitializeGui();
		void RunGui();
		void EndGui();

		//void SetMemoryManager(MemoryManager& manager);

	private:
		void StackGuiWindow();
		void DoubleStackGuiWindow();
		void PoolGuiWindow();
		void MallocGuiWindow();

		void TestAllocationsIntsStack(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount);
		void TestAllocationsIntsDoubleStackTop(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount);
		void TestAllocationsIntsDoubleStackBottom(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount);
		void TestAllocationsIntsPool(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount);
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

		//struct MemoryGuiData_DoubleStack : public MemoryGuiData
		//{
		//	std::vector<std::uint32_t*> mIntVector_Bottom;
		//	std::deque<float> mGraphData_Bottom;

		//	float mAverageAllocationTime_Bottom{ 0.0f };
		//	float mAverageDeallocationTime_Bottom{ 0.0f };
		//	std::uint64_t mNumAllocations_Bottom{ 0U };
		//	std::uint64_t mNumDeallocations_Bottom{ 0U };
		//};

		std::unordered_map<std::string, MemoryGuiData> mData;

		// Window Bools
		bool mShowStackWindows{ false };
		bool mShowDoubleStackWindows{ false };
		bool mShowPoolWindows{ false };
		bool mShowMallocWindows{ false };

		const std::string mMallocName = "MALLOC";

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
#endif // MemDebug