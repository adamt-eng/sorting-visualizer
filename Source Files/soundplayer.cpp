#include "../Header Files/soundplayer.h"
#include <QFile>

const float TWO_PI = 2.0f * static_cast<float>(M_PI);

SoundPlayer::SoundPlayer()
{
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec desiredSpec;
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_F32;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = AudioCallback;
    desiredSpec.userdata = this;

    if (SDL_OpenAudio(&desiredSpec, nullptr) < 0)
    {
        initialized = false;
    }
    else
    {
        initialized = true;
        SDL_PauseAudio(0);
    }
}

SoundPlayer::~SoundPlayer()
{
    if (audioDevice != 0)
    {
        SDL_CloseAudioDevice(audioDevice);
    }

    if (audioBuffer != nullptr)
    {
        SDL_FreeWAV(audioBuffer);
    }

    SDL_CloseAudio();
    SDL_Quit();
}

bool SoundPlayer::loadWAV(const std::string& filePath)
{
    if (!initialized) return false;

    if (audioBuffer != nullptr)
    {
        SDL_FreeWAV(audioBuffer);
        audioBuffer = nullptr;
    }

    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray fileData = file.readAll();
    SDL_RWops* rw = SDL_RWFromConstMem(fileData.data(), fileData.size());
    if (!rw || SDL_LoadWAV_RW(rw, 1, &wavSpec, &audioBuffer, &audioLength) == nullptr)
    {
        return false;
    }

    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
    if (audioDevice == 0)
    {
        SDL_FreeWAV(audioBuffer);
        audioBuffer = nullptr;
        return false;
    }

    // No issues
    return true;
}

void SoundPlayer::playSound()
{
    if (!initialized || audioBuffer == nullptr || audioDevice == 0) return;

    SDL_ClearQueuedAudio(audioDevice);
    SDL_QueueAudio(audioDevice, audioBuffer, audioLength);
    SDL_PauseAudioDevice(audioDevice, 0);
}

void SoundPlayer::stopSound()
{
    this->frequency = 0;
}

void SoundPlayer::AudioCallback(void* userdata, Uint8* stream, int len)
{
    SoundPlayer* player = reinterpret_cast<SoundPlayer*>(userdata);
    float* buffer = reinterpret_cast<float*>(stream);
    int samples = len / sizeof(float);

    if (player->isPlaying && player->frequency > 0)
    {
        for (int i = 0; i < samples; ++i)
        {
            // Make triangular wave
            float triangleWave = 2.0f * fabsf((player->phase / (TWO_PI)) - 0.5f) - 1.0f;

            // Apply ADSR envelope
            float amplitude = 0.0f;
            if (player->envelopeTime < player->attackTime)
            {
                // Attack phase
                amplitude = (player->envelopeTime / player->attackTime);
            }
            else if (player->envelopeTime < player->attackTime + player->decayTime)
            {
                // Decay phase
                float decayProgress = (player->envelopeTime - player->attackTime) / player->decayTime;
                amplitude = 1.0f - (1.0f - player->sustainLevel) * decayProgress;
            }
            else if (player->envelopeTime < player->attackTime + player->decayTime + player->sustainDuration)
            {
                // Sustain phase
                amplitude = player->sustainLevel;
            }
            else if (player->envelopeTime < player->attackTime + player->decayTime + player->sustainDuration + player->releaseTime)
            {
                // Release phase
                float releaseProgress = (player->envelopeTime - player->attackTime - player->decayTime - player->sustainDuration) / player->releaseTime;
                amplitude = player->sustainLevel * (1.0f - releaseProgress);
            }
            else
            {
                // End of envelope
                player->isPlaying = false;
                amplitude = 0.0f;
            }

            // Increment envelope time
            player->envelopeTime += 1.0f / 44100.0f;

            // Update current amplitude
            player->amplitude = amplitude;

            // Mix amplitude with the generated wave
            buffer[i] = triangleWave * player->amplitude * 0.5f;

            // Increment phase
            player->phase += player->phaseIncrement;
            if (player->phase > TWO_PI)
            {
                player->phase -= TWO_PI;
            }
        }
    }
    else
    {
        SDL_memset(stream, 0, len); // Silence if not playing
    }
}

void SoundPlayer::playFrequencyWithEnvelope(float frequency, float attack, float decay, float sustain, float sustainDur, float release)
{
    this->frequency = frequency;
    this->phaseIncrement = (TWO_PI * frequency) / 44100.0f;
    this->attackTime = attack;
    this->decayTime = decay;
    this->sustainLevel = sustain;
    this->sustainDuration = sustainDur;
    this->releaseTime = release;
    this->amplitude = 0.0f;
    this->envelopeTime = 0.0f;
    this->isPlaying = true;
}
