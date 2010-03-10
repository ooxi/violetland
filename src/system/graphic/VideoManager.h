#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "../Configuration.h"
#include "Camera.h"
#include "text/TextManager.h"
#include "../utility/Templates.h"

class VideoManager {
private:
	int m_framesCount;
	int m_fpsCountingStart;
	int m_fps;
	int m_lastFrameTime;
	int m_frameDeltaTime;
	FileUtility* m_fileUtility;
	VideoMode m_videoMode;
public:
	VideoManager(FileUtility* fileUtility);
	bool isModeAvailable(int w, int h, int bpp, bool fullscreen, int* true_bpp);
	vector<SDL_Rect> GetAvailableModes();
	void setMode(VideoMode mode, Camera* cam);
	void countFrame(int frameDelay);
	int getFps();
	int getFrameDeltaTime();
	VideoMode getVideoMode();
	float WK;
	float HK;
	TextManager* RegularText;
	TextManager* SmallText;
	~VideoManager();
};

#endif /* VIDEOMANAGER_H_ */
