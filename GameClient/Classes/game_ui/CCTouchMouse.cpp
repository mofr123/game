#include "CCTouchMouse.h"
#include "game_scene/GameScene.h"


CCTouchMouse::CCTouchMouse()
{
    //m_effectArray = new CCArray();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/touch_mouse.plist");
    if (m_effectArray.size() == 0)
    {
        int flag = 1;
        do
        {
            String* string = String::createWithFormat("touch_mouse_%02d.png",flag);
            SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->spriteFrameByName(string->getCString());
            CC_BREAK_IF(spriteFrame == NULL);
            m_effectArray.pushBack(spriteFrame);
            flag++;
        }
        while (1);
    }
}

CCTouchMouse::~CCTouchMouse()
{
    //CC_SAFE_RELEASE_NULL(m_effectArray);
}

void CCTouchMouse::playEffect(const Point& point)
{
    this->setVisible(true);
    this->setPosition(point);
	Animation* movie = Animation::createWithSpriteFrames(m_effectArray, 1 / (float)15);
	Animate* animate = Animate::create(movie);
    CallFunc* finish = CallFunc::create(this, callfunc_selector(CCTouchMouse::setHide));
    Sequence* actions = Sequence::create(animate, finish, NULL);
    this->stopAllActions();
    this->runAction(actions);
}

void CCTouchMouse::setHide()
{
    this->setVisible(false);
}