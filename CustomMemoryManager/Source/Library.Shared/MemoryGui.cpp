#include "MemoryGui.h"
#include "MemoryManager.h"
#include "MemPtr.h"

namespace CustomMemoryManager
{
	void MemoryGui::RunGui()
	{
		ImGui::Text("Hello from the Memory Gui!");

		if (mMemoryManager != nullptr)
		{
			ImGui::Checkbox("Show Stacks Window", &mShowStackWindows);
			std::vector<std::string> stackNames = mMemoryManager->Get(MemoryManager::AllocType::STACK);
			for (const std::string& name : stackNames)
			{
				std::string text = "\t\t" + name + " Window";
				ImGui::Text(text.c_str());
			}
		}

		StackGuiWindow();
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

					//MemPtr<std::uint32_t> memPtr(nullptr);
					if (ImGui::Button(std::string(name + "Alloc a std::uint32_t").c_str()))
					{
						std::uint32_t* addr = static_cast<std::uint32_t*>(stack->allocate(sizeof(std::uint32_t)));
						*addr = 4;
						//memPtr.SetPtr(MakeMemPtr_Raw<std::uint32_t>(name, MemoryManager::AllocType::STACK, *mMemoryManager));
#ifdef _DEBUG
						if (stack->IsStackOverflow())
							mStackOverflowList.emplace(std::move(name));
#endif //_DEBUG
					}
					if (ImGui::Button(std::string(name + "DeAlloc a std::uint32_t").c_str()))
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

	void MemoryGui::EndGui()
	{

	}

	void MemoryGui::SetMemoryManager(MemoryManager& manager)
	{
		mMemoryManager = &manager;
	}
}