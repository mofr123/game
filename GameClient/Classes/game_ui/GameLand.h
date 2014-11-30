#ifndef __mir_mobile__gameLand__
#define __mir_mobile__gameLand__

#include "publicDef/PublicDef.h"

class GameLand: public cocos2d::Layer,public EditBoxDelegate
{

	EditBox* m_accountInputBox;
	EditBox* m_cipherInputBox;

	virtual void editBoxReturn(EditBox* editBox);

public:
	static Scene* scene();

	GameLand();
	virtual ~GameLand();

	CREATE_FUNC(GameLand);
	bool init();


private:
	void sendLand(Ref* sender,Control::EventType controlEvent);

	void joinGame();

	void registration(Ref* sender,Control::EventType controlEvent);
};

#endif