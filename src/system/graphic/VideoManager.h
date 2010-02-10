#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "../Configuration.h"
#include "Camera.h"
#include "text/TextManager.h"

#define ARRSIZE(s) (sizeof(s) / sizeof(*s))

/*
 i don't know what of these methods is better to use
 just c or c++
 */

template<typename T> size_t structsize(const T& t) {
	return sizeof(t) / sizeof(*t);
}

class VideoManager {
private:
	int m_framesCount;
	int m_fpsCountingStart;
	int m_fps;
	FileUtility* m_fileUtility;
public:
	VideoManager(FileUtility* fileUtility);
	bool isModeAvailable(int w, int h, int bpp, bool fullscreen, int* true_bpp);
	vector<SDL_Rect> GetAvailableModes(Configuration* config);
	void setMode(Configuration* config, Camera* cam);
	void countFrame();
	int getFps();
	float WK;
	float HK;
	TextManager* RegularText;
	TextManager* SmallText;
	~VideoManager();
};

#endif /* VIDEOMANAGER_H_ */
