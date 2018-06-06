#include "MemoryGui.h"
#include "MemoryManager.h"
#include "MemPtr.h"

namespace CustomMemoryManager
{
	void MemoryGui::RunGui()
	{
		ImGui::Text("Hello from the Memory Gui!");
		ImGui::Checkbox("Stack Windoww", &mShowStackWindow);

		StackGuiWindow();
	}

	void MemoryGui::StackGuiWindow()
	{
		if (!mShowStackWindow)
		{
			return;
		}

		ImGui::Begin("Stack Window");

		ImGui::Text("What's Up!");
		//for (int i = 0; i < 4; ++i)
		//{
		//	data[i] += 0.005f;
		//}
		ImGui::SliderFloat("Stack Slider", &amount, 0, 200);
		for (int i = 0; i < 4; ++i)
		{
			data[i] = amount;
		}
		for (int i = 0; i < 200; ++i)
		{
			if (i <= amount)
				data2[i] = 200;
			else
				data2[i] = 0;
		}
		ImGui::PlotHistogram("Histogram", data2, 200, 0, "DATATATA", 0, 200, ImVec2(0, 100));
		ImGui::NewLine();
		ImGui::PlotLines("Stack Graph", data, 3, 0, "More Data", 0, 200, ImVec2(0, 200));
		ImGui::NewLine();

		if (mMemoryManager != nullptr)
		{
			Allocators::StackAllocator* stack = static_cast<Allocators::StackAllocator*>(mMemoryManager->Get("Stack1", MemoryManager::AllocType::STACK));
			if (stack != nullptr)
			{
				ImGui::Text("Stack Size (Bytes): %u", stack->StackSize_Bytes());
				ImGui::Text("Stack Size (Num Objects): %i", stack->StackSize_NumObjects());
				MemPtr<std::uint32_t> memPtr(nullptr);
				if (ImGui::Button("Alloc a std::uint32_t"))
				{
					memPtr.SetPtr(MakeMemPtr_Raw<std::uint32_t>("Stack1", MemoryManager::AllocType::STACK, *mMemoryManager));
				}
				if (ImGui::Button("DeAlloc a std::uint32_t"))
				{
					stack->Clear();
				}
			}
		}

		{
			//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
			//ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiSetCond_Always);
			ImGui::Begin("Stack View");// , NULL,
				//ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing
				//| ImGuiWindowFlags_NoTitleBar);
			ImGui::Columns(4, NULL);
			ImGui::Separator();
			for (int i = 0; i < 16; ++i)
			{
				if (i > 0 && i % 4 == 0) ImGui::Separator();
				//ImGui::Button("Hello", ImVec2(ImGui::GetWindowSize().x / 4, ImGui::GetWindowSize().x / 4));
				//ImGui::SmallButton("Hello");
				ImGui::ColorButton("", ImVec4(0, 0, 1, 1), 0, ImVec2(ImGui::GetWindowSize().x / 4, ImGui::GetWindowSize().x / 4));
				//ImGui::TextColored(ImVec4(1,0,0,1),"Hello");
				ImGui::NextColumn(); 
				//ImGui::LogText("GoodBye");
			}
			ImGui::Columns(1);
			ImGui::Separator();
			if (ImGui::Button("Close"))
			{

			}
			ImGui::End();
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