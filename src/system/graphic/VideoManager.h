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

	// Test a video mode for availability on current hardware
	// w - screen width
	// h - screen height
	// bpp - color (bits per pixel)
	// fullscreen - true for fullscreen, false for window
	// true_bpp - returns true bpp (not sure what is it)
	bool isModeAvailable(int w, int h, int bpp, bool fullscreen, int* true_bpp);

	// Returns a list of available video modes on current hardware
	std::vector<SDL_Rect> GetAvailableModes();

	// Gets current video mode
	VideoMode getVideoMode();

	// Sets current video mode and modifies the camera for this mode
	void setMode(VideoMode mode, Camera* cam);

	// Should be executed at rendering of every frame to count FPS value
	/* frameDelay - a value in milliseconds for inserting as gap between
	 * frames. If frameDelay = 0 then no gap will be inserted.
	 */
	void countFrame(int frameDelay);

	// Average value of FPS for 5 seconds
	int getFps();

	// Time in milliseconds between the latest and the previous frames
	int getFrameDeltaTime();
	
	void resetDeltaTime();

	/* Width and height factors
	 * (for dealing with various aspect ratios)
	 */
	float WK;
	float HK;

	/* Scale factor which is counted as screen_width / 800
	 * (for dealing with HUD, etc)
	 */
	float Scale;

	// Big text
	TextManager* RegularText;

	// Small text
	TextManager* SmallText;

	~VideoManager();
};

#endif /* VIDEOMANAGER_H_ */
