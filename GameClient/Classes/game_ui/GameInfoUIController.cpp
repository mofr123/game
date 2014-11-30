#include "GameInfoUIController.h"
#include "LMenu.h"
#include "figure/PlayerController.h"
#include "figure/Player.h"


GameInfoUIController::GameInfoUIController()
:m_operationMenu(NULL)
,m_isShowTabBar(true)
,m_headIcon(NULL)
,m_headBtn(NULL)
,m_levelTTF(NULL)
,m_nickNameTTF(NULL)
,m_bloodPro(NULL)
,m_magicPro(NULL)
,m_gameTabBarMenu(NULL)
,m_attackEnemyInfo(NULL)
,m_chatInterface(NULL)
,m_pMenuBig(NULL)
{

}

GameInfoUIController::~GameInfoUIController()
{

}

void GameInfoUIController::registerTouch(void)
{
	//CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	//touchDispatcher->addTargetedDelegate(this, 100, true);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches( true);
	listener->onTouchBegan = CC_CALLBACK_2(GameInfoUIController::onTouchBegan, this );
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool GameInfoUIController::init()
{
    if (!Layer::init())
        return false;
    
    m_winSize = Director::getInstance()->getWinSize();
    
    this->initWithHeadUI();
    
    m_mapThumbnailMenu = MapThumbnailMenu::create();
    m_mapThumbnailMenu->setPosition(ccpSub(m_winSize, Point(100, 100)));
    this->addChild(m_mapThumbnailMenu);
    
    m_operationMenu = LOperationMenu::create();
    this->addChild(m_operationMenu);
    
    m_attackEnemyInfo = AttackEnemyInfo::create();
    m_attackEnemyInfo->setPosition(Point(m_winSize.width/2-125, m_winSize.height-110));
    this->addChild(m_attackEnemyInfo);
    m_attackEnemyInfo->hide();
    
    m_chatInterface = ChatInterface::create();
    m_chatInterface->setPosition(Point(220, 0));
    this->addChild(m_chatInterface);
    
    m_gameTabBarMenu = GameTabBarMenu::create();
    m_gameTabBarMenu->setPosition(Point(m_winSize.width-960, -100));
    this->addChild(m_gameTabBarMenu);
    
    return true;
}

void GameInfoUIController::initWithHeadUI()
{
    Layer* layer = Layer::create();
    layer->setScale(1.0f);
    layer->setPosition(Point(-480*(1-layer->getScale()), m_winSize.height-320));
    this->addChild(layer);
    
    Layout* widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/head_UI/head_UI.json"));
    if (widget)
    {
        layer->addChild(widget);
    }
	Vector<Node*> v1 = widget->getChildren();
	
    string nickName = PlayerController::sharePlayerController()->getPlayerName();
    m_nickNameTTF = dynamic_cast<Text*>(v1.at(1));//dynamic_cast<Label*>(Helper::seekWidgetByName(root, "nickName"));
    if (m_nickNameTTF)
    {
        m_nickNameTTF->setString(nickName);
    }
    
	//m_headIcon = dynamic_cast<ImageView*>(Helper::seekWidgetByName(root, "ImageView"));
	m_headIcon = dynamic_cast<ImageView*>(v1.at(2));
	//Vector<Node*> v2 = m_headIcon->getChildren();
    if (m_headIcon)
    {
        m_headIcon->setTouchEnabled(true);
    }
    
    m_levelTTF = dynamic_cast<TextAtlas*>(Helper::seekWidgetByName(m_headIcon,"level"));
    if (m_levelTTF)
    {
        m_levelTTF->setString("3");
    }
    
	m_bloodPro = dynamic_cast<LoadingBar*>(Helper::seekWidgetByName(m_headIcon,"redBar"));
    if (m_bloodPro)
    {
        m_bloodPro->setPercent(100);
    }
    
    m_magicPro = dynamic_cast<LoadingBar*>(Helper::seekWidgetByName(m_headIcon,"blueBar"));
    if (m_magicPro)
    {
        m_magicPro->setPercent(100);
    }
    
    m_headBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(m_headIcon,"head"));
    if (m_headBtn)
    {
        m_headBtn->setTouchEnabled(true);
       // m_headBtn->addReleaseEvent(this, coco_releaseselector(GameInfoUIController::modeSwitch));
		m_headBtn->addTouchEventListener(CC_CALLBACK_2(GameInfoUIController::modeSwitch, this));
    }
    
}

void GameInfoUIController::modeSwitch(Ref *pSender, Widget::TouchEventType type)
{
    if (this->getActionByTag(0xacff))
        return;
    
    if (m_isShowTabBar)
    {
        m_isShowTabBar = false;
        m_operationMenu->hideOperationMenu();
        m_chatInterface->hideChatInterface();
        
        DelayTime* delay = DelayTime::create(0.3f);
        CallFunc* finish = CallFunc::create(m_gameTabBarMenu, callfunc_selector(GameTabBarMenu::showGameTabBarMenu));
        Sequence* actions = Sequence::create(delay, finish, NULL);
        actions->setTag(0xacff);
        this->runAction(actions);
    }
    else
    {
        m_isShowTabBar = true;
        m_gameTabBarMenu->hideGameTabBarMenu();
    
        DelayTime* delay = DelayTime::create(0.3f);
        CallFunc* finish = CallFunc::create(m_operationMenu, callfunc_selector(LOperationMenu::showOperationMenu));
        CallFunc* finish2 = CallFunc::create(m_chatInterface, callfunc_selector(ChatInterface::showChatInterface));
        Sequence* actions = Sequence::create(delay, finish, finish2, NULL);
        actions->setTag(0xacff);
        this->runAction(actions);
    }
}

void GameInfoUIController::updateBloodPro()
{
    m_bloodPro->setPercent(100*Player::sharePlayer()->getBlood()/Player::sharePlayer()->getBloodCap());
}

void GameInfoUIController::updateMagicPro()
{
    m_bloodPro->setPercent(100*Player::sharePlayer()->getMagic()/Player::sharePlayer()->getMagicCap());
}

void GameInfoUIController::updateLevelTTF()
{

}

void GameInfoUIController::addSmallMenu(Node *node)
{
    this->removeBigMenuAndButton(node,Control::EventType::TOUCH_UP_INSIDE);//?
    
    for (std::deque<Node*>::iterator itr=m_pMenuSmall.begin(); itr!=m_pMenuSmall.end(); itr++)
    {
        if (typeid(*node) == typeid(**itr))
            return;
    }
    
    if (m_pMenuSmall.size() >= 2)
    {
        m_pMenuSmall.at(0)->removeFromParent();
        m_pMenuSmall.pop_front();
        m_pMenuSmall.at(0)->setPosition(Point(m_winSize.width/4, (m_winSize.height+80)/2));
    }
    
    node->setPosition(Point(m_winSize.width/4 * (m_pMenuSmall.size()*2+1), (m_winSize.height+80)/2));
    this->addChild(node);
    m_pMenuSmall.push_back(node);
    
    this->setTouchEnabled(true);
}

void GameInfoUIController::removeSmallMenu(Node *node)
{
    if (node == NULL)
        return;
    
    std::deque<Node*>::iterator* pItr = NULL;
    for (std::deque<Node*>::iterator itr=m_pMenuSmall.begin(); itr!=m_pMenuSmall.end(); itr++)
    {
        //if (node->isEqual(*itr)),可能不行
		if(node->getName()==(*itr)->getName())
        {
            pItr = &itr;
            break;
        }
    }
    
    if (pItr)
    {
        m_pMenuSmall.erase(*pItr);
        node->removeFromParent();
    }
    
    if (m_pMenuSmall.size() > 0)
    {
        m_pMenuSmall.at(0)->setPosition(Point(m_winSize.width/4, (m_winSize.height+80)/2));
    }
    else
    {
        this->setTouchEnabled(false);
    }
}

void GameInfoUIController::removeSmallMenuAndButton(Ref* pSender, Control::EventType controlEvent)
{
	Node *note = dynamic_cast<Node*>(pSender);
    this->removeSmallMenu(note->getParent());
}

void GameInfoUIController::addBigMenu(Node *node)
{
    this->removeBigMenuAndButton(node,Control::EventType::TOUCH_UP_INSIDE);//
    
    if (m_pMenuSmall.size() > 0)
    {
        for (std::deque<Node*>::iterator itr=m_pMenuSmall.begin(); itr!=m_pMenuSmall.end(); itr++)
            (*itr)->removeFromParent();
        
        m_pMenuSmall.clear();
    }
    
    
    node->setPosition(Point(m_winSize.width/2, (m_winSize.height+80)/2));
    this->addChild(node);
    m_pMenuBig = node;
    
    this->setTouchEnabled(true);
}

void GameInfoUIController::removeBigMenuAndButton(Ref* pSender, Control::EventType controlEvent)
{
    if (m_pMenuBig)
    {
        m_pMenuBig->removeFromParent();
        m_pMenuBig = NULL;
    }
    this->setTouchEnabled(false);
}

bool GameInfoUIController::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}