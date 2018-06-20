#pragma once
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
		struct MemoryGuiData
		{
			std::vector<std::uint32_t*> mIntVector;
			std::deque<float> mGraphData;
			float mAverageAllocationTime{ 0.0f };
			std::uint64_t mNumAllocations{ 0U };
			std::uint64_t mNumDeallocations{ 0U };
		};
		void StackGuiWindow();
		void DoubleStackGuiWindow();
		void TestAllocationsInts(
			std::vector<std::uint32_t*>& intPtrVectors, 
			std::deque<float>& floatData,
			const std::string& name, int allocAmount, int deallocAmount);

		std::unordered_map<std::string, MemoryGuiData> mData;

		// Stack Allocators
		bool mShowStackWindows{ false };
		//std::unordered_map<std::string, std::vector<std::uint32_t*>> mStackIntPtrVectors;
		//std::unordered_map<std::string, std::deque<float>> mStackGraphData;
		//float mAverageAllocationTime = 0.0f;
		//std::uint64_t mNumAllocations = 0;

		bool mShowDoubleStackWindows{ false };



#ifdef _DEBUG
		std::set<std::string> mStackOverflowList;
#endif // _DEBUG

		float data[4] = { 1, 1, 1, 1 };
		float data2[200];
		float amount{ 0 };
		std::uint32_t mUsedBytes_Stack{ 0 };

		MemoryManager* mMemoryManager{ nullptr };

		std::chrono::microseconds allocationSpeed;
		std::chrono::microseconds allocationSpeedOther;

		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> end;
	};
}