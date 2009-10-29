#include "Sound.h"

Sound::Sound(Mix_Chunk* sndRef, bool enabled) {
	m_sndRef = sndRef;
	m_enabled = enabled;
	m_chan = -1;
}

bool Sound::isPlaying() {
	return !(m_chan == -1 || Mix_Playing(m_chan) == 0) && m_enabled;
}

void Sound::play() {
	if (m_enabled) {
		if (isPlaying())
			stop();

		m_chan = Mix_PlayChannel(-1, m_sndRef, 0);
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

void Sound::stop() {
	if (m_enabled)
		Mix_HaltChannel(m_chan);
}

Sound::~Sound() {
	if (m_sndRef)
		Mix_FreeChunk(m_sndRef);
}
