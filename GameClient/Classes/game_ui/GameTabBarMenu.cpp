#include "GameTabBarMenu.h"

#include "game_scene/GameScene.h"
#include "SkillTableView.h"
#include "PropColumn.h"
#include "GameOptions.h"


//#define WINSIZE Director::getInstance()->getWinSize()

bool GameTabBarMenu::init()
{
	if (!Layer::init())
    {
        return false;
    }
    
    Layout* widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/tabbarMenu/tabbarMenu.json"));
	widget->setTouchEnabled(true);
	this->addChild(widget);
	Layout* root = static_cast<Layout*>(widget->getChildByName("Panel_22"));
	
    Button* roleInfoBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_role"));
    roleInfoBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showRoleInfo, this));
    
    Button* backPackBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_backPack"));//
    backPackBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showBackPack, this));
    
    Button* skillBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_skill"));
    skillBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showSkillInfo, this));
    
    Button* strengthenBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_strengthen"));
    strengthenBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showStrengthen, this));
    
    Button* makeFriendsBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_makeFriends"));
    makeFriendsBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showMakeFriends, this));
    
    Button* destinyBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_destiny"));
    destinyBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showDestiny, this));
    
    Button* mallBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_mall"));
    mallBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showMall, this));
    
    Button* canonBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButton_canon"));
    canonBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showCanon, this));
    
    Button* setUpBtn = static_cast<Button*>(Helper::seekWidgetByName(root, "TextButtonsetUp"));
    setUpBtn->addTouchEventListener(CC_CALLBACK_2(GameTabBarMenu::showSetUp, this));
    
    return true;
};



void GameTabBarMenu::showRoleInfo(Ref* sender, Widget::TouchEventType touchEvent)
{

}

void GameTabBarMenu::showBackPack(Ref* sender, Widget::TouchEventType touchEvent)
{
    PropColumnMenu* layer= PropColumnMenu::create();
    GAME_UILAYER->addSmallMenu(layer);
    GAME_UILAYER->getOperationMenu()->joinEditState();
}

void GameTabBarMenu::showSkillInfo(Ref* sender, Widget::TouchEventType touchEvent)
{
    SkillTableView* layer = SkillTableView::create();
    GAME_UILAYER->addSmallMenu(layer);
    GAME_UILAYER->getOperationMenu()->joinEditState();
}

void GameTabBarMenu::showStrengthen(Ref* sender,Widget::TouchEventType touchEvent)
{
    
}

void GameTabBarMenu::showMakeFriends(Ref* sender,Widget::TouchEventType touchEvent)
{
    
}

void GameTabBarMenu::showDestiny(Ref* sender, Widget::TouchEventType touchEvent)
{
    
}

void GameTabBarMenu::showMall(Ref* sender, Widget::TouchEventType touchEvent)
{
    
}

void GameTabBarMenu::showCanon(Ref* sender , Widget::TouchEventType touchEvent)
{
    
}

void GameTabBarMenu::showSetUp(Ref* sender, Widget::TouchEventType touchEvent)
{
    GameOptions* layer = GameOptions::create();
    GAME_UILAYER->addBigMenu(layer);
}

void GameTabBarMenu::hideGameTabBarMenu()
{
    this->stopAllActions();
    Size winSize = Director::getInstance()->getWinSize();
    MoveTo* moveTo = MoveTo::create(0.3f, Point(winSize.width-960, -100));
    EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    this->runAction(easeBack);
}

void GameTabBarMenu::showGameTabBarMenu()
{
    this->stopAllActions();
    Size winSize = Director::getInstance()->getWinSize();
    MoveTo* moveTo = MoveTo::create(0.3f, Point(winSize.width-960, 0));
    EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    this->runAction(easeBack);
}