#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include <cmath>

class SoundPlayer
{
public:
    SoundPlayer();
    ~SoundPlayer();

    bool loadWAV(const std::string& filePath);
    void playSound();

    void playFrequencyWithEnvelope(float frequency, float attack, float decay, float sustain, float sustainDur, float release);

private:
    static void AudioCallback(void* userdata, Uint8* stream, int len);

    bool initialized = false;
    SDL_AudioDeviceID audioDevice = 0;
    Uint8* audioBuffer = nullptr;
    Uint32 audioLength = 0;
    SDL_AudioSpec wavSpec;

    float frequency = 0.0f;
    float phaseIncrement = 0.0f;
    float phase = 0.0f;
    bool isPlaying = false;

    float envelopeTime = 0.0f;
    float attackTime = 0.0f;
    float decayTime = 0.0f;
    float sustainLevel = 0.0f;
    float sustainDuration = 0.0f;
    float releaseTime = 0.0f;
    float amplitude = 0.0f;
};

#endif // SOUNDPLAYER_H
