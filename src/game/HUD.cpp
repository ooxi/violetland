#include "HUD.h"

HUD::HUD(VideoManager* videoManager) {
	m_videoManager = videoManager;
	reset();
}

void HUD::draw() {
	if (!m_messages.empty()) {
		int s = m_messages.size();
		for (int i = s - 1; i >= 0; i--) {
			m_messages[i]->draw(true, m_messages[i]->X
					+ m_videoManager->SmallText->getIndent(),
					m_videoManager->getVideoMode().Height - s
							* m_videoManager->SmallText->getHeight() + i
							* m_videoManager->SmallText->getHeight());
			m_messages[i]->AMask -= 0.0001f
					* m_videoManager->getFrameDeltaTime();

			if (m_messages[i]->AMask <= 0) {
				delete m_messages[i];
				m_messages.erase(m_messages.begin() + i);
			}
		}
	}
}

void HUD::addMessage(std::string message) {
	m_messages.push_back(m_videoManager->SmallText->getObject(message.c_str(),
			0, 0, TextManager::LEFT, TextManager::MIDDLE));
}

void HUD::reset() {
	clearVector<TextObject*> (&m_messages);
}

HUD::~HUD() {
	reset();
}
