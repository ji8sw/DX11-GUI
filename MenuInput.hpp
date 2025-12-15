#pragma once
#include <Windows.h>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <chrono>

class InputHandler
{
public:
	struct Hotkey
	{
		unsigned int m_VK;
		bool m_LastState = false;
		bool m_BlockGameInputWhenMenuOpen = false;

		// Repeat handling
		int m_RepeatCount = 0;
		int m_RepeatDelay = 400;             // milliseconds
		static constexpr int m_InitialDelay = 400;      // first delay before repeat
		static constexpr int m_MinDelay = 50;           // fastest repeat speed
		static constexpr int m_AccelerationStep = 40;   // decrease delay each repeat
		std::chrono::steady_clock::time_point m_LastTime;

		Hotkey(unsigned int VK, bool BlockGameInputWhenMenuOpen = false)
			: m_VK(VK), m_BlockGameInputWhenMenuOpen(BlockGameInputWhenMenuOpen)
		{
		}
		Hotkey() = default;

		bool IsDown() const
		{
			return (GetAsyncKeyState(m_VK) & 0x8000) != 0;
		}

		bool WasReleased()
		{
			bool CurrentState = IsDown();
			bool Pressed = !CurrentState && m_LastState;
			m_LastState = CurrentState;
			return Pressed;
		}

		bool IsRepeating() 
		{
			using namespace std::chrono;
			auto Now = steady_clock::now();

			if (!IsDown()) 
			{
				m_RepeatCount = 0;
				m_RepeatDelay = m_InitialDelay;
				return false;
			}

			if (m_RepeatCount == 0)
			{
				// First repeat happens after initial delay
				if (duration_cast<milliseconds>(Now - m_LastTime).count() >= m_RepeatDelay)
				{
					m_LastTime = Now;
					m_RepeatCount++;
					// accelerate by decreasing delay
					m_RepeatDelay = (std::max)(m_MinDelay, m_RepeatDelay - m_AccelerationStep);
					return false;
				}
			}
			else 
			{
				// Subsequent repeats
				if (duration_cast<milliseconds>(Now - m_LastTime).count() >= m_RepeatDelay)
				{
					m_LastTime = Now;
					m_RepeatCount++;
					m_RepeatDelay = (std::max)(m_MinDelay, m_RepeatDelay - m_AccelerationStep);
					return true;
				}
			}

			return false;
		}
	};

	std::unordered_map<std::string, Hotkey> Hotkeys;

	void RegisterHotkey(const std::string& Name, unsigned int VK, bool BlockGameInputWhenMenuOpen = false)
	{
		Hotkeys.emplace(Name, Hotkey(VK, BlockGameInputWhenMenuOpen));
	}

	void RegisterStandardHotkeys() // this layout uses arrow keys, F4, Enter, Backspace, right ctrl, and right shift
	{
		RegisterHotkey("Up", VK_UP, true);
		RegisterHotkey("Down", VK_DOWN, true);
		RegisterHotkey("Back", VK_LEFT, true);
		RegisterHotkey("Select", VK_RIGHT, true);
		RegisterHotkey("Select_Secondary", VK_RETURN, true);
		RegisterHotkey("Back_Secondary", VK_BACK, true);
		RegisterHotkey("TabUp", VK_RSHIFT, true);
		RegisterHotkey("TabDown", VK_RCONTROL, true);
		RegisterHotkey("MenuToggle", VK_F4, true);
	}
};