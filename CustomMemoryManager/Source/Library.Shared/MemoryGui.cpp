#include "MemoryGui.h"
#include "MemoryManager.h"
#include "MemPtr.h"

namespace CustomMemoryManager
{
	MemoryGui::MemoryGui(MemoryManager& manager)
		: mMemoryManager(&manager)
	{
		std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::STACK);
		for (const auto& name : stackNames)
		{
			std::vector<std::uint32_t*> IntVector;
			for (std::uint32_t i = 0U; i < 200; ++i)
			{
				IntVector.push_back(static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::STACK)));
			}
			mData.emplace(std::pair<std::string, MemoryGuiData>(name, {IntVector, std::deque<float>()}));
			//mStackIntPtrVectors.emplace(std::pair<std::string, std::vector<std::uint32_t*>>(name, IntVector));
			//mStackGraphData.emplace(std::pair<std::string, std::deque<float>>(name, std::deque<float>()));
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
		}

		StackGuiWindow();
		DoubleStackGuiWindow();
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

					/*int allocAmount = 0U;
					int deallocAmount = 0U;
					ImGui::InputInt("Alloc Amount: ", &allocAmount);
					ImGui::InputInt("Dealloc Amount: ", &deallocAmount);*/
					TestAllocationsInts(mData.find(name)->second.mIntVector, mData.find(name)->second.mGraphData, name, 100, 100);

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

					ImGui::Text("%s Size (Bytes): %u", name.c_str(), stack->StackSize_Bytes());
					ImGui::Text("%s Used Space Bottom (Bytes): %i", name.c_str(), stack->UsedBytes_Bottom());
					ImGui::Text("%s Used Space Top (Bytes): %i", name.c_str(), stack->UsedBytes_Top());
					//Library::StopWatch stopWatch;

					if (ImGui::Button(std::string(name + " Alloc a std::uint32_t on Top").c_str()))
					{
						//stopWatch.Start();
						std::uint32_t* addr = static_cast<std::uint32_t*>(stack->allocateTop(sizeof(std::uint32_t)));
						//stopWatch.Stop();
						//allocationSpeed = stopWatch.Elapsed();
						if (addr)
							*addr = 4;

						start = std::chrono::high_resolution_clock::now();
						//stopWatch.Restart();
						malloc(sizeof(std::uint32_t));
						end = std::chrono::high_resolution_clock::now();
						//stopWatch.Stop();
						//allocationSpeedOther = stopWatch.Elapsed();
					}

					ImGui::Text("%s Speed: %.12lf", name.c_str(), std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
					//ImGui::Text("%s Speed: %.12lf", name.c_str(), allocationSpeed.count()/1000000.f);
					//ImGui::Text("%s Speed Malloc: %.12lf", name.c_str(), allocationSpeedOther.count() / 1000000.f);
					if (ImGui::Button(std::string(name + " Alloc a std::uint32_t on Bottom").c_str()))
					{
						std::uint32_t* addr = static_cast<std::uint32_t*>(stack->allocateBottom(sizeof(std::uint32_t)));
						if (addr)
							*addr = 4;
					}
					if (ImGui::Button(std::string("Clear " + name).c_str()))
					{
						stack->Clear();
					}
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

	void MemoryGui::TestAllocationsInts(std::vector<std::uint32_t*>& intPtrVectors, std::deque<float>& floatData, const std::string& name, int allocAmount, int deallocAmount)
	{
		if (allocAmount <= 0U || deallocAmount <= 0U)
		{
			return;
		}

		std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
		std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

		for (std::uint32_t i = 0; i < numObjectsToDeallocate; ++i)
		{
			if (!intPtrVectors.empty())
			{
				std::uint32_t* intPtr = intPtrVectors.back();
				intPtrVectors.pop_back();
				mMemoryManager->Deallocate(intPtr, name, CustomMemoryManager::MemoryManager::AllocType::STACK);
				++(mData.find(name)->second.mNumDeallocations);
			}
		}

		CustomMemoryManager::Allocators::StackAllocator* stackAlloc = static_cast<CustomMemoryManager::Allocators::StackAllocator*>(mMemoryManager->Get(name, CustomMemoryManager::MemoryManager::AllocType::STACK));
		std::size_t x = (stackAlloc->StackSize_Bytes() - stackAlloc->UsedSpace_Bytes()) / sizeof(std::uint32_t) + 1;
		if (numObjectsToAllocate >= x)
		{
			numObjectsToAllocate = x;
		}

		Library::StopWatch stopWatch;
		for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
		{
			stopWatch.Start();
			intPtrVectors.push_back(static_cast<std::uint32_t*>(mMemoryManager->Allocate(sizeof(std::uint32_t), name, CustomMemoryManager::MemoryManager::AllocType::STACK)));
			stopWatch.Stop();
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

	void MemoryGui::EndGui()
	{

	}
}