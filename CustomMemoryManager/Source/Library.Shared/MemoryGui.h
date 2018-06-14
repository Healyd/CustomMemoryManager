#pragma once
#include "imgui-1.61/imgui.h"
#include <cstdint>
#include <unordered_map>
#include <set>

namespace CustomMemoryManager
{
	class MemoryManager;

	class MemoryGui final
	{
	public:
		MemoryGui() = default;
		MemoryGui(const MemoryGui&) = delete;
		MemoryGui(MemoryGui&&) = delete;
		MemoryGui& operator=(const MemoryGui&) = delete;
		MemoryGui& operator=(MemoryGui&&) = delete;

		void InitializeGui();
		void RunGui();
		void EndGui();

		void SetMemoryManager(MemoryManager& manager);

	private:
		void StackGuiWindow();
		// Window Bools
		bool mShowStackWindows{ false };

#ifdef _DEBUG
		std::set<std::string> mStackOverflowList;
#endif // _DEBUG

		float data[4] = { 1, 1, 1, 1 };
		float data2[200];
		float amount{ 0 };
		std::uint32_t mUsedBytes_Stack{ 0 };

		MemoryManager* mMemoryManager{ nullptr };
	};
}