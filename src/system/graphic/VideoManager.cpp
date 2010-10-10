#include "VideoManager.h"

VideoManager::VideoManager(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	printf("SDL_GL_SetAttribute SDL_GL_DOUBLEBUFFER...\n");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	RegularText = NULL;
	SmallText = NULL;

	m_lastFrameTime = m_fpsCountingStart = SDL_GetTicks();
	m_framesCount = 0;

	// Seems that this code is supported only in windows.
	// printf("SDL_GL_SetAttribute SDL_GL_SWAP_CONTROL...\n");
	// SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
}

void VideoManager::countFrame(int frameDelay) {
	m_framesCount++;

	int now = SDL_GetTicks();

	m_frameDeltaTime = now - m_lastFrameTime;
	m_lastFrameTime = now;

	if (now - m_fpsCountingStart > 5000) {
		m_fpsCountingStart = now;
		m_fps = m_framesCount / 5;
		m_framesCount = 0;
	}

	if (frameDelay > 0 && m_frameDeltaTime < frameDelay)
		SDL_Delay(frameDelay - m_frameDeltaTime);
}

int VideoManager::getFps() {
	return m_fps;
}

int VideoManager::getFrameDeltaTime() {
	return m_frameDeltaTime;
}

bool VideoManager::isModeAvailable(int w, int h, int bpp, bool fullscreen,
		int* true_bpp) {
	Uint32 flags = SDL_OPENGL;
	if (fullscreen)
		flags = flags | SDL_FULLSCREEN;
	int r = SDL_VideoModeOK(w, h, bpp, flags);
	if (true_bpp)
		*true_bpp = r;
	return (r != 0);
}

std::vector<SDL_Rect> VideoManager::GetAvailableModes() {
	/* This way is better than SDL_ListModes because of
	 * SDL_ListModes returns not all possible modes
	 */

	// Number of possible modes
	int wL[] = { 400, 640, 800, 1024, 1280, 1280, 1280, 1280, 1366, 1600, 1600,
			1680, 1920, 1920 };
	int hL[] = { 300, 480, 600, 768, 720, 768, 800, 1024, 768, 900, 1200, 1050,
			1080, 1200 };

	// If the mode is supported, it will be added to the return list
	std::vector<SDL_Rect> modes;
	for (unsigned int i = 0; i < getStructSize(wL); i++) {
		if (isModeAvailable(wL[i], hL[i], 16, true, NULL)) {
			SDL_Rect r;
			r.w = wL[i];
			r.h = hL[i];
			modes.push_back(r);
		}
	}

	return modes;
}

VideoMode VideoManager::getVideoMode() {
	return m_videoMode;
}

void VideoManager::setMode(VideoMode mode, Camera* cam) {
	fprintf(stdout, "SDL_SetVideoMode %ix%i (%c)...\n", mode.Width,
			mode.Height, mode.Full ? 'f' : 'w');

	m_videoMode = mode;

	SDL_Surface *screen = SDL_SetVideoMode(mode.Width, mode.Height, mode.Color,
			mode.Full ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL);

	float aspect = (float) mode.Width / mode.Height;
	cam->setH((int) (cam->getW() / aspect));
	WK = (float) mode.Width / cam->getW();
	HK = (float) mode.Height / cam->getH();
	Scale = (float) mode.Width / 800;

	if (screen == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(2);
	}

	printf("glViewport...\n");
	glViewport(0, 0, mode.Width, mode.Height);

	if (RegularText != NULL) {
		delete RegularText;
	}
	if (SmallText != NULL) {
		delete SmallText;
	}

	printf("Preparing fonts...\n");

	RegularText = new TextManager(m_fileUtility->getFullPath(
			FileUtility::common, "fonts/archangelsk.ttf"), 46 * WK);

	SmallText = new TextManager(m_fileUtility->getFullPath(FileUtility::common,
			"fonts/archangelsk.ttf"), 24 * WK);
}

VideoManager::~VideoManager() {
	delete RegularText;
	delete SmallText;
}
