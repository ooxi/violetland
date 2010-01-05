#ifndef MONSTERFACTORY_H_
#define MONSTERFACTORY_H_

#include "../system/graphic/Sprite.h"
#include "../system/utility/FileUtility.h"

class MonsterFactory {
private:
	FileUtility* m_fileUtility;
public:
	MonsterFactory(FileUtility* fileUtility);
	~MonsterFactory();
	Sprite* loadMonsterSprite(unsigned int index, string animType);
	vector<Sprite*> MoveSprites;
	vector<Sprite*> DeathSprites;
};

#endif /* MONSTERFACTORY_H_ */
