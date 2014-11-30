#include "LOperationMenu.h"
#include "LMenu.h"
#include "figure/Player.h"
#include "game_scene/GameScene.h"
#include "figure/Enemy.h"
#include "game_scene/GameLoading.h"
#include "Direction.h"


LOperationMenu::LOperationMenu()
:m_isHangUpAttack(false)
,m_attackMethods(1001)
,m_moveMethods(0)
,m_direction(NULL)
,m_currProgress(NULL)
,m_operation(NULL)
{

}

LOperationMenu::~LOperationMenu()
{

}

bool LOperationMenu::init()
{
    if (!Layer::init())
        return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
//    Sprite* backGround = Sprite::create("ui/direction_tray.png");
//    Sprite* rocker = Sprite::create("ui/direction_head.png");
//    Direction* dir = Direction::shareDirection(backGround, rocker);
//	dir->setDelegate(GAME_SCENE);
//    dir->setRadius(120);
//    dir->setrockerRadius(40);
//	dir->setPosition(CCPoint(120, 120));
//    dir->setScale(0.8f);
//	this->addChild(dir);
//    m_direction = dir;
    
    m_operation = Node::create();
    m_operation->setPosition(Point(winSize.width, 0));
    this->addChild(m_operation);
    
    Size commonAttackBtnSize = Size(90, 90);
    Point commonAttackBtnPoint = ccpAdd(Point(10, 10), commonAttackBtnSize/2);
    
    m_commonAttackBtn = ControlButton::create(Scale9Sprite::create("ui/attack_normal.png"));
    m_commonAttackBtn->setBackgroundSpriteForState(Scale9Sprite::create("ui/attack_selected.png"), 
		Control::State::HIGH_LIGHTED);//CCControlStateHighlighted
    m_commonAttackBtn->setPreferredSize(commonAttackBtnSize);
    m_commonAttackBtn->setPosition(Point(-commonAttackBtnPoint.x, commonAttackBtnPoint.y));
    m_operation->addChild(m_commonAttackBtn, 0, 0);
    m_commonAttackBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LOperationMenu::touchCommonAttack), 
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside
    
    Size skillAttackBtnSize = Size(75, 75);
    Point skillAttackBtnPoint[3];
    skillAttackBtnPoint[0] = ccpAdd(Point(120, 10), skillAttackBtnSize/2);
    skillAttackBtnPoint[1] = ccpAdd(Point(110, 110), skillAttackBtnSize/2);
    skillAttackBtnPoint[2] = ccpAdd(Point(10, 120), skillAttackBtnSize/2);
    
    for (int i=0; i<3; i++)
    {
        m_skillAttackBtn[i] = ControlButton::create(Scale9Sprite::create("ui/groove_normal.png"));
        m_skillAttackBtn[i]->setBackgroundSpriteForState(Scale9Sprite::create("ui/groove_selected.png"), 
			Control::State::HIGH_LIGHTED);
        m_skillAttackBtn[i]->setPreferredSize(skillAttackBtnSize);
        m_skillAttackBtn[i]->setPosition(Point(-skillAttackBtnPoint[i].x, skillAttackBtnPoint[i].y));
        m_operation->addChild(m_skillAttackBtn[i], 0, 0);
        m_skillAttackBtn[i]->addTargetWithActionForControlEvents(this, cccontrol_selector(LOperationMenu::skillAttack), 
			Control::EventType::TOUCH_UP_INSIDE);
    }
    
    Size drugsBtnSize = Size(60, 60);
    Point drugsBtnPoint = ccpAdd(Point(210, 90),drugsBtnSize/2);
    
    m_drugsBtn = ControlButton::create(Scale9Sprite::create("ui/groove_normal.png"));
    m_drugsBtn->setBackgroundSpriteForState(Scale9Sprite::create("ui/groove_selected.png"), 
		Control::State::HIGH_LIGHTED);//CCControlStateHighlighted
    m_drugsBtn->setPreferredSize(drugsBtnSize);
    m_drugsBtn->setPosition(Point(-drugsBtnPoint.x, drugsBtnPoint.y));
    m_operation->addChild(m_drugsBtn, 0, 0);
    m_drugsBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LOperationMenu::useDrugs), 
		Control::EventType::TOUCH_UP_INSIDE);
    
    Size deliveryBtnSize = Size(60, 60);
    Point deliveryBtnPoint = ccpAdd(Point(10, 215), deliveryBtnSize/2);
    
    m_deliveryBtn = ControlButton::create(Scale9Sprite::create("ui/groove_normal.png"));
    m_deliveryBtn->setBackgroundSpriteForState(Scale9Sprite::create("ui/groove_selected.png"), 
		Control::State::HIGH_LIGHTED);

    m_deliveryBtn->setPreferredSize(deliveryBtnSize);
    m_deliveryBtn->setPosition(Point(-deliveryBtnPoint.x, deliveryBtnPoint.y));
    m_operation->addChild(m_deliveryBtn, 0, 0);
    m_deliveryBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LOperationMenu::usedeliveryProp), 
		Control::EventType::TOUCH_UP_INSIDE);
    
	//string str = ToUTF8::getInstance()->WStrToUTF8(L"自动攻击");
	auto label = Label::createWithTTF("Auto Attack", "fonts/Marker Felt.ttf", 12);
    ControlButton* hangUpBtn = ControlButton::create(label, Scale9Sprite::create("ui/direction_head.png"));
    hangUpBtn->setPreferredSize(Size(95, 95));
    hangUpBtn->setPosition(Point(WINSIZE.width-250, WINSIZE.height-100));
    this->addChild(hangUpBtn, 0, 0xfffffff0);
    hangUpBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LOperationMenu::hangUpAttack), 
		Control::EventType::TOUCH_UP_INSIDE);
    
	//取消跑步
    ControlButton* moveMethodsBtn = ControlButton::create(Label::createWithTTF("Cancel Running", "fonts/Marker Felt.ttf", 12), 
		Scale9Sprite::create("ui/direction_head.png"));
    moveMethodsBtn->setPreferredSize(Size(95, 95));
    moveMethodsBtn->setPosition(Point(100, WINSIZE.height-200));
    this->addChild(moveMethodsBtn, 0);
    moveMethodsBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LOperationMenu::moveMethods), 
		Control::EventType::TOUCH_UP_INSIDE);
    
    //this->addSkillIcon(0, 1002);
    this->addSkillIcon(1, 2007);
    this->addSkillIcon(2, 2005);
    return true;
}

ControlButton* LOperationMenu::getSkillAttackBtn(unsigned int number)
{
    if (number > 2)
        return NULL;
    
    return m_skillAttackBtn[number];

}

void LOperationMenu::addSkillIcon(ControlButton* btn, int skillNumber)
{
    Player::sharePlayer()->subAttackSkill(btn->getTag());
    
    btn->removeChildByTag(TAG_PROGRESS_SKILL);

    for (int i=0; i<3; i++)
    {
        if (skillNumber == m_skillAttackBtn[i]->getTag())
        {
            m_skillAttackBtn[i]->removeChildByTag(TAG_PROGRESS_SKILL);
            m_skillAttackBtn[i]->setTag(0);
        }
    }
    
    CCString* str = CCString::createWithFormat("ui/skill/skill_%d.png",skillNumber);
    Texture2D* texture = TextureCache::sharedTextureCache()->addImage(str->getCString());
    if (texture)
    {
        ProgressAutomatic* progressTimer = ProgressAutomatic::create(Sprite::createWithTexture(texture));
        progressTimer->setType(kCCProgressTimerTypeRadial);
        progressTimer->setPosition(btn->getContentSize()/2);
        btn->addChild(progressTimer, 1, TAG_PROGRESS_SKILL);
        progressTimer->setPercentage(100);

        progressTimer->setScale(1.8f);
    }
    
    btn->setTag(skillNumber);
    
    Player::sharePlayer()->addAttackSkill(skillNumber);
}

void LOperationMenu::addSkillIcon(int btnNumber, int skillNumber)
{
    ControlButton* btn = m_skillAttackBtn[btnNumber];
    this->addSkillIcon(btn, skillNumber);
}

void LOperationMenu::addDrugs(int drugsNumber)
{
    m_drugsBtn->removeChildByTag(TAG_PROGRESS_SKILL);
    
    CCString* str = CCString::createWithFormat("ui/skill/skill_%d.png",drugsNumber);
    Sprite* sprite = Sprite::create(str->getCString());
    sprite->setPosition(m_drugsBtn->getContentSize()/2);
    m_drugsBtn->addChild(sprite, 1, TAG_PROGRESS_SKILL);
    sprite->setScale(1.2f);
    
    m_drugsBtn->setTag(drugsNumber);
}

void LOperationMenu::touchCommonAttack(Ref* sender, Control::EventType controlEvents)
{
    m_currProgress = NULL;
    m_attackMethods = 1001;
    this->attack();
}

void LOperationMenu::commonAttack()
{
	m_currProgress = NULL;
	m_attackMethods = 1001;
	this->attack();
}

void LOperationMenu::skillAttack(Ref* sender, Control::EventType controlEvents)
{
    ControlButton* btn = (ControlButton*)sender;
    if (btn->getTag() == 0)
        return;
    m_currProgress = dynamic_cast<ProgressAutomatic*>(btn->getChildByTag(TAG_PROGRESS_SKILL));
    m_attackMethods = btn->getTag();
    this->attack();
}

void LOperationMenu::useDrugs(Ref* sender, Control::EventType controlEvents)
{
    Player::sharePlayer()->bloodReturn(5000);
    m_drugsBtn->removeChildByTag(TAG_PROGRESS_SKILL);
    m_drugsBtn->setTag(0);
}

void LOperationMenu::usedeliveryProp(Ref* sender, Control::EventType controlEvents)
{
    float r = CCRANDOM_0_1();
    
    if (r == 1)
        r = 0;
    
    GameLoading::runGameLoading(100, r * 4 + 1);
}

void LOperationMenu::skillAttackCooling(ProgressTimer* progress)
{
    
}

void LOperationMenu::attack()
{
    if (GAME_SCENE->getSelected() == NULL)
    {
        GAME_SCENE->showSelected(Player::sharePlayer()->getAgainstMeOfFirst());
        
    }
    
    Monomer* attackMonomerMajor =  GAME_SCENE->getSelected();
    
    if (attackMonomerMajor == NULL)
    {
        MapPoint playerPoint = MapPoint(Player::sharePlayer()->getPosition());
        CCArray  *array = CCArray::create();
        CCArray *enemyKeys = GAME_SCENE->getEnemyDictionary()->allKeys();
        if (enemyKeys)
        {
            for (unsigned int i=0; i<enemyKeys->count(); i++)
            {
                int key = ((CCInteger*)enemyKeys->objectAtIndex(i))->getValue();
                Enemy* enemy = (Enemy*)GAME_SCENE->getEnemyDictionary()->objectForKey(key);
                MapPoint enemyPoint = MapPoint(enemy->getPosition());
                if (playerPoint.getDistance(enemyPoint) <= Player::sharePlayer()->getVisualRange())
                {
                    array->addObject(enemy);
                }
            }
        }
        
        for (unsigned int i=0; i<array->count(); i++)
        {
            Enemy* enemy = (Enemy*)array->objectAtIndex(i);
            
            if (attackMonomerMajor == NULL)
            {
                attackMonomerMajor = enemy;
                continue;
            }
            MapPoint enemyPoint = MapPoint(enemy->getPosition());
            
            MapPoint attackMonomerPoint = MapPoint(attackMonomerMajor->getPosition());

            if (playerPoint.getDistance(enemyPoint) < playerPoint.getDistance(attackMonomerPoint))
            {
                attackMonomerMajor = enemy;
            }
        }
        GAME_SCENE->showSelected(attackMonomerMajor);
    }
    
    if (attackMonomerMajor)
    {
        Player::sharePlayer()->followAttackAndSetAttackMethods(attackMonomerMajor, m_attackMethods);
    }
}

void LOperationMenu::hangUpAttack(Ref* sender, Control::EventType controlEvents)
{
    bool flag = false;
    CCArray* enemyKeys = GAME_SCENE->getEnemyDictionary()->allKeys();
    if (enemyKeys)
    {
        for (unsigned int i=0; i<enemyKeys->count(); i++)
        {
            int key = ((CCInteger*)enemyKeys->objectAtIndex(i))->getValue();
            Enemy* enemy = (Enemy*)GAME_SCENE->getEnemyDictionary()->objectForKey(key);
            MapPoint playerPoint = MapPoint(Player::sharePlayer()->getPosition());
            MapPoint enemyPoint = MapPoint(enemy->getPosition());
            if (playerPoint.getDistance(enemyPoint) <= Player::sharePlayer()->getVisualRange())
            {
                flag = true;
                break;
            }
        }
    }
    
    do {
        CC_BREAK_IF(!flag);
        ControlButton* menu = (ControlButton*)sender;
        if (m_isHangUpAttack)
        {
            m_isHangUpAttack = false;
            ((Label*)menu->getTitleLabelForState(Control::State::NORMAL))->setString("Auto Attack");//"自动攻击"
        }
        else
        {
            m_isHangUpAttack = true;
            this->attack();
            ((Label*)menu->getTitleLabelForState(Control::State::NORMAL))->setString("Stop Auto");//停止自动
        }
    }
    while (0);
}

void LOperationMenu::intelligentSearch()
{
    if (m_isHangUpAttack)
    {
        CallFunc* callFunc = CallFunc::create(this, callfunc_selector(LOperationMenu::attack));
        Sequence* actions = Sequence::create(DelayTime::create(0.1f), callFunc, NULL);
        this->runAction(actions);
        
        if (Player::sharePlayer()->getBloodCap()/4 >= Player::sharePlayer()->getBlood())
        {
            Player::sharePlayer()->bloodReturn(Player::sharePlayer()->getBloodCap());
        }
    }
}

void LOperationMenu::cancelHangUP()
{
    if (m_isHangUpAttack)
    {
        this->hangUpAttack((Ref*)this->getChildByTag(0xfffffff0), Control::EventType::TOUCH_UP_INSIDE);
    }
}

void LOperationMenu::moveMethods(Ref* sender, Control::EventType controlEvents)
{
    ControlButton* menu = (ControlButton*)sender;
    
    if (m_moveMethods == 0)
    {
        m_moveMethods = 1;
        ((Label*)menu->getTitleLabelForState(Control::State::NORMAL))->setString("Cancel Walk");//取消步行
    }
    else if (m_moveMethods == 1)
    {
        m_moveMethods = 2;
        ((Label*)menu->getTitleLabelForState(Control::State::NORMAL))->setString("Cancel GPS");//取消寻路
    }
    else if (m_moveMethods == 2)
    {
        m_moveMethods = 0;
        ((Label*)menu->getTitleLabelForState(Control::State::NORMAL))->setString("Cancel Running");//取消跑步
    }
}

void LOperationMenu::hideOperationMenu()
{
    if (m_direction)
    {
        ((Direction*)m_direction)->setTouchEnabled(false);
        m_direction->stopAllActions();
        
        MoveTo* moveTo = MoveTo::create(0.3f, Point(-120, 120));
        ScaleTo* scaleTo = ScaleTo::create(0.3f, 0.8f);
        EaseSineOut* easeBack = EaseSineOut::create(Spawn::create(moveTo, scaleTo, NULL));
        m_direction->runAction(easeBack);
    }
    
    if (m_operation)
    {
        m_operation->stopAllActions();
        
        Size winSize = Director::getInstance()->getWinSize();
        MoveTo* moveTo2 = MoveTo::create(0.3f, Point(winSize.width+300, 0));
        ScaleTo* scaleTo2 = ScaleTo::create(0.3f, 1.0f);
        EaseSineOut* easeBack2 = EaseSineOut::create(Spawn::create(moveTo2, scaleTo2, NULL));
        m_operation->runAction(easeBack2);
    }
}

void LOperationMenu::showOperationMenu()
{
    if (m_direction)
    {
        ((Direction*)m_direction)->setTouchEnabled(true);
        m_direction->stopAllActions();
        
        MoveTo* moveTo = MoveTo::create(0.3f, Point(120, 120));
        ScaleTo* scaleTo = ScaleTo::create(0.3f, 0.8f);
        EaseSineOut* easeBack = EaseSineOut::create(Spawn::create(moveTo, scaleTo, NULL));
        m_direction->runAction(easeBack);
    }
    
    if (m_operation)
    {
        m_operation->stopAllActions();
        
        Size winSize = Director::getInstance()->getWinSize();
        MoveTo* moveTo2 = MoveTo::create(0.3f, Point(winSize.width, 0));
        ScaleTo* scaleTo2 = ScaleTo::create(0.3f, 1.0f);
        EaseSineOut* easeBack2 = EaseSineOut::create(Spawn::create(moveTo2, scaleTo2, NULL));
        m_operation->runAction(easeBack2);
    }
}

void LOperationMenu::joinEditState()
{
    if (m_operation)
    {
        m_operation->setPositionY(100);
        Size winSize = Director::getInstance()->getWinSize();
        MoveTo* moveTo2 = MoveTo::create(0.3f, Point(winSize.width, 100));
        ScaleTo* scaleTo2 = ScaleTo::create(0.3f, 1.0f);
        EaseSineOut* easeBack2 = EaseSineOut::create(Spawn::create(moveTo2, scaleTo2, NULL));
        m_operation->runAction(easeBack2);
    }
}
