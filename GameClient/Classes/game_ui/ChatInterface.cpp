#include "ChatInterface.h"
#include "ChatInterfaceCell.h"
#include "figure/PlayerController.h"


ChatInterface::ChatInterface()
:m_selectedBtn(NULL)
,m_selected(NULL)
,m_tooBar(NULL)
,m_dataArray(NULL)
,m_tableView(NULL)
{
    m_dataWorldArray = new deque<ChatInfoObject>;
    m_dataGuildArray = new deque<ChatInfoObject>;
    m_dataSystemArray = new deque<ChatInfoObject>;
}

ChatInterface::~ChatInterface()
{
    CC_SAFE_DELETE(m_dataWorldArray);
    CC_SAFE_DELETE(m_dataGuildArray);
    CC_SAFE_DELETE(m_dataSystemArray);
}


/*void ChatInterface::registerWithTouchDispatcher()
{
	//CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	//touchDispatcher->addTargetedDelegate(this, 1, true);//数字表示优先级
}*/

void ChatInterface::onEnterTransitionDidFinish()
{
    LayerColor::onEnterTransitionDidFinish();
    this->switchInterface(m_worldBtn,Control::EventType::TOUCH_UP_INSIDE);//m_worldBtn
}

bool ChatInterface::init()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    if (!LayerColor::initWithColor(Color4B(100, 100, 100, 100), //ccc4
		winSize.width-520, winSize.height/2))
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    
    m_tooBar = Scale9Sprite::create("ui/switch_toobar.png");
    m_tooBar->setPreferredSize(Size(this->getContentSize().width+40, 65));
    m_tooBar->setAnchorPoint(Point(0.5, 1.f));
    this->addChild(m_tooBar);
    
    m_selected = Sprite::create("ui/switch_selected.png");
    m_tooBar->addChild(m_selected);
    
	string str1 = ToUTF8::getInstance()->WStrToUTF8(L"世界");
	string str2 = ToUTF8::getInstance()->WStrToUTF8(L"行会");
	string str3 = ToUTF8::getInstance()->WStrToUTF8(L"系统");
    m_worldBtn = ChatInterface::createBtn(str1.c_str(), ccpAdd(m_tooBar->getContentSize()/2, Point(-170, 0)), CHAT_TAG_WORLD);
    m_guildBtn = ChatInterface::createBtn(str2.c_str(), ccpAdd(m_tooBar->getContentSize()/2, Point(0, 0)), CHAT_TAG_GUILD);
    m_systemBtn = ChatInterface::createBtn(str3.c_str(), ccpAdd(m_tooBar->getContentSize()/2, Point(170, 0)), CHAT_TAG_SYSTEM);
    
    string str = "12345678901234567890";
    for (int i=0; i<20; i++)
    {
        m_dataWorldArray->push_back(ChatInfoObject(CHAT_TAG_WORLD, 0, "", str));
        str  = str + "1234567890";
    }
    
    
    m_switchSizeBtn = ControlButton::create(Scale9Sprite::create("ui/indicator.png"));
    m_switchSizeBtn->setPreferredSize(Size(40, 40));
    m_switchSizeBtn->setPosition(Point(this->getContentSize().width+50, 50));
    this->addChild(m_switchSizeBtn);
    m_switchSizeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ChatInterface::switchSize), 
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside
    
    Size boxSize=Size(winSize.width-680, 40);
	str3 = ToUTF8::getInstance()->WStrToUTF8(L"从这里输入");
    m_textField = LyfTextFieldTTF::textFieldWithPlaceHolder(str3.c_str(), boxSize, kCCTextAlignmentLeft, "Helvetica-Bold", 30);
    m_textField->setAnchorPoint(Point::ZERO);
    m_textField->setDelegate(this);
    this->addChild(m_textField);
    m_textField->setMaxLength(120);
    m_textField->setVisible(false);
    
    LayerColor* textFieldBg = LayerColor::create(Color4B(200, 200, 200, 200), boxSize.width+10, boxSize.height);
    textFieldBg->setPosition(Point(-5, 0));
    //m_textField->addChild(textFieldBg, -1);//delete 2014-11-22 12:02:19
    
	str3 = ToUTF8::getInstance()->WStrToUTF8(L"发送");
    m_sendNewsBtn = ControlButton::create(Label::create(str3.c_str(), "Helvetica-Bold", 30), Scale9Sprite::create("ui/btn_normal.png"));
    m_sendNewsBtn->setBackgroundSpriteForState(Scale9Sprite::create("ui/btn_selected.png"), Control::State::HIGH_LIGHTED);//CCControlStateHighlighted
    m_sendNewsBtn->setPreferredSize(Size(boxSize.height*2, boxSize.height));
    this->addChild(m_sendNewsBtn);
    m_sendNewsBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ChatInterface::sendNews), 
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside
    m_sendNewsBtn->setVisible(false);
    
    m_worldTableView = ChatInterface::createTableView();
    m_guildTtableView = ChatInterface::createTableView();
    m_systemTableView = ChatInterface::createTableView();
    
    this->setSizeMin();
    
    return true;
}

LyfTableView* ChatInterface::createTableView()
{
    LyfTableView* tableView = LyfTableView::create(this->getContentSize().height-5);
    tableView->setPosition(Point(0, 5));
    tableView->setTableViewDataSource(this);
    this->addChild(tableView);
    tableView->setIndicatorPointOff(0.f);
    tableView->setIndicatorScaleX(1.0f);
    tableView->setMaxNumberOfPages(50);
    tableView->setCanSkip(false);
    tableView->setVisible(false);
    tableView->setScale(0.f);
    return tableView;
}

ControlButton* ChatInterface::createBtn(const char *title, Point point, int tag)
{
    ControlButton* btn = ControlButton::create(title, "Arial", 30);
    btn->setPosition(point);
    m_tooBar->addChild(btn);
    btn->setTag(tag);
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ChatInterface::switchInterface), 
		Control::EventType::TOUCH_UP_INSIDE);//CCControlEventTouchUpInside
    return btn;
}

void ChatInterface::setSizeMax()
{
    if (this->getContentSize().height <= 100)
    {
        Size winSize = Director::getInstance()->getWinSize();
        Size size = this->getContentSize();
        size.height = winSize.height/2;
        m_guildTtableView->setContentSizeHeight(size.height-110);
        m_systemTableView->setContentSizeHeight(size.height-110);
        m_worldTableView->setContentSizeHeight(size.height-110);
        m_textField->setVisible(true);
        m_sendNewsBtn->setVisible(true);
        m_textField->setPosition(Point(10, size.height-95));
        m_sendNewsBtn->setPosition(Point(m_textField->getSize().width+75, size.height-75));
        this->setContentSize(size);
        m_tooBar->setVisible(true);
        m_tooBar->setScale(1.f);
        m_tooBar->setPosition(Point(size.width/2, size.height+10));
    }
}

void ChatInterface::setSizeMin()
{
    if (this->getContentSize().height > 100)
    {
        Size size = this->getContentSize();
        size.height = 100;
        m_guildTtableView->setContentSizeHeight(size.height-5);
        m_systemTableView->setContentSizeHeight(size.height-5);
        m_worldTableView->setContentSizeHeight(size.height-5);
        m_textField->setVisible(false);
        m_sendNewsBtn->setVisible(false);
        this->setContentSize(size);
        m_tooBar->setVisible(false);
        m_tooBar->setScale(0);
        m_tooBar->setPositionY(-10);
    }
}

void ChatInterface::showChatInterface()
{
    this->stopAllActions();
    MoveTo* moveTo = MoveTo::create(0.3f, Point(this->getPositionX(), 0));
    EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    this->runAction(easeBack);
}

void ChatInterface::hideChatInterface()
{
    this->setSizeMin();
    
    this->stopAllActions();
    MoveTo* moveTo = MoveTo::create(0.3f, Point(this->getPositionX(), -this->getContentSize().height-20));
    EaseSineOut* easeBack = EaseSineOut::create(moveTo);
    this->runAction(easeBack);
}

void ChatInterface::switchSize(Ref* sender,Control::EventType controlEvent)
{
    if (this->getContentSize().height > 150)
    {
        this->setSizeMin();
    }
    else
    {
        this->setSizeMax();
    }
}

void ChatInterface::switchInterface(Ref* sender,Control::EventType controlEvent)
{
    if (m_selectedBtn)
    {
        ((ControlButton*)m_selectedBtn)->setTouchEnabled(true);
        Sprite* sprite =(Sprite*)((ControlButton*)m_selectedBtn)->getTitleLabel();
        sprite->setColor(ccWHITE);
    }
    
    m_selectedBtn = (ControlButton*)sender;
    ((ControlButton*)m_selectedBtn)->setHighlighted(false);
    ((ControlButton*)m_selectedBtn)->setTouchEnabled(false);
    
    m_selected->stopAllActions();
    MoveTo* moveTo = MoveTo::create(0.2f, m_selectedBtn->getPosition());
    m_selected->runAction(moveTo);
    
    Sprite* sprite =(Sprite*)((ControlButton*)m_selectedBtn)->getTitleLabel();
    sprite->setColor(ccYELLOW);
    
    if (m_tableView)
    {
        m_tableView->setVisible(false);
        m_tableView->setScale(0.f);
    }
    
    if (m_selectedBtn->getTag() == CHAT_TAG_GUILD)
    {
        m_tableView = m_guildTtableView;
        m_dataArray = m_dataGuildArray;
    }
    else if (m_selectedBtn->getTag() == CHAT_TAG_SYSTEM)
    {
        m_tableView = m_systemTableView;
        m_dataArray = m_dataSystemArray;
    }
    else
    {
        m_tableView = m_worldTableView;
        m_dataArray = m_dataWorldArray;
    }
    
    m_tableView->setVisible(true);
    m_tableView->setScale(1.f);
}

void ChatInterface::sendNews(Ref* sender,Control::EventType controlEvent)
{
    if (m_textField->getString().c_str() == NULL || strcmp(m_textField->getString().c_str(), "") == 0)
        return;
    
    if (m_selectedBtn->getTag() == CHAT_TAG_SYSTEM)
    {
        MessageBox("系统频道不能发言！", "警告");
        return;
    }
    
    ChatInfoObject object = ChatInfoObject(m_selectedBtn->getTag(), 0, PlayerController::sharePlayerController()->getPlayerName(), m_textField->getString());
    
    m_dataArray->push_back(object);
    m_tableView->insertLastTableCell();
    
    float y = MAX(m_tableView->getTableViewHight(), m_tableView->getContentSize().height);
    m_tableView->getNode()->setPositionY(y);
    
    m_textField->setString("");
}

LyfTableViewCell* ChatInterface::tableCellAtIndex(LyfTableView *table, unsigned int idx)
{
    ChatInterfaceCell *cell = cell = ChatInterfaceCell::create(this->getContentSize().width);
    
    if (table == m_guildTtableView)
    {
        cell->setString(m_dataGuildArray->at(idx));
    }
    else if (table == m_systemTableView)
    {
        cell->setString(m_dataSystemArray->at(idx));
    }
    else
    {
        cell->setString(m_dataWorldArray->at(idx));
    }
    
    return cell;
}

unsigned int ChatInterface::numberfRowsInTableView(LyfTableView *table)
{
    if (table == m_guildTtableView)
    {
        return m_dataGuildArray->size();
    }
    else if (table == m_systemTableView)
    {
        return m_dataSystemArray->size();
    }
    else
    {
        return m_dataWorldArray->size();
    }
}

bool ChatInterface::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
	point = this->convertToNodeSpace(point);
    
    Rect rect = CCRectZero;
    rect.size = this->getContentSize();
    
    if (rect.containsPoint(point))
    {
        return true;
    }
    return false;
}

bool ChatInterface::onTextFieldDetachWithIME(TextFieldTTF *sender){
    //键盘收回

    return false;
}

bool ChatInterface::onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen){
    //返回True 表示不可再加入文字
    
    return false;
}