#include "MemoryGui.h"

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

		ImGui::End();
	}

	void MemoryGui::EndGui()
	{

	}
}