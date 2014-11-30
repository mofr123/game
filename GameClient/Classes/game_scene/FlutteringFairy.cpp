#include "FlutteringFairy.h"
#include "figure/Monomer.h"

FlutteringFairy::FlutteringFairy()
{

}

FlutteringFairy::~FlutteringFairy()
{

}

FlutteringFairy* FlutteringFairy::addFairy(Node *parent, Point point, FairyType type, int value, Ref* target, SEL_CallFunc callfunc)
{
    FlutteringFairy* fairy = new FlutteringFairy();
    fairy->initWithFairy(fairy ,type, value, target, callfunc);
    fairy->setPosition(ccpAdd(parent->getPosition(), point));
    parent->getParent()->addChild(fairy,10);
    fairy->release();
    return fairy;
}

void FlutteringFairy::initWithFairy(FlutteringFairy *fairy ,FairyType type, int value, Ref* target, SEL_CallFunc callfunc)
{
    Node::init();
    
    value = (int)fabsf(value);
    
    char str[25];
    
    Color3B color = Color3B::WHITE;
    
    switch (type)
    {
        case TypeAddExp:
        {
            sprintf(str, "获得经验:%d", value);
            color = Color3B::YELLOW;//ccYELLOW
            break;
        }
        case TypeAddBlood:
        {
            sprintf(str, "+%d", value);
            color = Color3B::GREEN;//ccGREEN;
            break;
        }
        case TypeSubBlood:
        {
            sprintf(str, "-%d", value);
            color = Color3B::RED;//ccRED
            break;
        }
        case TypeSubMagic:
        {
            sprintf(str, "-%d", value);
            color = Color3B(25, 18, 112);
            break;
        }
        default:
            break;
    }
    
    Label *label = Label::create(str, "Helvetica-Bold", 30);
	label->setPosition(Point::ZERO);

	m_fairy = fairy;
	m_fairy->addChild(label);
    m_fairy->setPosition(Point::ZERO);
    //this->addChild(m_fairy);2014-11-23 17:29:07
    m_fairy->setColor(color);
    
    m_fairy->setScale(2.0f);
    m_fairy->setOpacity(0);
    
    FadeIn* fadeIn = FadeIn::create(0.1f);
    ScaleTo* scaleTo = ScaleTo::create(0.2f, 1.0f);
    Spawn* spawn = Spawn::create(fadeIn, scaleTo, NULL);
    EaseSineOut* easeBack = EaseSineOut::create(spawn);
    DelayTime* delayTime = DelayTime::create(0.3f);
    FadeOut* fadeOut = FadeOut::create(0.2f);
    CallFunc* callFunc = CallFunc::create(target, callfunc);
    CallFunc* finish = CallFunc::create(this, callfunc_selector(FlutteringFairy::removeFromParent));
    Sequence* actions = Sequence::create(easeBack, delayTime, fadeOut, callFunc, finish, NULL);
    m_fairy->runAction(actions);
    
}

const Size& FlutteringFairy::getContentSize() const
{
    return m_fairy->getContentSize();
}