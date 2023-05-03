#ifndef audio
#define audio
#include <SDL_mixer.h>

#include "config.h"

class game_audio
{
private:
    Mix_Music* music = NULL;
    Mix_Chunk* sound = NULL;

    float sound_vol = VOLUME;
    float music_vol = VOLUME;

    bool sound_music = true; //false -> music

public:

    game_audio(){}
    game_audio(string path, bool sound_music, float volume) //volume percentage of MASTER VOLUME
    {
        Mix_Volume(-1,VOLUME);
        this->sound_music = sound_music;
        if(sound_music)
        {
            load_sound(path);
            sound_vol = volume;
        }
        else
        {
            load_music(path);
            music_vol = volume;
        }
    }
    ~game_audio(){}

    void load_music(string path)
    {
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        music = Mix_LoadMUS(path.c_str());
        if(music == NULL) cout << "Failed to load music" << endl;
    }

    void load_sound(string path)
    {
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        sound = Mix_LoadWAV(path.c_str());
        if(sound == NULL) cout << "Failed to load WAV" << endl;
    }

    void play()
    {
        if(sound_music)
        {
            Mix_VolumeChunk(sound,sound_vol/100*VOLUME*MIX_MAX_VOLUME/100);
            Mix_PlayChannel(-1,sound,0);
        }
        else
        {
            Mix_VolumeMusic(music_vol/100*VOLUME*MIX_MAX_VOLUME/100);
            Mix_PlayMusic(music,-1);
        }
    }

    void set_volume(float vol)
    {
        sound_vol = vol;
        music_vol = vol;
    }

    void pause_music()
    {
        Mix_PauseMusic();
    }

};
#endif // audio


