#include "LAlertView.h"

LAlertView::LAlertView(const char* title, const char* messageText)
:m_title(title)
,m_messageText(messageText)
,m_target(NULL)
,m_callBack(NULL)
{
    //m_btnArray = new CCArray();
}

LAlertView::~LAlertView()
{
    //CC_SAFE_RELEASE_NULL(m_btnArray);
}

void LAlertView::onEnter()
{
    Scale9Sprite::onEnter();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->addTargetedDelegate(this, kCCMenuHandlerPriority-1000, true);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches( true);
	listener->onTouchBegan = CC_CALLBACK_2(LAlertView::onTouchBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}

void LAlertView::onExit()
{
    Scale9Sprite::onExit();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->removeDelegate(this);
}

LAlertView* LAlertView::create(const char* title, const char* messageText)
{
    LAlertView* _alertView = new LAlertView(title, messageText);
    if (_alertView && _alertView->init())
    {
        _alertView->autorelease();
    }
    return _alertView;
}

bool LAlertView::init()
{
    if (!Scale9Sprite::initWithFile("ui/alert_backGround.png"))
        return false;
    
    Label* title = Label::create(m_title, "Helvetica-Bold", 22);
    title->setAnchorPoint(Point(0.5, 1));
    title->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height-52));
    this->addChild(title);
    
    Label* message = Label::create(m_messageText, "Helvetica-Bold", 20);
    message->setPosition(ccpAdd(this->getContentSize()/2, Point(0, 30)));
    this->addChild(message);
    
    return true;
}

void LAlertView::addButtonWithTitle(const char *btnTitle)
{
    Label* message = Label::create(btnTitle, "Helvetica-Bold", 25);
    ControlButton* btn = ControlButton::create(message, Scale9Sprite::create("ui/btn_normal.png"));
    btn->setBackgroundSpriteForState(Scale9Sprite::create("ui/btn_selected.png"), 
		Control::State::SELECTED);//CCControlStateSelected
    btn->setPreferredSize(Size(140, 50));
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(LAlertView::touchUpInSide),
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside
    btn->setTag(0xff+m_btnArray.size());
    //btn->setTouchPriority(kCCMenuHandlerPriority-1001);
    m_btnArray.pushBack(btn);
}

void LAlertView::show(Ref *target, SEL_Alert selector)
{
    m_target = target;
    m_callBack = selector;

    if (m_btnArray.size() > 2)
    {
        this->setPreferredSize(Size((m_btnArray.size() + 1) * 150, _contentSize.height));
    }
    
    if (m_btnArray.size() == 0)
    {
        Label* message = Label::create("确定", "Helvetica-Bold", 25);
        ControlButton* btn = ControlButton::create(message, Scale9Sprite::create("ui/btn_normal.png"));
        btn->setBackgroundSpriteForState(Scale9Sprite::create("ui/btn_selected.png"), 
			Control::State::SELECTED);//CCControlStateSelected
        btn->setPreferredSize(Size(130, 43));
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(LAlertView::touchUpInSide),
			Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside
        btn->setTag(0xff);
        //btn->setTouchPriority(kCCMenuHandlerPriority-1001);
        btn->setPosition(Point(this->getContentSize().width/2, 100));
        this->addChild(btn);
    }
    else
    {
        for (int i=0; i<m_btnArray.size(); i++)
        {
            ControlButton* btn = (ControlButton*)m_btnArray.at(i);
            float x = this->getContentSize().width / (m_btnArray.size()+1) * (i + 1);
            btn->setPosition(Point(x, 100));
            this->addChild(btn);
        }
    }
    this->setPosition(Director::getInstance()->getWinSize()/2);
    Director::getInstance()->getRunningScene()->addChild(this, 0xffff);
}

void LAlertView::touchUpInSide(Ref* sender, Control::EventType controlEvents)
{
    ControlButton* btn = (ControlButton*)sender;
    
    if (m_target && m_callBack)
    {
        ((Ref *)m_target->*m_callBack)(btn->getTag()-0xff);
    }
    this->removeFromParent();
}

bool LAlertView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}