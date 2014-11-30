#ifndef __mir9__GameTabBarMenu__
#define __mir9__GameTabBarMenu__

#include "publicDef/PublicDef.h"

using namespace ui;


class GameTabBarMenu: public Layer {
    
public:
    
    CREATE_FUNC(GameTabBarMenu);
    
    bool init();
    
    void hideGameTabBarMenu();
    
    void showGameTabBarMenu();
    
public:
    
    void showRoleInfo(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showBackPack(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showSkillInfo(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showStrengthen(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showMakeFriends(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showDestiny(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showMall(Ref* sender, Widget::TouchEventType touchEvent);
    
    void showCanon(Ref* sender , Widget::TouchEventType touchEvent);
    
    void showSetUp(Ref* sender, Widget::TouchEventType touchEvent);

};


#endif /* defined(__mir9__GameTabBarMenu__) */
