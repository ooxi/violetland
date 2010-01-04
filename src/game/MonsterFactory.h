#ifndef MONSTERFACTORY_H_
#define MONSTERFACTORY_H_

#include "../system/graphic/Sprite.h"
#include "../system/utility/FileUtility.h"

class MonsterFactory {
public:
	MonsterFactory(FileUtility* fileUtility);
	~MonsterFactory();
	vector<Sprite*> MoveSprites;
	vector<Sprite*> DeathSprites;
};

#endif /* MONSTERFACTORY_H_ */
