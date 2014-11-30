#ifndef __mir9__MapThumbnailLayer__
#define __mir9__MapThumbnailLayer__

#include "publicDef/PublicDef.h"

class MapThumbnailLayer;
class MapThumbnailMenu:
public Sprite
//public CCTargetedTouchDelegate
{

public:
    
    MapThumbnailMenu();
    
    ~MapThumbnailMenu();
    
    static MapThumbnailMenu* create();
    
    bool initWithFile(const char *pszFilename);

    
    virtual void onEnter();
    
    virtual void onExit();
    
protected:
    
    Sprite* m_playerIndicator;
    
    Dictionary* m_enemyDic;
    
    vector<int> keyVec;
    
    Label* m_pCoordinateTTF;
    
protected:
    
    void update(float delay);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
};

class MapThumbnailScrollView;
class MapThumbnailLayer:
public Layer
{
    
public:
    
    CREATE_FUNC(MapThumbnailLayer);
    
    bool init();
    
protected:
    
    MapThumbnailScrollView* m_nMap;
    
    //void registerWithTouchDispatcher(void);


	void touchDownAction(Ref *senderz, Control::EventType controlEvent);//add by 2014-11-15 11:08:02
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
};

class MapThumbnailScrollView:
	public extension::ScrollView
{
    
    bool m_bIsMoved;
    
    Sprite* m_playerIndicator;
    
    Sprite* m_pEndPoint;
    
    void update(float delay);
    
    Point m_beginPoint;
    
public:
    
    MapThumbnailScrollView();
    
    ~MapThumbnailScrollView();
    
    static MapThumbnailScrollView* create();
    
    void initWithMap();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
};

#endif /* defined(__mir9__MapThumbnailLayer__) */
