#include "Sound.h"

Sound::Sound(Mix_Chunk* sndRef, bool enabled) {
	m_sndRef = sndRef;
	m_enabled = enabled;
	m_chan = -1;
}

bool Sound::isPlaying() {
	return m_chan != -1 && Mix_Playing(m_chan) != 0 && m_enabled;
}

void Sound::play(int chan, int fade, int loops) {
	if (m_enabled) {
		if (isPlaying())
			stop(0);

		m_chan = Mix_PlayChannel(chan, m_sndRef, loops);

		//		if (fade == 0) {
		//			m_chan = Mix_PlayChannel(-1, m_sndRef, loops);
		//		} else
		//			m_chan = Mix_FadeInChannel(-1, m_sndRef, loops, fade);
	}
}

void Sound::playInf(int chan) {
	if (m_enabled) {
		if (isPlaying())
			stop(0);

		m_chan = Mix_PlayChannel(chan, m_sndRef, 0);
	}
}

void Sound::setPos(Sint16 angle, Uint8 distance) {
	if (m_enabled && m_chan != -1)
		Mix_SetPosition(m_chan, angle, distance);
}

void Sound::stop(int fade) {
	if (m_enabled && m_chan != -1) {
		Mix_HaltChannel(m_chan);

		//		if (fade == 0)
		//			Mix_HaltChannel(m_chan);
		//		else
		//			Mix_FadeOutChannel(m_chan, fade);

		m_chan = -1;
	}
}

void Sound::setVol(int value) {
	if (m_chan != -1)
		Mix_Volume(0, value);
}

Sound::~Sound() {
	if (m_sndRef)
		Mix_FreeChunk(m_sndRef);
}
