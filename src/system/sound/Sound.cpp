#include "Sound.h"

Sound::Sound(Mix_Chunk* sndRef, bool enabled) {
	m_sndRef = sndRef;
	m_enabled = enabled;
	m_chan = -1;
}

bool Sound::isPlaying() {
	return !(m_chan == -1 || Mix_Playing(m_chan) == 0) && m_enabled;
}

void Sound::play(int fade, int loops) {
	if (m_enabled) {
		if (isPlaying())
			stop(0);

		if (fade == 0) {
			m_chan = Mix_PlayChannel(-1, m_sndRef, loops);
		} else
			m_chan = Mix_FadeInChannel(-1, m_sndRef, loops, fade);
	}
}

void Sound::playInf() {
	if (m_enabled)
		m_chan = Mix_PlayChannel(-1, m_sndRef, 0);
}

void Sound::setPos(Sint16 angle, Uint8 distance) {
	if (m_enabled)
		Mix_SetPosition(m_chan, angle, distance);
}

void Sound::stop(int fade) {
	if (m_enabled) {
		if (fade == 0)
			Mix_HaltChannel(m_chan);
		else
			Mix_FadeOutChannel(m_chan, fade);
	}
}

void Sound::setVol(int value) {
	if (m_chan != -1)
		Mix_Volume(m_chan, value);
}

Sound::~Sound() {
	if (m_sndRef)
		Mix_FreeChunk(m_sndRef);
}
