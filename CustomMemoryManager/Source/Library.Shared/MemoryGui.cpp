#include "MemoryGui.h"
#include "MemoryManager.h"
#include "MemPtr.h"

namespace CustomMemoryManager
{
	MemoryGui::MemoryGui(MemoryManager& manager)
		: mMemoryManager(&manager)
	{
		// Stack Initialization
		std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::STACK);
		for (const auto& name : stackNames)
		{
			std::vector<std::uint32_t*> IntVector;
			for (std::uint32_t i = 0U; i < 200; ++i)
			{
				IntVector.push_back(static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::STACK)));
			}
			mData.emplace(std::pair<std::string, MemoryGuiData>(name, { IntVector, std::deque<float>() }));
			//mStackIntPtrVectors.emplace(std::pair<std::string, std::vector<std::uint32_t*>>(name, IntVector));
			//mStackGraphData.emplace(std::pair<std::string, std::deque<float>>(name, std::deque<float>()));
		}

		// Double Stack Initialization
		std::vector<std::string> doubleStackNames = mMemoryManager->Get(MemoryManager::AllocType::DOUBLESTACK);
		for (const auto& name : doubleStackNames)
		{
			std::vector<std::uint32_t*> IntVector, IntVector_Bot;
			for (std::uint32_t i = 0U; i < 200; ++i)
			{
				IntVector.push_back(static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, Allocators::Info::TOP)));
				IntVector_Bot.push_back(static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, Allocators::Info::BOTTOM)));
			}
			MemoryGuiData d;
			d.mIntVector = IntVector;
			d.mGraphData = std::deque<float>();
			d.mIntVector_Bottom = IntVector_Bot;
			d.mGraphData_Bottom = std::deque<float>();
			mData.emplace(std::pair<std::string, MemoryGuiData>(name, d));
			//mStackIntPtrVectors.emplace(std::pair<std::string, std::vector<std::uint32_t*>>(name, IntVector));
			//mStackGraphData.emplace(std::pair<std::string, std::deque<float>>(name, std::deque<float>()));
		}

		// Pool Initialization (currently only support for std::uint32_t pools)
		std::vector<std::string> poolNames = mMemoryManager->Get(MemoryManager::AllocType::POOL);
		for (const auto& name : poolNames)
		{
			std::vector<std::uint32_t*> IntVector;
			for (std::uint32_t i = 0U; i < 200; ++i)
			{
				IntVector.push_back(static_cast<std::uint32_t*>(mMemoryManager->Allocate(1, name, CustomMemoryManager::MemoryManager::AllocType::POOL)));
			}
			mData.emplace(std::pair<std::string, MemoryGuiData>(name, { IntVector, std::deque<float>() }));
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
			// Stack Allocators
			ImGui::Checkbox("Show Stacks Window", &mShowStackWindows);
			std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::STACK);
			for (const std::string& name : stackNames)
			{
				std::string text = "\t\t" + name + " Window";
				ImGui::Text(text.c_str());
			}

			// Double-Ended Stack Allocators
			ImGui::Checkbox("Show Double Stacks Window", &mShowDoubleStackWindows);
			std::vector<std::string> doubleStackNames = mMemoryManager->Get(MemoryManager::AllocType::DOUBLESTACK);
			for (const std::string& name : doubleStackNames)
			{
				std::string text = "\t\t" + name + " Window";
				ImGui::Text(text.c_str());
			}

			// Pool Allocators
			ImGui::Checkbox("Show Pools Window", &mShowPoolWindows);
			std::vector<std::string> poolNames = mMemoryManager->Get(MemoryManager::AllocType::POOL);
			for (const std::string& name : poolNames)
			{
				std::string text = "\t\t" + name + " Window";
				ImGui::Text(text.c_str());
			}

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
		MallocGuiWindow();
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
					//ImGui::Begin(std::string(name+"Stack Window").c_str());
					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					ImGui::Text("%s Size (Bytes): %u", name.c_str(), stack->StackSize_Bytes());
					ImGui::Text("%s Used Space (Bytes): %i", name.c_str(), stack->UsedSpace_Bytes());
					ImGui::Text("%s Average Allocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageAllocationTime / mData.find(name)->second.mNumAllocations);
					ImGui::Text("%s Average Deallocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageDeallocationTime / mData.find(name)->second.mNumDeallocations);
					ImGui::Text("%s Num Allocations: %u", name.c_str(), mData.find(name)->second.mNumAllocations);
					ImGui::Text("%s Num Dellocations: %u", name.c_str(), mData.find(name)->second.mNumDeallocations);

					//MemPtr<std::uint32_t> memPtr(nullptr);
						//memPtr.SetPtr(MakeMemPtr_Raw<std::uint32_t>(name, MemoryManager::AllocType::STACK, *mMemoryManager));

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

					TestAllocationsIntsStack(mData.find(name)->second.mIntVector, mData.find(name)->second.mGraphData, name, 100, 100);

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
					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					// Total
					ImGui::Text("%s Total Size (Bytes): %u", name.c_str(), stack->StackSize_Bytes());
					ImGui::Text("%s Total Used Size (Bytes): %u", name.c_str(), stack->UsedBytes());
					
					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

					// Top Stack
					ImGui::Text("%s Top Size (Bytes): %u", name.c_str(), stack->TopStackSize_Bytes());
					ImGui::Text("%s Top Used Size (Bytes): %i", name.c_str(), stack->UsedBytes_Top());
					
					ImGui::Spacing();

					ImGui::Text("%s Top Average Allocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageAllocationTime / mData.find(name)->second.mNumAllocations);
					ImGui::Text("%s Top Average Deallocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageDeallocationTime / mData.find(name)->second.mNumDeallocations);
					ImGui::Text("%s Top Num Allocations: %u", name.c_str(), mData.find(name)->second.mNumAllocations);
					ImGui::Text("%s Top Num Dellocations: %u", name.c_str(), mData.find(name)->second.mNumDeallocations);

					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

					// Bottom Stack
					ImGui::Text("%s Bottom Size (Bytes): %u", name.c_str(), stack->BottomStackSize_Bytes());
					ImGui::Text("%s Bottom Used Size (Bytes): %i", name.c_str(), stack->UsedBytes_Bottom());
					
					ImGui::Spacing();

					ImGui::Text("%s Bottom Average Allocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageAllocationTime_Bottom / mData.find(name)->second.mNumAllocations_Bottom);
					ImGui::Text("%s Bottom Average Deallocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageDeallocationTime_Bottom / mData.find(name)->second.mNumDeallocations_Bottom);
					ImGui::Text("%s Bottom Num Allocations: %u", name.c_str(), mData.find(name)->second.mNumAllocations_Bottom);
					ImGui::Text("%s Bottom Num Dellocations: %u", name.c_str(), mData.find(name)->second.mNumDeallocations_Bottom);


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

					TestAllocationsIntsDoubleStackTop(mData.find(name)->second.mIntVector, mData.find(name)->second.mGraphData, name, 100/2, 100/2);
					TestAllocationsIntsDoubleStackBottom(mData.find(name)->second.mIntVector_Bottom, mData.find(name)->second.mGraphData_Bottom, name, 100/2, 100/2);

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
				Allocators::PoolAllocator<std::uint32_t>* pool = static_cast<Allocators::PoolAllocator<std::uint32_t>*>(mMemoryManager->Get(name, MemoryManager::AllocType::POOL));
				if (pool != nullptr)
				{
					//ImGui::Begin(std::string(name+"Stack Window").c_str());
					ImGui::TextColored(ImVec4(0, 1, 0, 1), name.c_str());

					ImGui::Text("%s Size (Bytes): %u", name.c_str(), pool->PoolSize_Bytes());
					ImGui::Text("%s Used Space (Bytes): %i", name.c_str(), pool->UsedSpace_Bytes());
					ImGui::Text("%s Average Allocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageAllocationTime / mData.find(name)->second.mNumAllocations);
					ImGui::Text("%s Average Deallocation Time: %f microseconds", name.c_str(), mData.find(name)->second.mAverageDeallocationTime / mData.find(name)->second.mNumDeallocations);
					ImGui::Text("%s Num Allocations: %u", name.c_str(), mData.find(name)->second.mNumAllocations);
					ImGui::Text("%s Num Dellocations: %u", name.c_str(), mData.find(name)->second.mNumDeallocations);

					if (ImGui::Button(std::string(name + " Alloc a std::uint32_t").c_str()))
					{
						std::uint32_t* addr = static_cast<std::uint32_t*>(pool->allocate(1));
						*addr = 4;
					}


					if (ImGui::Button(std::string("Clear " + name).c_str()))
					{
						pool->Clear();
					}

					TestAllocationsIntsPool(mData.find(name)->second.mIntVector, mData.find(name)->second.mGraphData, name, 100, 100);

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

			//ImGui::Text("%s Size (Bytes): %u", name.c_str(), pool->PoolSize_Bytes());
			//ImGui::Text("%s Used Space (Bytes): %i", name.c_str(), pool->UsedSpace_Bytes());
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

	void MemoryGui::TestAllocationsIntsStack(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount)
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
				mMemoryManager->Deallocate(intPtr, name, CustomMemoryManager::MemoryManager::AllocType::STACK);
				stopWatch.Stop();
				mData.find(name)->second.mAverageDeallocationTime += stopWatch.Elapsed().count();
				++(mData.find(name)->second.mNumDeallocations);
			}
		}

		stopWatch.Reset();

		CustomMemoryManager::Allocators::StackAllocator* stackAlloc = static_cast<CustomMemoryManager::Allocators::StackAllocator*>(mMemoryManager->Get(name, CustomMemoryManager::MemoryManager::AllocType::STACK));
		std::size_t x = (stackAlloc->StackSize_Bytes() - stackAlloc->UsedSpace_Bytes()) / sizeof(std::uint32_t) + 1;
		if (numObjectsToAllocate >= x)
		{
			numObjectsToAllocate = x;
		}

		for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
		{
			stopWatch.Start();
			std::uint32_t* ptr = static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::STACK));
			stopWatch.Stop();
			intPtrVectors.push_back(ptr);
			mData.find(name)->second.mAverageAllocationTime += stopWatch.Elapsed().count();
			++(mData.find(name)->second.mNumAllocations);
		}

		if (floatData.size() < 101U)
		{
			floatData.push_back((float)stackAlloc->UsedSpace_Bytes());
		}
		else
		{
			floatData.push_back((float)stackAlloc->UsedSpace_Bytes());
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
			(int)floatData.size(),						// Amount of data to show
			0,										// Offset
			"",										// Overlay Text
			0,										// Scale Min
			(float)stackAlloc->StackSize_Bytes(),	// Scale Max
			ImVec2(100, 100)						// Graph size
		);
		delete dat;
	}

	void MemoryGui::TestAllocationsIntsDoubleStackTop(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount)
	{
		if (allocAmount <= 0U || deallocAmount <= 0U)
		{
			return;
		}

		// Top
		{
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
					mMemoryManager->Deallocate(intPtr, name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::TOP);
					stopWatch.Stop();
					mData.find(name)->second.mAverageDeallocationTime += stopWatch.Elapsed().count();
					++(mData.find(name)->second.mNumDeallocations);
				}
			}

			stopWatch.Reset();

			CustomMemoryManager::Allocators::DoubleEndedStackAllocator* stackAlloc = static_cast<CustomMemoryManager::Allocators::DoubleEndedStackAllocator*>(mMemoryManager->Get(name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK));
			std::size_t x = (stackAlloc->TopStackSize_Bytes() - stackAlloc->UsedBytes_Top()) / sizeof(std::uint32_t) + 1;
			if (numObjectsToAllocate >= x)
			{
				numObjectsToAllocate = x;
			}

			for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
			{
				stopWatch.Start();
				std::uint32_t* ptr = static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, Allocators::Info::TOP));
				stopWatch.Stop();
				intPtrVectors.push_back(ptr);
				mData.find(name)->second.mAverageAllocationTime += stopWatch.Elapsed().count();
				++(mData.find(name)->second.mNumAllocations);
			}

			if (floatData.size() < 101U)
			{
				floatData.push_back((float)stackAlloc->UsedBytes_Top());
			}
			else
			{
				floatData.push_back((float)stackAlloc->UsedBytes_Top());
				floatData.pop_front();
			}

			float *dat = new float[floatData.size()];
			for (std::uint32_t i = 0; i < floatData.size(); ++i)
			{
				dat[i] = floatData[i];
			}
			ImGui::PlotHistogram(
				std::string(name + " Graph Top").c_str(),	// Plot Name
				dat,										// Data
				(int)floatData.size(),						// Amount of data to show
				0,											// Offset
				"",											// Overlay Text
				0,											// Scale Min
				(float)stackAlloc->TopStackSize_Bytes(),		// Scale Max
				ImVec2(100, 100)							// Graph size
			);
			delete dat;
		}
	}

	void MemoryGui::TestAllocationsIntsDoubleStackBottom(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount)
	{
		if (allocAmount <= 0U || deallocAmount <= 0U)
		{
			return;
		}

		// Bottom
		{
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
					mMemoryManager->Deallocate(intPtr, name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::BOTTOM);
					stopWatch.Stop();
					mData.find(name)->second.mAverageDeallocationTime_Bottom += stopWatch.Elapsed().count();
					++(mData.find(name)->second.mNumDeallocations_Bottom);
				}
			}

			stopWatch.Reset();

			CustomMemoryManager::Allocators::DoubleEndedStackAllocator* stackAlloc = static_cast<CustomMemoryManager::Allocators::DoubleEndedStackAllocator*>(mMemoryManager->Get(name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK));
			std::size_t x = (stackAlloc->BottomStackSize_Bytes() - stackAlloc->UsedBytes_Bottom()) / sizeof(std::uint32_t) + 1;
			if (numObjectsToAllocate >= x)
			{
				numObjectsToAllocate = x;
			}

			for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
			{
				stopWatch.Start();
				std::uint32_t* ptr = static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, Allocators::Info::BOTTOM));
				stopWatch.Stop();
				intPtrVectors.push_back(ptr);
				mData.find(name)->second.mAverageAllocationTime_Bottom += stopWatch.Elapsed().count();
				++(mData.find(name)->second.mNumAllocations_Bottom);
			}

			if (floatData.size() < 101U)
			{
				floatData.push_back((float)stackAlloc->UsedBytes_Bottom());
			}
			else
			{
				floatData.push_back((float)stackAlloc->UsedBytes_Bottom());
				floatData.pop_front();
			}

			float *dat = new float[floatData.size()];
			for (std::uint32_t i = 0; i < floatData.size(); ++i)
			{
				dat[i] = floatData[i];
			}
			ImGui::PlotHistogram(
				std::string(name + " Graph Bottom").c_str(),	// Plot Name
				dat,										// Data
				(int)floatData.size(),						// Amount of data to show
				0,											// Offset
				"",											// Overlay Text
				0,											// Scale Min
				(float)stackAlloc->BottomStackSize_Bytes(),		// Scale Max
				ImVec2(100, 100)							// Graph size
			);
			delete dat;
		}
	}

	void MemoryGui::TestAllocationsIntsPool(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount)
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
				mMemoryManager->Deallocate(intPtr, name, CustomMemoryManager::MemoryManager::AllocType::POOL);
				stopWatch.Stop();
				mData.find(name)->second.mAverageDeallocationTime += stopWatch.Elapsed().count();
				++(mData.find(name)->second.mNumDeallocations);
			}
		}

		stopWatch.Reset();

		CustomMemoryManager::Allocators::PoolAllocator<std::uint32_t>* stackAlloc = static_cast<CustomMemoryManager::Allocators::PoolAllocator<std::uint32_t>*>(mMemoryManager->Get(name, CustomMemoryManager::MemoryManager::AllocType::POOL));
		std::size_t x = (stackAlloc->PoolSize_Bytes() - stackAlloc->UsedSpace_Bytes()) / sizeof(std::uint32_t) + 1;
		if (numObjectsToAllocate >= x)
		{
			numObjectsToAllocate = x;
		}

		for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
		{
			stopWatch.Start();
			std::uint32_t* ptr = static_cast<std::uint32_t*>(mMemoryManager->Allocate(1, name, CustomMemoryManager::MemoryManager::AllocType::POOL));
			stopWatch.Stop();
			intPtrVectors.push_back(ptr);
			mData.find(name)->second.mAverageAllocationTime += stopWatch.Elapsed().count();
			++(mData.find(name)->second.mNumAllocations);
		}

		if (floatData.size() < 101U)
		{
			floatData.push_back((float)stackAlloc->UsedSpace_Bytes());
		}
		else
		{
			floatData.push_back((float)stackAlloc->UsedSpace_Bytes());
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
			(int)floatData.size(),						// Amount of data to show
			0,										// Offset
			"",										// Overlay Text
			0,										// Scale Min
			(float)stackAlloc->PoolSize_Bytes(),	// Scale Max
			ImVec2(100, 100)						// Graph size
		);
		delete dat;
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

	void MemoryGui::EndGui()
	{

	}
}