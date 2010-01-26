#include "VideoManager.h"

VideoManager::VideoManager(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	printf("SDL_GL_SetAttribute SDL_GL_DOUBLEBUFFER...\n");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	RegularText = NULL;
	SmallText = NULL;

	// seems that this code is supported only in windows
	// printf("SDL_GL_SetAttribute SDL_GL_SWAP_CONTROL...\n");
	// SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
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

#define ARRSIZE(s) (sizeof(s) / sizeof(*s))

/*
 i don't know what of these methods is better to use
 i even can create a static function to calculate size of array
 */

template<typename T> size_t structsize(const T& t) {
	return sizeof(t) / sizeof(*t);
}

vector<SDL_Rect> VideoManager::GetAvailableModes(Configuration* config) {
	int wL[] = { 400, 640, 800, 1024, 1280, 1280, 1280, 1280, 1600, 1600, 1680,
			1920, 1920 };
	int hL[] = { 300, 480, 600, 768, 720, 768, 800, 1024, 900, 1200, 1050,
			1080, 1200 };

	vector<SDL_Rect> modes;
	for (unsigned int i = 0; i < structsize(wL); i++) {
		if (isModeAvailable(wL[i], hL[i], config->ScreenColor, true, NULL)) {
			SDL_Rect r;
			r.w = wL[i];
			r.h = hL[i];
			modes.push_back(r);
		}
	}

	return modes;
}

void VideoManager::setMode(Configuration* config, Camera* cam) {
	fprintf(stdout, "SDL_SetVideoMode %ix%i (%c)...\n", config->ScreenWidth,
			config->ScreenHeight, config->FullScreen ? 'f' : 'w');

	SDL_Surface *screen = SDL_SetVideoMode(config->ScreenWidth,
			config->ScreenHeight, config->ScreenColor,
			config->FullScreen ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL);

	float aspect = (float) config->ScreenWidth / config->ScreenHeight;
	cam->setH(cam->getW() / aspect);
	WK = (float) config->ScreenWidth / cam->getW();
	HK = (float) config->ScreenHeight / cam->getH();

	if (screen == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(2);
	}

	printf("glViewport...\n");
	glViewport(0, 0, config->ScreenWidth, config->ScreenHeight);

	if (RegularText != NULL) {
		delete RegularText;
	}
	if (SmallText != NULL) {
		delete SmallText;
	}

	printf("Preparing fonts...\n");

	RegularText = new TextManager(m_fileUtility->getFullPath(
			FileUtility::common, "fonts/harabara.ttf"), 46 * WK);

	SmallText = new TextManager(m_fileUtility->getFullPath(FileUtility::common,
			"fonts/harabara.ttf"), 24 * WK);
}

VideoManager::~VideoManager() {
	delete RegularText;
	delete SmallText;
}
