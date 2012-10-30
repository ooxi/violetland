#include "VideoManager.h"

VideoManager::VideoManager(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	std::cout << "SDL_GL_SetAttribute SDL_GL_DOUBLEBUFFER..." << std::endl;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	RegularText = NULL;
	SmallText = NULL;

	m_lastFrameTime = m_fpsCountingStart = SDL_GetTicks();
	m_framesCount = 0;

	// Seems that this code is supported only in windows.
	// std::cout << "SDL_GL_SetAttribute SDL_GL_SWAP_CONTROL..." << std::endl;
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

	/* All possible width
	 */
	#define EXPAND_VIDEO_MODE(name, width, height)			\
		width,

	static int wL[] = {
		#include "VideoModes.h"
	};
	#undef EXPAND_VIDEO_MODE

	/* All possible height
	 */
	#define EXPAND_VIDEO_MODE(name, width, height)			\
		height,

	static int hL[] = {
		#include "VideoModes.h"
	};
	#undef EXPAND_VIDEO_MODE


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

void VideoManager::setMode(VideoMode mode, Camera* cam) {
	std::cout << "SDL_SetVideoMode " << mode.Width << 'x' << mode.Height << '('
			<< (mode.Full ? 'f' : 'w') << ")..." << std::endl;

	m_videoMode = mode;

	SDL_Surface *screen = SDL_SetVideoMode(mode.Width, mode.Height, mode.Color,
			mode.Full ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL);

	std::cout << "Calculating aspect size..." << std::endl;

	cam->setAspect((float) mode.Width / mode.Height);
	WK = (float) mode.Width / cam->getW();
	HK = (float) mode.Height / cam->getH();
	Scale = (float) mode.Width / 800;

	if (screen == NULL) {
		std::cerr << "Couldn't set video mode: " << SDL_GetError() << std::endl;
		exit(2);
	}

	std::cout << "glViewport..." << std::endl;
	glViewport(0, 0, mode.Width, mode.Height);

	if (RegularText != NULL) {
		delete RegularText;
	}
	if (SmallText != NULL) {
		delete SmallText;
	}

	std::cout << "Preparing fonts..." << std::endl;
	boost::filesystem::path fontPath = m_fileUtility->getFullPath(
			FileUtility::common, "fonts/archangelsk.ttf");

	RegularText = new TextManager(fontPath, 46 * WK);
	SmallText = new TextManager(fontPath, 30 * WK);
}

VideoManager::~VideoManager() {
	delete RegularText;
	delete SmallText;
}
