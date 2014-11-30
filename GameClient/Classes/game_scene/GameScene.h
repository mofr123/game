
#ifndef __mir_mobile__GameScene__
#define __mir_mobile__GameScene__

#include "figure/Player.h"
#include "figure/Enemy.h"
#include "figure/OtherPlayer.h"
#include "map/BgMap.h"
#include "skill_system/AttackSkillSystem.h"
#include "map/NpcInfoSystem.h"
#include "prop_system/PropSystem.h"
#include "GameLoading.h"
#include "game_ui/LAlertView.h"
#include "game_ui/CCTouchMouse.h"
#include "game_ui/GameInfoUIController.h"
#include "prop_system/PropIconShow.h"

#include "publicDef/PublicDef.h"


#define GAME_SCENE GameScene::shareGameScene()

class GameScene: public Layer
, public BgMapDelegate
{
    
    Point m_playerDirection;
    
    Sprite* m_selected;

    CCTouchMouse* m_touchMouse;
    
    bool m_touchProtected;
    
    CC_SYNTHESIZE_READONLY(bool, m_isPlayerMoveActions, PlayerMoveActions);
    
    CC_SYNTHESIZE(BgMap*, m_bgMap, CurrBgMap);
    
    CC_SYNTHESIZE_READONLY(GameInfoUIController*, m_gameInfoUIController, GameInfoUIController);
    
    CC_SYNTHESIZE_READONLY(Dictionary*, m_enemyDictionary, EnemyDictionary);
    
    CC_SYNTHESIZE_READONLY(Dictionary*, m_otherDictionary, OtherDictionary);
    
    std::map<Monomer*, int> m_gMapPoint;
    
    std::multimap<unsigned int, MapPoint> m_waitReincarnationEnemy;
    
    std::map<int, PropIconShow*> m_gPropMap;
    
public:
    
    static Scene* scene();
    
    GameScene();
    
    virtual ~GameScene();

    static GameScene* shareGameScene();
    
    static GameScene* create();
    
    bool init();
    
    void replaceBgMap(int mapID, const int born);

    void removeEnemy(Enemy* enemy);
    
    void addEnemy();
    
    vector<Monomer*> getMonmerVecIsLenght(Point point, int lenght);
    
    void OutOfCombat();
    
    void showSelected(Monomer* var);
    
    Monomer* getSelected();
    
    bool isPointValid(const MapPoint& point);
    
    std::deque<MapPoint> getPath(const MapPoint& begin, const MapPoint& end);
    
    std::deque<MapPoint> getPathNextRunGrid(const MapPoint& begin, const MapPoint& end);
    
    std::deque<MapPoint> getPathNextWalkGrid(const MapPoint& begin, const MapPoint& end);
    
    void insterMapPoint(Monomer* var, const MapPoint& point);
    
    void eraseMapPoint(Monomer* var);
    
    bool getMapPoint(const MapPoint& point);
    
    bool insterMapPointForProp(PropIconShow* var, const MapPoint& point);
    
    void eraseMapPointForProp(const MapPoint& point);
    
    PropIconShow* getMapPointForProp(const MapPoint& point);
    
protected:
    
    virtual void onEnterTransitionDidFinish();
    
    void registerTouch();
	virtual void onEnter();
	virtual void onExit();
    
	//void unRegisterWithTouchDispatcher(void);
    
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
	void onTouchMoved(Touch *pTouch, Event *pEvent);
    
	void onTouchEnded(Touch *pTouch, Event *pEvent);
    
	void onTouchCancelled(Touch *pTouch, Event *pEvent);

    void touchProtected();
    
    void untouchProtected();
    
    void updateImageDisplay(Rect& showRect, Rect& hideRect);
    
    void playerMovement(float dt);
    
    MapPoint getPlayerDirection();
    
public:
    
    void playerRunning(float delay = 0);
    
    virtual void beginMoveActions();
    
    virtual void stopMoveActions();
    
	virtual void AccurateMoveActions(const MapPoint& point);
    
public:
    
    //virtual void keyBackClicked();
    
    //virtual void keyMenuClicked();
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    void alertCallBack(const int number);
    
    void log(float dt);
};


#endif /* defined(__mir_mobile__GameScene__) */
