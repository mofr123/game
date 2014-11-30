#include "NpcFigure.h"
#include "game_ui/LAlertView.h"

NpcFigure::NpcFigure(unsigned int roleNumber, NPCDirectionType type)
:m_npcInfo(NULL)
,m_eTpye(type)
,m_fileName(NULL)
,m_plistName(NULL)
,m_sprite(NULL)
,m_nIsDisplay(false)
,m_fAnchorPoint(Point(0.5f, 3/8.0f))
,m_fSpriteRect(Rect(80, 64, 50, 96))
{
    m_npcInfo = NpcInfoSystem::getNpcInfo(roleNumber);
    this->setNpcFileName(CCString::createWithFormat("texture_npc/npc_%u_1_%d.png", m_npcInfo->getRID(), m_eTpye));
    this->setNpcPlistName(CCString::createWithFormat("texture_npc/npc_%u_1_%d.plist", m_npcInfo->getRID(), m_eTpye));
    //m_pArray = new CCArray();
}

NpcFigure::~NpcFigure()
{
    CC_SAFE_DELETE(m_fileName);
    CC_SAFE_DELETE(m_plistName);
    //CC_SAFE_DELETE(m_pArray);
}

void NpcFigure::onEnter()
{
    Sprite::onEnter();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->addTargetedDelegate(this, 100, true);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches( true);
	listener->onTouchBegan = CC_CALLBACK_2(NpcFigure ::onTouchBegan, this );
	listener->onTouchMoved = CC_CALLBACK_2(NpcFigure ::onTouchMoved, this );
	listener->onTouchEnded = CC_CALLBACK_2(NpcFigure ::onTouchEnded, this );
	listener->onTouchCancelled = CC_CALLBACK_2(NpcFigure::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void NpcFigure::onExit()
{
    Sprite::onExit();
    //CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //touchDispatcher->removeDelegate(this);
    this->hideImageView();
}

NpcFigure* NpcFigure::create(unsigned int roleNumber, NPCDirectionType type)
{
    NpcFigure* npc = new NpcFigure(roleNumber, type);
    if (npc && npc->init())
    {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return NULL;
}

void NpcFigure::displayImageView()
{
    if (m_nIsDisplay == false)
    {
        m_nIsDisplay = true;
        
        TextureCache::getInstance()->addImageAsync(m_fileName->getCString(),  
			CC_CALLBACK_1(NpcFigure::initWithImageView,this));
    }
}

void NpcFigure::hideImageView()
{
    if (m_nIsDisplay)
    {
        m_nIsDisplay = false;

        if (m_sprite)
        {
            m_sprite->removeFromParent();
            m_sprite = NULL;
        }
        
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(m_plistName->getCString());
        TextureCache::getInstance()->removeTextureForKey(m_fileName->getCString());
        //m_pArray->removeAllObjects();
		m_pArray.clear();
    }
}

void NpcFigure::initWithImageView(Texture2D* texture)
{
    if (m_sprite == NULL)
    {
        m_sprite = Sprite::create();
        m_sprite->setAnchorPoint(m_fAnchorPoint);
        this->addChild(m_sprite);
        
        m_spriteHigh = Sprite::create();
        m_spriteHigh->setAnchorPoint(m_fAnchorPoint);
        this->addChild(m_spriteHigh);
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_plistName->getCString(), texture);
        
        m_sprite->runAction(this->getActions());
        m_spriteHigh->runAction(this->getActions());
        
        m_spriteHigh->setVisible(false);
    }
}

void NpcFigure::IntelligentDisplay(Rect& showRect, Rect& hideRect)
{
    if (showRect.containsPoint(_position))//m_obPosition
    {
        this->displayImageView();
    }
    
    if (!hideRect.containsPoint(_position))
    {
        this->hideImageView();
    }
}

Action* NpcFigure::getActions()
{
    if (m_pArray.size() == 0)
    {
        int flag = 0;
        do
        {
            CCString* string = CCString::createWithFormat("npc_%u_1_%d_%02d.png", m_npcInfo->getRID(), m_eTpye, flag);
            SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->spriteFrameByName(string->getCString());
            CC_BREAK_IF(spriteFrame == NULL);
            m_pArray.pushBack(spriteFrame);
            flag++;
        }
        while (1);
    }
    
    if (m_pArray.size() > 0)
    {
        Animation* movie = Animation::createWithSpriteFrames(m_pArray, 1 / (float)5);
        Animate* animate = Animate::create(movie);
        RepeatForever* repeatForever = RepeatForever::create(animate);
        return repeatForever;
    }
    return NULL;
}

void NpcFigure::setHighlight()
{
    if (m_spriteHigh->isVisible() == false)
    {
        ccBlendFunc blend = {GL_DST_COLOR, GL_ONE};
        
        m_spriteHigh->setBlendFunc(blend);
        
        m_spriteHigh->setVisible(true);
    }
}

void NpcFigure::setNormal()
{
    if (m_spriteHigh->isVisible() == true)
    {
//        ccBlendFunc blend = {CC_BLEND_SRC, CC_BLEND_DST};
//        
//        m_spriteHigh->setBlendFunc(blend);
        
        m_spriteHigh->setVisible(false);
    }
}

bool NpcFigure::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (m_sprite == NULL)
        return false;
    
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    if (!m_fSpriteRect.containsPoint(point))
    {
        return false;
    }
    
    this->setHighlight();
    
    return true;
}

void NpcFigure::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    if (!m_fSpriteRect.containsPoint(point))
    {
        this->setNormal();
    }
    else
    {
        this->setHighlight();
    }
    
}

void NpcFigure::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    this->setNormal();
    
    log("%s,%s",m_npcInfo->getNpcName().c_str(), m_npcInfo->getSentence().c_str());
    
    LAlertView* alertView = LAlertView::create("", m_npcInfo->getSentence().c_str());
    alertView->show(this, Alert_selector(NpcFigure::alertCallBack));
    
}

void NpcFigure::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

void NpcFigure::alertCallBack(const int number)
{
    if (number == 0)
    {
        
    }
}