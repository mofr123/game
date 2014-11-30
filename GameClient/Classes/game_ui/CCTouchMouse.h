#ifndef __mir9__CCTouchMouse__
#define __mir9__CCTouchMouse__

#include "publicDef/PublicDef.h"

class CCTouchMouse: public Sprite {
    
    Vector<SpriteFrame*> m_effectArray;
    
    void setHide();
    
public:
    
    CCTouchMouse();
    
    virtual ~CCTouchMouse();
    
    CREATE_FUNC(CCTouchMouse);
    
    void playEffect(const Point& point);
    
};

#endif /* defined(__mir9__CCTouchMouse__) */
