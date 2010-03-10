#ifndef HUD_H_
#define HUD_H_

#include "../system/graphic/VideoManager.h"

class HUD {
private:
	VideoManager* m_videoManager;
	vector<TextObject*> m_messages;
public:
	HUD(VideoManager* videoManager);
	void draw();
	void addMessage(std::string message);
	void reset();
	float Health;
	float Experience;
	float Ammo;
	int Grenades;
	int LevelUpPoints;
	int Time;
	~HUD();
};

#endif /* HUD_H_ */
