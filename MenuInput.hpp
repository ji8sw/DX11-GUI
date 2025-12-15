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
		unsigned int VK;
		bool LastState = false;
		bool BlockGameInputWhenMenuOpen = false;

		// Repeat handling
		int RepeatCount = 0;
		int RepeatDelay = 400;             // milliseconds
		static constexpr int InitialDelay = 400;      // first delay before repeat
		static constexpr int MinDelay = 50;           // fastest repeat speed
		static constexpr int AccelerationStep = 40;   // decrease delay each repeat
		std::chrono::steady_clock::time_point LastTime;

		Hotkey(unsigned int InVK, bool InBlockGameInputWhenMenuOpen = false)
			: VK(InVK), BlockGameInputWhenMenuOpen(InBlockGameInputWhenMenuOpen)
		{
		}
		Hotkey() = default;

		bool IsDown() const
		{
			return (GetAsyncKeyState(VK) & 0x8000) != 0;
		}

		bool WasReleased()
		{
			bool CurrentState = IsDown();
			bool Pressed = !CurrentState && LastState;
			LastState = CurrentState;
			return Pressed;
		}

		bool IsRepeating() 
		{
			using namespace std::chrono;
			auto Now = steady_clock::now();

			if (!IsDown()) 
			{
				RepeatCount = 0;
				RepeatDelay = InitialDelay;
				return false;
			}

			if (RepeatCount == 0) 
			{
				// First repeat happens after initial delay
				if (duration_cast<milliseconds>(Now - LastTime).count() >= RepeatDelay) 
				{
					LastTime = Now;
					RepeatCount++;
					// accelerate by decreasing delay
					RepeatDelay = (std::max)(MinDelay, RepeatDelay - AccelerationStep);
					return false;
				}
			}
			else 
			{
				// Subsequent repeats
				if (duration_cast<milliseconds>(Now - LastTime).count() >= RepeatDelay) 
				{
					LastTime = Now;
					RepeatCount++;
					RepeatDelay = (std::max)(MinDelay, RepeatDelay - AccelerationStep);
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