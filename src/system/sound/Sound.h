#ifndef VIOLET_SOUND_H_
#define VIOLET_SOUND_H_

#include "SDL_mixer.h"

namespace violet {

class Sound {
private:
	bool m_enabled;
	int m_chan;
	Mix_Chunk* m_sndRef;
public:
	Sound(Mix_Chunk* sndRef, bool enabled);
	bool isPlaying();
	void play(int chan, int fade, int loop);
	void playInf(int chan);
	void setPos(Sint16 angle, Uint8 distance);
	void setVol(int value);
	void stop(int fade);
	~Sound();
};
}

#endif /* VIOLET_SOUND_H_ */
