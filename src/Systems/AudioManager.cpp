#include "AudioManager.h"
#include "raylib.h"

AudioManager::AudioManager() {
    SaveSystem::LoadSettings(masterVolume, sfxVolume, musicVolume);
    SetMasterVolume(masterVolume);
}

void AudioManager::UpdateVolumes() {
    SetMasterVolume(masterVolume);
    SaveSystem::SaveSettings(masterVolume, sfxVolume, musicVolume);
}

// Getters
float AudioManager::GetMasterVolume() const { return masterVolume; }
float AudioManager::GetSFXVolume() const { return sfxVolume; }
float AudioManager::GetMusicVolume() const { return musicVolume; }

// Setters
void AudioManager::SetMasterVolume(float volume) { 
    masterVolume = volume; 
    ::SetMasterVolume(masterVolume);
}
void AudioManager::SetSFXVolume(float volume) { sfxVolume = volume; }
void AudioManager::SetMusicVolume(float volume) { musicVolume = volume; }