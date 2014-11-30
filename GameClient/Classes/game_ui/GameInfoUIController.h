#ifndef __mir9__GameInfoUIController__
#define __mir9__GameInfoUIController__

#include "publicDef/PublicDef.h"

#include "LOperationMenu.h"
#include "GameTabBarMenu.h"
#include "AttackEnemyInfo.h"
#include "ChatInterface.h"
#include "MapThumbnailLayer.h"


#define GAME_UILAYER GAME_SCENE->getGameInfoUIController()

class GameInfoUIController: public Layer {
    
    CC_SYNTHESIZE_READONLY(LOperationMenu*, m_operationMenu, OperationMenu);
    
    CC_SYNTHESIZE_READONLY(GameTabBarMenu*, m_gameTabBarMenu, GameTabBarMenu);
    
    CC_SYNTHESIZE_READONLY(AttackEnemyInfo*, m_attackEnemyInfo, AttackEnemyInfo);
    
    CC_SYNTHESIZE_READONLY(ChatInterface*, m_chatInterface, ChatInterface);
    
    CC_SYNTHESIZE(MapThumbnailMenu*, m_mapThumbnailMenu, MapThumbnailMenu);
    
    std::deque<Node*> m_pMenuSmall;
    
    Node* m_pMenuBig;
    
public:
    
    GameInfoUIController();
    
    virtual ~GameInfoUIController();
    
    CREATE_FUNC(GameInfoUIController);
    
    bool init();
    
    //void modeSwitch(CCObject* sender = NULL);
	void modeSwitch(Ref *pSender, Widget::TouchEventType type);
    
    void updateBloodPro();
    
    void updateMagicPro();
    
    void updateLevelTTF();
    
    void addSmallMenu(Node* node);
    
    void addBigMenu(Node* node);
    
    void removeSmallMenu(Node* node);
    
	//Node* node
    void removeSmallMenuAndButton(Ref* pSender, Control::EventType controlEvent);//CCControlEventTouchUpInside
    
    void removeBigMenuAndButton(Ref* pSender, Control::EventType controlEvent);
    
private:
    
    void registerTouch(void);
    
    void initWithHeadUI();
    
    void initWithMapThumbnail();
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
private:
    
    Size m_winSize;
    
    bool m_isShowTabBar;
    
    ImageView* m_headIcon;
    
    Text* m_nickNameTTF;
    
    TextAtlas* m_levelTTF;//LabelAtlas
    
    LoadingBar* m_bloodPro;
    
    LoadingBar* m_magicPro;
    
    Button* m_headBtn;
};


#endif /* defined(__mir9__GameInfoUIController__) */
