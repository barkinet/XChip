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

#ifndef _XCHIP_SDLSOUND_H_
#define _XCHIP_SDLSOUND_H_

#include <XChip/Plugins/iSound.h>
#include <XChip/Utility/StdintDef.h>




struct SDL_AudioSpec;
typedef uint32_t SDL_AudioDeviceID;

namespace xchip {


class SdlSound final : public iSound
{
	static constexpr const char* const PLUGIN_NAME = "SdlSound";
	static constexpr const char* const PLUGIN_VER = "1.0";
	static constexpr float defaultFreq = 450;
public:
	SdlSound() noexcept;
	~SdlSound();

	bool Initialize() noexcept override;
	void Dispose() noexcept override;
	bool IsInitialized() const noexcept override;
	const char* GetPluginName() const noexcept override;
	const char* GetPluginVersion() const noexcept override;
	PluginDeleter GetPluginDeleter() const noexcept override;
	bool IsPlaying() const noexcept override;
	float GetCountdownFreq() const noexcept override;	
	float GetSoundFreq() const noexcept override;
	void SetCountdownFreq(const float hertz) noexcept override;
	void SetSoundFreq(const float hz) noexcept override;
	void Play(const uint8_t soundTimer) noexcept override;
	void Stop() noexcept override;



private:
	bool InitDevice(SDL_AudioSpec& want, SDL_AudioSpec& have);
	float GetPlayFreq() const;
	float GetCurFreq() const;
	void SetPlayFreq(const float hz);
	void SetCurFreq(const float hz);
	void SetCycleTime(const float hz);
	void SetLenght(const unsigned int len);
	template<class T>
	static void audio_callback(void* userdata, uint8_t* stream, int len) noexcept;


	SDL_AudioSpec* _specs = nullptr;
	SDL_AudioDeviceID _dev = 0;
	float _cycleTime;
	float _playFreq;
	float _curFreq;
	float _len;
	unsigned int _pos;
	int _amplitude;
	bool _initialized = false;
	enum SpecsID { Want, Have };
};

























}
#endif