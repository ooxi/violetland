#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_

#include "../Configuration.h"
#include "Camera.h"
#include "text/TextManager.h"

class VideoManager {
private:
	FileUtility* m_fileUtility;
public:
	VideoManager(FileUtility* fileUtility);
	bool isModeAvailable(int w, int h, int bpp, bool fullscreen, int* true_bpp);
	vector<SDL_Rect> GetAvailableModes(Configuration* config);
	void setMode(Configuration* config, Camera* cam);
	float WK;
	float HK;
	TextManager* RegularText;
	TextManager* SmallText;
	~VideoManager();
};

#endif /* VIDEOMANAGER_H_ */
