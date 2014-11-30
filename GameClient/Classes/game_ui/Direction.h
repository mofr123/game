
#include "map/MapPoint.h"
#include "game_scene/GameScene.h"


class Direction;
struct DirectionDelegate
{
    virtual void beginMoveActions() = 0;
    
    virtual void stopMoveActions() = 0;
    
	virtual void AccurateMoveActions(const MapPoint& point) = 0;
};


class Direction : public Node//, public CCTouchOneByOneDelegate
{
	Sprite* m_backGround;

	Sprite* m_rocker;

	Rect m_rect;

	Point m_point;

    CC_SYNTHESIZE(GameScene*, m_delegate, Delegate);//类型、名字、函数
    
    CC_SYNTHESIZE(float, m_bgRadius, Radius);
    
    CC_SYNTHESIZE(float, m_rockerRadius, rockerRadius);
    
    CC_SYNTHESIZE(float, m_touchEnabled, TouchEnabled);
    
public:

	Direction();

	virtual~Direction();

	void onEnter();

	void onExit();

	static Direction* shareDirection(Sprite* backGround, Sprite* rocker);
    
	bool initWithDirection(Sprite* backGround, Sprite* rocker);

private:

	//void registerWithTouchDispatcher(void);

	//void unRegisterWithTouchDispatcher(void);

	bool onTouchBegan(Touch *pTouch, Event *pEvent);

	void onTouchMoved(Touch *pTouch, Event *pEvent);

	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void onTouchCancelled(Touch *pTouch, Event *pEvent);

	void setRocker(Point point);

	void readyActions(float delay = 0.0f);
    
    void rockerHoming();

    //void touchBegin();
	void touchBegin(float dt);
    
    void touchEnd();
    
};

