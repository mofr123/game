#ifndef __mir_mobile__GameLoading__
#define __mir_mobile__GameLoading__

#include "publicDef/PublicDef.h"

class GameLoading: public Sprite//, public CCTargetedTouchDelegate
{
	int m_mapID;
    
    int m_born;
    
    Sprite* m_sprite;
    
public:
    
    GameLoading(const int mapID, const int born);
    
    virtual ~GameLoading();
    
    static GameLoading* runGameLoading(int mapID, const int born);

    void setOpacity(GLubyte opacity);
    
protected:
    
    void onEnter();
    
    void onExit();
    
    bool init();
    
    void initGameScene(float delay);
	
	void updateBgMap(float delay); 
	
	void updateFinish();
	
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
};

#endif /* defined(__mir_mobile__GameLoading__) */
