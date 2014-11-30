#ifndef __mir_mobile__TextureController__
#define __mir_mobile__TextureController__

#include "publicDef/PublicDef.h"

typedef enum
{
	TexturePathFigure = 1,
	TexturePathMonster = 2,
	TexturePathHair = 3,
	TexturePathWeapon = 4,
	TexturePathSkillCaster = 5,
	TexturePathSkillLocus = 6,
	TexturePathSkillExplosion = 7
}TexturePathType;

class TextureController {

public:
	//已经是值传递，不用返回
	static string& getTextureRoute(string &path , TexturePathType type, unsigned int number);
	
	static string& getTexturePath(string &path , TexturePathType type, unsigned int number);
	
	static void addSpriteFrames(TexturePathType type, unsigned int number, Ref *target = NULL, SEL_CallFunc selector = NULL);
    
    static void subSpriteFrames(TexturePathType type, unsigned int number);

    static void removeAllSpriteFrames();

};

#endif /* defined(__mir_mobile__TextureController__) */
