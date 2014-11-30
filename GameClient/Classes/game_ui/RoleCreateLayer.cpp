#include "RoleCreateLayer.h"
#include "game_scene/GameLoading.h"
#include "GameSocket.h"
#include "figure/Monomer.h"
#include "figure/TextureController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* font_UITextFieldTest ="Marker Felt";
#else
const char* font_UITextFieldTest ="fonts/Marker Felt.ttf";
#endif

RoleCreateLayer::RoleCreateLayer()
	: m_textButton(NULL)
	,m_textField(NULL)
	,m_nickName("")
	,m_selected(NULL)
{
	//m_roleArray = new CCArray();
}

RoleCreateLayer::~RoleCreateLayer()
{
	//CC_SAFE_DELETE(m_roleArray);
	TextureCache::getInstance()->removeAllTextures();
}

bool RoleCreateLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	//ccc4
	LayerColor* layerColor = LayerColor::create(Color4B(140, 150, 180, 255), winSize.width, winSize.height);
	this->addChild(layerColor, -1);

	Layer* layer = Layer::create();
	layer->setPosition(ccpSub(winSize, Point(960, 640))/2);
	layer->scheduleUpdate();
	this->addChild(layer);

	ui::Layout* widget = dynamic_cast<ui::Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/role_create/role_create.json"));
	if (widget)
	{
		layer->addChild(widget);
	}
	ui::Layout* root = static_cast<ui::Layout*>(widget->getChildByName("Panel"));
	m_textButton = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(root, "TextButton"));
	if (m_textButton)
	{
		m_textButton->setColor(Color3B(127, 127, 127));
		// m_textButton->addReleaseEvent(this, coco_releaseselector(RoleCreateLayer::sendMessage));
		m_textButton->addTouchEventListener(CC_CALLBACK_2(RoleCreateLayer::sendMessage, this));
	}

	//ui::Button* button10 = dynamic_cast<ui::Button*>(layer->getChildByName("Button_10"));
	ui::Button* button10 = static_cast<ui::Button*>(ui::Helper::seekWidgetByName(root, "Button_10"));
	if (button10)
	{
		button10->setTouchEnabled(true);
		//button10->addReleaseEvent(this, coco_releaseselector(RoleCreateLayer::randomNickName));
		button10->addTouchEventListener(CC_CALLBACK_2(RoleCreateLayer::randomNickName, this));
	}

	//UITextArea
	//Label* textAreaTitle = dynamic_cast<Label*>(layer->getChildByName("TextTitle"));
	ui::Text* textAreaTitle = static_cast<ui::Text*>(ui::Helper::seekWidgetByName(root, "TextTitle"));
	if (textAreaTitle)
	{
		textAreaTitle->setString(ToUTF8::getInstance()->WStrToUTF8(L"傲来国"));
	}

	//m_textField = dynamic_cast<ui::TextField*>(layer->getChildByName("TextField"));
	m_textField = static_cast<ui::TextField*>(ui::Helper::seekWidgetByName(root, "TextField"));
	if (m_textField)
	{
		//m_textField->setFontName(font_UITextFieldTest);
		m_textField->setTouchEnabled(true); 

		//m_textField->setTouchSize(CCSize(200, 40));addEventListenerTextField(textfieldeventselector)
		m_textField->addEventListener(CC_CALLBACK_2(RoleCreateLayer::eventListenerTextField,this));
		//m_textField->addAttachWithIMEEvent(this, coco_TextField_AttachWithIME_selector(RoleCreateLayer::attachWithIMEEvent));
		// m_textField->addDetachWithIMEEvent(this, coco_TextField_DeleteBackward_selector(RoleCreateLayer::detachWithIMEEvent));
		//m_textField->addInsertTextEvent(this, coco_TextField_DeleteBackward_selector(RoleCreateLayer::insertTextEvent));
		//m_textField->addDeleteBackwardEvent(this, coco_TextField_DeleteBackward_selector(RoleCreateLayer::detachWithIMEEvent));
	}

	for (unsigned int i =0; i < 6; i++)
	{
		CCString* string = CCString::createWithFormat("FIGURE_%d",i);
		//UIPanel* panel = dynamic_cast<UIPanel*>(layer->getWidgetByName(string->getCString()));
		ui::Layout *panel = static_cast<ui::Layout*>(ui::Helper::seekWidgetByName(root, string->getCString()));
		if (panel)
		{
			ControlButton* button = ControlButton::create();
			button->setHighlighted(false);
			button->setPreferredSize(Size(120, 200));
			button->setPosition(ccpAdd(layer->getPosition(), panel->getPosition()));
			this->addChild(button);
			button->setTag(panel->getTag());
			button->addTargetWithActionForControlEvents(this, cccontrol_selector(RoleCreateLayer::setSelector), 
				Control::EventType::TOUCH_DOWN);

			Figure* monomer = Figure::create(TexturePathFigure, panel->getTag());
			monomer->setHair(panel->getTag() / 10 - 100);
			monomer->setWeapon(1000);
			monomer->setPosition(button->getContentSize()/2);
			monomer->setColor(Color3B(127, 127, 127));
			button->addChild(monomer);
			m_roleArray.pushBack(monomer);

			//CCCallFunc* finish = CCCallFunc::create(monomer, callfunc_selector(Monomer::stopAllActions));
			//CCCallFunc* finish2 = CCCallFunc::create(monomer->getWeaponSprite(), callfunc_selector(CCSprite::stopAllActions));
			//CCSequence* actions = CCSequence::create(CCDelayTime::create(0.15f), finish, finish2, NULL);
			//monomer->runAction(actions);
		}
	}

	return true;
}

void RoleCreateLayer::runActionsForFigure(Ref* pSender)
{
	Figure* monomer = (Figure*)pSender;

	monomer->setFigureState(FStateAttack, FDirDown);

	DelayTime* delayTime = DelayTime::create(2.f);
	CCCallFuncO* finish = CCCallFuncO::create(this, callfuncO_selector(RoleCreateLayer::runActionsForFigure), monomer);
	Sequence* actions = Sequence::create(delayTime, finish, NULL);
	monomer->runAction(actions);
}

void RoleCreateLayer::setSelector(Ref* pSender, Control::EventType controlEvent)
{
	ControlButton* btn = dynamic_cast<ControlButton*>(pSender);
	if (btn)
	{
		m_roleID = btn->getTag();
		if (m_selected)
		{
			Figure* monomer = (Figure*)m_selected;
			ControlButton* button = (ControlButton*)monomer->getParent();
			monomer->setFigureState(FStateStand, FDirDown);
			monomer->setColor(ccc3(127, 127, 127));
			ScaleTo* scaleTo = ScaleTo::create(0.15f, 1.0f);
			CallFunc* finish = CallFunc::create(monomer, callfunc_selector(Monomer::stopAllActions));
			CallFunc* finish2 = CallFunc::create(monomer->getWeaponSprite(), callfunc_selector(Sprite::stopAllActions));
			Sequence* actions = Sequence::create(scaleTo, finish, finish2, NULL);
			monomer->runAction(actions);
			button->setTouchEnabled(true);
		}

		for (int i=0; i<m_roleArray.size(); i++)
		{
			Figure* monomer = (Figure*)m_roleArray.at(i);
			ControlButton* button = (ControlButton*)monomer->getParent();
			if (btn->_ID==button->_ID)//btn-> isEqual(button)
			{
				button->setTouchEnabled(false);
				ScaleTo* scaleTo = ScaleTo::create(0.15, 1.2);
				monomer->runAction(scaleTo);
				monomer->setColor(Color3B::WHITE);
				this->runActionsForFigure(monomer);
				m_selected = monomer;
			}
		}
	}

	if (m_textButton)
	{
		m_textButton->setColor(Color3B::WHITE);//ccWHITE
		m_textButton->setTouchEnabled(true);
	}
}

void RoleCreateLayer::randomNickName(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (m_textField)
	{
		int num = CCRANDOM_0_1() * 0xffffff;
		char str[20];
		sprintf(str, "random%d",num);
		m_textField->setText(str);
	}
}

void RoleCreateLayer::sendMessage(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (0 == strcmp(m_textField->getStringValue().c_str(), ""))
	{
		MessageBox("您还没有取名哦!","提示");
		return;
	}
	GameSocket::sendRoleCreate(this, m_roleID, m_textField->getStringValue().c_str());//SOCKET
	m_textField->setTouchEnabled(false);
}

void RoleCreateLayer::eventListenerTextField(Ref* pSender,ui::TextField::EventType type)
{
	switch (type)//
	{
	case ui::TextField::EventType::ATTACH_WITH_IME://激活输入时触发
		{
			attachWithIMEEvent(pSender);
		}
		break;

	case ui::TextField::EventType::DETACH_WITH_IME://输入结束时触发
		{
			detachWithIMEEvent(pSender);
		}
		break;

	case ui::TextField::EventType::INSERT_TEXT://插入文字时触发
		insertTextEvent(pSender);
		break;

	case ui::TextField::EventType::DELETE_BACKWARD://删除文字时触发
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("delete word")->getCString());
		deleteBackwardEvent(pSender);
		break;

	default:
		break;
	}
}

void RoleCreateLayer::attachWithIMEEvent(Ref* pSender)
{

}

void RoleCreateLayer::detachWithIMEEvent(Ref* pSender)
{

}


void RoleCreateLayer::insertTextEvent(Ref* pSender)
{

}


void RoleCreateLayer::deleteBackwardEvent(Ref* pSender)
{

}

void RoleCreateLayer::joinGame()
{
	GameLoading::runGameLoading(200, 1);
	//this->removeFromParent();
}