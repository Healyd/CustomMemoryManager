
#ifdef MemDebug

#include "MemoryGui.h"
#include "MemoryManager.h"
#include "MemPtr.h"
#include <iostream>

namespace CustomMemoryManager
{
	MemoryGui::MemoryGui(MemoryManager& manager)
		: mMemoryManager(&manager)
	{
		// Stack Initialization
		std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::STACK);
		for (const auto& name : stackNames)
		{
			mGraphData.emplace(std::move(std::pair<std::string, std::deque<float>>(name, std::deque<float>())));
		}

		// Double Stack Initialization
		std::vector<std::string> doubleStackNames = mMemoryManager->Get(MemoryManager::AllocType::DOUBLESTACK);
		for (const auto& name : doubleStackNames)
		{
			mGraphData.emplace(std::move(std::pair<std::string, std::deque<float>>(name + "_top", std::deque<float>())));
			mGraphData.emplace(std::move(std::pair<std::string, std::deque<float>>(name + "_bot", std::deque<float>())));
		}

		// Pool Initialization (currently only support for std::uint32_t pools)
		std::vector<std::string> poolNames = mMemoryManager->Get(MemoryManager::AllocType::POOL);
		for (const auto& name : poolNames)
		{
			mGraphData.emplace(std::move(std::pair<std::string, std::deque<float>>(name, std::deque<float>())));
		}

		std::vector<std::string> heapNames = mMemoryManager->Get(MemoryManager::AllocType::HEAP);
		for (const auto& name : heapNames)
		{
			mGraphData.emplace(std::move(std::pair<std::string, std::deque<float>>(name, std::deque<float>())));
		}

		// Malloc Initialization
		{
			std::vector<std::uint32_t*> IntVector;
			for (std::uint32_t i = 0U; i < 200; ++i)
			{
				IntVector.push_back(static_cast<std::uint32_t*>(malloc(sizeof(std::uint32_t))));
			}
			mData.emplace(std::pair<std::string, MemoryGuiData>("MALLOC", { IntVector, std::deque<float>() }));
		}

		srand((std::uint32_t)(time(NULL)));
	}

	void MemoryGui::RunGui()
	{
		ImGui::Text("Hello from the Memory Gui!");

		if (mMemoryManager != nullptr)
		{
			ShowAllocatorSelector("Show Stack Window", mShowStackWindows, mMemoryManager->Get(MemoryManager::AllocType::STACK));
			ShowAllocatorSelector("Show Double Stacks Window", mShowDoubleStackWindows, mMemoryManager->Get(MemoryManager::AllocType::DOUBLESTACK));
			ShowAllocatorSelector("Show Pools Window", mShowPoolWindows, mMemoryManager->Get(MemoryManager::AllocType::POOL));
			ShowAllocatorSelector("Show Heaps Window", mShowHeapWindows, mMemoryManager->Get(MemoryManager::AllocType::HEAP));

			// Malloc
			ImGui::Checkbox("Show Malloc Window", &mShowMallocWindows);
			{
				std::string text = "\t\tMALLOC Window";
				ImGui::Text(text.c_str());
			}
		}

		StackGuiWindow();
		DoubleStackGuiWindow();
		PoolGuiWindow();
		HeapGuiWindow();
		MallocGuiWindow();
	}
	
	void MemoryGui::EndGui()
	{

	}
	
	void MemoryGui::ShowAllocatorSelector(const std::string& checkBoxName, bool& mShowWindow, std::vector<std::string> allocatorNames)
	{
		ImGui::Checkbox(checkBoxName.c_str(), &mShowWindow);
		for (const std::string& name : allocatorNames)
		{
			std::string text = "\t\t" + name + " Window";
			ImGui::Text(text.c_str());
		}
	}

	void MemoryGui::StackGuiWindow()
	{
		if (!mShowStackWindows)
		{
			return;
		}

		ImGui::Begin("Stacks Window");

		if (mMemoryManager != nullptr)
		{
			std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::STACK);
			for (const std::string& name : stackNames)
			{
				Allocators::StackAllocator* stack = static_cast<Allocators::StackAllocator*>(mMemoryManager->Get(name, MemoryManager::AllocType::STACK));
				if (stack != nullptr)
				{
					const MemoryManager::Data* const data = mMemoryManager->GetData(name);

					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					ImGui::Text("%s Size (Bytes): %u", name.c_str(), stack->StackSize_Bytes());
					ImGui::Text("%s Used Space (Bytes): %i", name.c_str(), stack->UsedSpace_Bytes());
					ImGui::Text("%s Average Allocation Time: %f microseconds", name.c_str(), data->mAverageAllocationTime / data->mNumAllocations);
					ImGui::Text("%s Average Deallocation Time: %f microseconds", name.c_str(), data->mAverageDeallocationTime / data->mNumDeallocations);
					ImGui::Text("%s Num Allocations: %u", name.c_str(), data->mNumAllocations);
					ImGui::Text("%s Num Dellocations: %u", name.c_str(), data->mNumDeallocations);

					if (ImGui::Button(std::string(name + " Alloc a std::uint32_t").c_str()))
					{
						std::uint32_t* addr = static_cast<std::uint32_t*>(stack->allocate(sizeof(std::uint32_t)));
						*addr = 4;
#ifdef _DEBUG
						if (stack->IsStackOverflow())
							mStackOverflowList.emplace(std::move(name));
#endif //_DEBUG
					}


					if (ImGui::Button(std::string("Clear " + name).c_str()))
					{
						stack->Clear();
					}
#ifdef _DEBUG
					for (const auto& name1 : mStackOverflowList)
					{
						if (name1 == name)
							ImGui::TextColored(ImVec4(1, 0, 0, 1), std::string(name1 + " OVERFLOW").c_str());
					}
#endif //_DEBUG

					GraphHistorgram(name, mGraphData.find(name)->second/*.mGraphData*/, static_cast<float>(stack->UsedSpace_Bytes()), static_cast<float>(stack->StackSize_Bytes()));

					ImGui::Text("");
					ImGui::Separator();
				}
			}
		}
		if (ImGui::Button("Close"))
		{
			mShowStackWindows = false;
		}
		ImGui::End();
	}

	void MemoryGui::DoubleStackGuiWindow()
	{
		if (!mShowDoubleStackWindows)
		{
			return;
		}

		ImGui::Begin("Double Stacks Window");

		if (mMemoryManager != nullptr)
		{
			std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::DOUBLESTACK);
			for (const std::string& name : stackNames)
			{
				Allocators::DoubleEndedStackAllocator* stack = static_cast<Allocators::DoubleEndedStackAllocator*>(mMemoryManager->Get(name, MemoryManager::AllocType::DOUBLESTACK));
				if (stack != nullptr)
				{
					const MemoryManager::Data* const data = mMemoryManager->GetData(name);

					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					// Total
					ImGui::Text("%s Total Size (Bytes): %u", name.c_str(), stack->StackSize_Bytes());
					ImGui::Text("%s Total Used Size (Bytes): %u", name.c_str(), stack->UsedBytes());

					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

					// Top Stack
					ImGui::Text("%s Top Size (Bytes): %u", name.c_str(), stack->TopStackSize_Bytes());
					ImGui::Text("%s Top Used Size (Bytes): %i", name.c_str(), stack->UsedBytes_Top());

					ImGui::Spacing();

					ImGui::Text("%s Top Average Allocation Time: %f microseconds", name.c_str(), data->mAverageAllocationTime / data->mNumAllocations);
					ImGui::Text("%s Top Average Deallocation Time: %f microseconds", name.c_str(), data->mAverageDeallocationTime / data->mNumDeallocations);
					ImGui::Text("%s Top Num Allocations: %u", name.c_str(), data->mNumAllocations);
					ImGui::Text("%s Top Num Dellocations: %u", name.c_str(), data->mNumDeallocations);

					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

					// Bottom Stack
					ImGui::Text("%s Bottom Size (Bytes): %u", name.c_str(), stack->BottomStackSize_Bytes());
					ImGui::Text("%s Bottom Used Size (Bytes): %i", name.c_str(), stack->UsedBytes_Bottom());

					ImGui::Spacing();

					ImGui::Text("%s Bottom Average Allocation Time: %f microseconds", name.c_str(), data->mAverageAllocationTime_Bottom / data->mNumAllocations_Bottom);
					ImGui::Text("%s Bottom Average Deallocation Time: %f microseconds", name.c_str(), data->mAverageDeallocationTime_Bottom / data->mNumDeallocations_Bottom);
					ImGui::Text("%s Bottom Num Allocations: %u", name.c_str(), data->mNumAllocations_Bottom);
					ImGui::Text("%s Bottom Num Dellocations: %u", name.c_str(), data->mNumDeallocations_Bottom);

					if (ImGui::Button(std::string(name + " Alloc a std::uint32_t on Top").c_str()))
					{
						std::uint32_t* addr = static_cast<std::uint32_t*>(stack->allocate(sizeof(std::uint32_t), CustomMemoryManager::Allocators::Info::TOP));
						if (addr)
							*addr = 4;
					}
					if (ImGui::Button(std::string(name + " Alloc a std::uint32_t on Bottom").c_str()))
					{
						std::uint32_t* addr = static_cast<std::uint32_t*>(stack->allocate(sizeof(std::uint32_t), CustomMemoryManager::Allocators::Info::BOTTOM));
						if (addr)
							*addr = 4;
					}
					if (ImGui::Button(std::string("Clear " + name).c_str()))
					{
						stack->Clear();
					}
					if (ImGui::Button(std::string("Clear Bottom " + name).c_str()))
					{
						stack->ClearBottom();
					}
					if (ImGui::Button(std::string("Clear Top " + name).c_str()))
					{
						stack->ClearTop();
					}

					GraphHistorgram(name + " Top", mGraphData.find(name + "_top")->second/*.mGraphData*/, static_cast<float>(stack->UsedBytes_Top()), static_cast<float>(stack->TopStackSize_Bytes()));
					GraphHistorgram(name + " Bottom", mGraphData.find(name + "_bot")->second/*.mGraphData_Bottom*/, static_cast<float>(stack->UsedBytes_Bottom()), static_cast<float>(stack->BottomStackSize_Bytes()));

					ImGui::Text("");
					ImGui::Separator();
				}
			}
		}
		if (ImGui::Button("Close"))
		{
			mShowDoubleStackWindows = false;
		}
		ImGui::End();
	}

	void MemoryGui::PoolGuiWindow()
	{
		if (!mShowPoolWindows)
		{
			return;
		}

		ImGui::Begin("Pools Window");

		if (mMemoryManager != nullptr)
		{
			std::vector<std::string> poolNames = mMemoryManager->Get(MemoryManager::AllocType::POOL);
			for (const std::string& name : poolNames)
			{
				Allocators::IAllocator* pool = mMemoryManager->Get(name, MemoryManager::AllocType::POOL);
				if (pool != nullptr)
				{
					const MemoryManager::Data* const data = mMemoryManager->GetData(name);

					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					ImGui::Text("%s Size (Bytes): %u", name.c_str(), pool->Size_Bytes());
					ImGui::Text("%s Used Space (Bytes): %i", name.c_str(), pool->UsedSize_Bytes());

					ImGui::Text("%s Average Allocation Time: %f microseconds", name.c_str(), data->mAverageAllocationTime / data->mNumAllocations);
					ImGui::Text("%s Average Deallocation Time: %f microseconds", name.c_str(), data->mAverageDeallocationTime / data->mNumDeallocations);
					ImGui::Text("%s Num Allocations: %u", name.c_str(), data->mNumAllocations);
					ImGui::Text("%s Num Dellocations: %u", name.c_str(), data->mNumDeallocations);

					GraphHistorgram(name, mGraphData.find(name)->second/*.mGraphData*/, static_cast<float>(pool->UsedSize_Bytes()), static_cast<float>(pool->Size_Bytes()));

					ImGui::Text("");
					ImGui::Separator();
				}
			}
		}

		if (ImGui::Button("Close"))
		{
			mShowPoolWindows = false;
		}
		ImGui::End();
	}

	void MemoryGui::HeapGuiWindow()
	{
		if (!mShowHeapWindows)
		{
			return;
		}

		ImGui::Begin("Heaps Window");

		if (mMemoryManager != nullptr)
		{
			std::vector<std::string> heapNames = mMemoryManager->Get(MemoryManager::AllocType::HEAP);
			for (const std::string& name : heapNames)
			{
				Allocators::HeapAllocator* heap = static_cast<Allocators::HeapAllocator*>(mMemoryManager->Get(name, MemoryManager::AllocType::HEAP));
				if (heap != nullptr)
				{
					const MemoryManager::Data* const data = mMemoryManager->GetData(name);

					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					ImGui::Text("%s Size (Bytes): %u", name.c_str(), heap->Size_Bytes());
					ImGui::Text("%s Used Space (Bytes): %i", name.c_str(), heap->UsedSize_Bytes());

					ImGui::Text("%s Average Allocation Time: %f microseconds", name.c_str(), data->mAverageAllocationTime / data->mNumAllocations);
					ImGui::Text("%s Average Deallocation Time: %f microseconds", name.c_str(), data->mAverageDeallocationTime / data->mNumDeallocations);
					ImGui::Text("%s Num Allocations: %u", name.c_str(), data->mNumAllocations);
					ImGui::Text("%s Num Dellocations: %u", name.c_str(), data->mNumDeallocations);

					//if (data->mNumAllocations <= 10000 && data->mNumDeallocations <= 10000)
						//std::cout << data->mAverageAllocationTime / data->mNumAllocations << ", " << data->mAverageDeallocationTime / data->mNumDeallocations << std::endl;

					ImGui::Text("%s Num Active Nodes: %u", name.c_str(), heap->NumActiveNodes());
					ImGui::Text("%s Num InActive Nodes: %u", name.c_str(), heap->NumInActiveNodes());
					ImGui::Text("%s Num Nodes: %u", name.c_str(), heap->NumNodes());

					GraphHistorgram(name, mGraphData.find(name)->second, static_cast<float>(heap->UsedSize_Bytes()), static_cast<float>(heap->Size_Bytes()));

					ImGui::Text("");
					ImGui::Separator();
				}
			}
		}

		if (ImGui::Button("Close"))
		{
			mShowHeapWindows = false;
		}
		ImGui::End();
	}

	void MemoryGui::MallocGuiWindow()
	{
		if (!mShowMallocWindows)
		{
			return;
		}

		ImGui::Begin("Malloc Window");

		if (mMemoryManager != nullptr)
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1), mMallocName.c_str());

			ImGui::Text("%s Average Allocation Time: %f microseconds", mMallocName.c_str(), mData.find(mMallocName)->second.mAverageAllocationTime / mData.find(mMallocName)->second.mNumAllocations);
			ImGui::Text("%s Average Dellocation Time: %f microseconds", mMallocName.c_str(), mData.find(mMallocName)->second.mAverageDeallocationTime / mData.find(mMallocName)->second.mNumDeallocations);
			ImGui::Text("%s Num Allocations: %u", mMallocName.c_str(), mData.find(mMallocName)->second.mNumAllocations);
			ImGui::Text("%s Num Dellocations: %u", mMallocName.c_str(), mData.find(mMallocName)->second.mNumDeallocations);

			TestAllocationsIntsMalloc(mData.find(mMallocName)->second.mIntVector, mData.find(mMallocName)->second.mGraphData, mMallocName, 100, 100);

			ImGui::Text("");
			ImGui::Separator();
		}

		if (ImGui::Button("Close"))
		{
			mShowPoolWindows = false;
		}
		ImGui::End();
	}

	void MemoryGui::TestAllocationsIntsMalloc(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>&, const std::string& name, int allocAmount, int deallocAmount)
	{
		if (allocAmount <= 0U || deallocAmount <= 0U)
		{
			return;
		}

		std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
		std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

		Library::StopWatch stopWatch;

		for (std::uint32_t i = 0; i < numObjectsToDeallocate; ++i)
		{
			if (!intPtrVectors.empty())
			{
				std::uint32_t* intPtr = intPtrVectors.back();
				intPtrVectors.pop_back();
				stopWatch.Start();
				delete intPtr;
				stopWatch.Stop();
				mData.find(name)->second.mAverageDeallocationTime += stopWatch.Elapsed().count();
				++(mData.find(name)->second.mNumDeallocations);
			}
		}

		stopWatch.Reset();

		for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
		{
			stopWatch.Start();
			std::uint32_t* ptr = static_cast<std::uint32_t*>(malloc(sizeof(std::uint32_t)));
			stopWatch.Stop();
			intPtrVectors.push_back(ptr);
			mData.find(name)->second.mAverageAllocationTime += stopWatch.Elapsed().count();
			++(mData.find(name)->second.mNumAllocations);
		}
	}

	void MemoryGui::GraphHistorgram(const std::string& name, std::deque<float>& floatData, float usedSpace, float scaleMax)
	{
		if (floatData.size() < 101U)
		{
			floatData.push_back(usedSpace);
		}
		else
		{
			floatData.push_back(usedSpace);
			floatData.pop_front();
		}

		float *dat = new float[floatData.size()];
		for (std::uint32_t i = 0; i < floatData.size(); ++i)
		{
			dat[i] = floatData[i];
		}
		ImGui::PlotHistogram(
			std::string(name + " Graph").c_str(),	// Plot Name
			dat,									// Data
			(int)floatData.size(),					// Amount of data to show
			0,										// Offset
			"",										// Overlay Text
			0,										// Scale Min
			scaleMax,								// Scale Max
			ImVec2(100, 100)						// Graph size
		);
		delete dat;
	}
}
#endif // MemDebug