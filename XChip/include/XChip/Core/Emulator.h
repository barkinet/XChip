/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#ifndef _XCHIP_EMULATOR_H_
#define _XCHIP_EMULATOR_H_
#include <string>

#include "CpuManager.h"
#include <XChip/Media/Interfaces.h>
#include <XChip/Media/UniqueMediaPlugin.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Assert.h>

 

namespace xchip {
namespace instructions { extern void execute_instruction(CpuManager&); }
using UniqueRender = UniqueMediaPlugin<iRender>;
using UniqueInput = UniqueMediaPlugin<iInput>;
using UniqueSound = UniqueMediaPlugin<iSound>;





class Emulator
{
public:
	Emulator() noexcept;
	~Emulator();
	Emulator(const Emulator&) = delete;
	Emulator& operator=(const Emulator&) = delete;

	bool Initialize(UniqueRender render, UniqueInput input, UniqueSound sound) noexcept;

	void Dispose() noexcept;
	bool IsInitialized() const;
	bool GetInstrFlag() const;
	bool GetDrawFlag() const;
	bool GetExitFlag() const;
	void HaltForNextFlag() const;
	int GetCpuFreq() const;
	int GetFps() const;


	void UpdateSystems();
	void ExecuteInstr();
	void CleanFlags();
	void Draw();
	void Reset();

	void SetDrawFlag(const bool val);
	void SetExitFlag(const bool val);
	void SetCpuFreq(const int value);
	void SetFps(const int value);
	bool LoadRom(const std::string& fileName);

	bool SetRender(UniqueRender rend);
	bool SetInput(UniqueInput input);
	bool SetSound(UniqueSound sound);

	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();
	
	UniqueRender SwapRender(UniqueRender rend = nullptr);
	UniqueInput SwapInput(UniqueInput input = nullptr);
	UniqueSound SwapSound(UniqueSound sound = nullptr);

private:
 	void UpdateTimers();
	bool InitRender();
	bool InitInput();
	bool InitSound();

	CpuManager _manager;
	utility::Timer _instrTimer;
	utility::Timer _frameTimer;
	utility::Timer _chDelayTimer;
	bool _instrf = false;
	bool _drawf  = false;
	bool _exitf  = true;
	bool _initialized = false;
};





inline bool Emulator::IsInitialized() const { return _initialized; }
inline bool Emulator::GetInstrFlag() const { return _instrf; }
inline bool Emulator::GetDrawFlag() const { return _drawf; }
inline bool Emulator::GetExitFlag() const { return _exitf || _manager.GetFlags(Cpu::EXIT); }


inline int Emulator::GetCpuFreq() const {
	using namespace utility::literals;
	return static_cast<int>(1_sec / _instrTimer.GetTarget());
}

inline int Emulator::GetFps() const {
	using namespace utility::literals;
	return static_cast<int>(1_sec / _frameTimer.GetTarget());
}


inline void Emulator::SetDrawFlag(const bool val) { _drawf = val; }
inline void Emulator::SetExitFlag(const bool val) { _exitf = val; }

inline void Emulator::SetCpuFreq(const int value) { 
	_instrTimer.SetTargetTime(utility::literals::operator""_hz(value > 0 ? value : 1));
}
inline void Emulator::SetFps(const int value) { 
	_frameTimer.SetTargetTime(utility::literals::operator""_hz(value > 0 ? value : 1));
}


inline bool Emulator::LoadRom(const std::string& fname) { return _manager.LoadRom(fname.c_str(), 0x200); }
inline iRender* Emulator::GetRender() { return _manager.GetRender(); }
inline iInput* Emulator::GetInput() { return _manager.GetInput(); }
inline iSound* Emulator::GetSound() { return _manager.GetSound(); }


inline void Emulator::ExecuteInstr()
{
	instructions::execute_instruction(_manager);
	_instrf = false;
}


inline void Emulator::Draw()
{
	ASSERT_MSG(_manager.GetRender() != nullptr, "null render!");

	_manager.GetRender()->DrawBuffer();
	_drawf = false;
}









}
#endif
