#pragma once
#include "../Systems/SaveSystem.h"

class AudioManager {
private:
    float masterVolume;
    float sfxVolume;
    float musicVolume;
    
public:
    AudioManager();
    void UpdateVolumes();
    
    float GetMasterVolume() const;
    float GetSFXVolume() const;
    float GetMusicVolume() const;
    
    void SetMasterVolume(float volume);
    void SetSFXVolume(float volume);
    void SetMusicVolume(float volume);
};