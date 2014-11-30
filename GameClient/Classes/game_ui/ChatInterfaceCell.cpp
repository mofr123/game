#include "ChatInterfaceCell.h"
#include "ChatInterface.h"


ChatInterfaceCell::ChatInterfaceCell(unsigned int lenght)
	:m_lenght(lenght)
	,m_chatTypeTTF(NULL)
	,m_speakerTTF(NULL)
	,m_contentTTF(NULL)
{

}

ChatInterfaceCell::~ChatInterfaceCell()
{

}

ChatInterfaceCell* ChatInterfaceCell::create(unsigned int lenght)
{
	ChatInterfaceCell* cell = new ChatInterfaceCell(lenght);
	if (cell && cell->init())
	{
		cell->setTouchEnabled(false);
		cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return NULL;
}

void ChatInterfaceCell::setString(ChatInfoObject& object)
{
	m_object = &object;

	unsigned int ttfSize = 20;

	if (m_chatTypeTTF == NULL)
	{
		m_chatTypeTTF = Label::create("", "Arial", ttfSize);
		m_chatTypeTTF->setAnchorPoint(Point(0, 1.0f));
		this->addChild(m_chatTypeTTF);
	}
	if (m_speakerTTF == NULL)
	{
		m_speakerTTF = Label::create("", "Arial", ttfSize);
		m_speakerTTF->setAnchorPoint(Point(0, 1.0f));
		this->addChild(m_speakerTTF);
	}

	string _speaker = string(m_object->nickName);
	if (_speaker.compare("") != 0)
	{
		_speaker = _speaker + ":";
	}
	string str;
	switch (m_object->chatType)
	{
	case CHAT_TAG_GUILD:
		str = ToUTF8::getInstance()->WStrToUTF8(L"【行会】");
		m_chatTypeTTF->setString(str);
		break;
	case CHAT_TAG_SYSTEM:
		{
			str = ToUTF8::getInstance()->WStrToUTF8(L"【系统】");
			m_chatTypeTTF->setString(str);
			_speaker = "";
		}
		break;
	case CHAT_TAG_WORLD:
		str = ToUTF8::getInstance()->WStrToUTF8(L"【世界】");
		m_chatTypeTTF->setString(str);
		break;
	default:
		m_chatTypeTTF->setString("");
		break;
	}


	m_speakerTTF->setString(_speaker.c_str());

	if (m_contentTTF)
	{
		m_contentTTF->removeFromParent();
		m_contentTTF = NULL;
	}
	unsigned int lenght = m_lenght;
	unsigned int height = ttfSize+1;
	unsigned int row = 1;
	lenght = lenght - m_chatTypeTTF->getContentSize().width;
	if (m_speakerTTF)
	{
		lenght = lenght - m_speakerTTF->getContentSize().width;
	}

	Label* tmp = Label::create(m_object->content.c_str(), "Arial", ttfSize);
	row = (unsigned int)ceilf(tmp->getContentSize().width/lenght);
	height = MAX(height, row * (tmp->getContentSize().height));
	m_contentTTF = Label::create(m_object->content.c_str(), "Arial", ttfSize, Size(lenght, height+1), kCCTextAlignmentLeft);
	m_contentTTF->setAnchorPoint(Point(0, 1.0f));
	this->addChild(m_contentTTF);

	if (m_object->content.compare("") != 0)
	{

	}

	if (m_chatTypeTTF)
	{
		m_chatTypeTTF->setPosition(Point(0, height));
	}
	if (m_speakerTTF)
	{
		m_speakerTTF->setPosition(Point(m_chatTypeTTF->getContentSize().width, height));
	}
	if (m_contentTTF)
	{
		m_contentTTF->setPosition(Point(m_chatTypeTTF->getContentSize().width+m_speakerTTF->getContentSize().width, height));
	}
	this->setContentSize(Size(m_lenght, m_contentTTF->getContentSize().height));

	Color3B color;;

	if (m_object->chatType == CHAT_TAG_GUILD)
	{
		color = Color3B::BLUE;
	}
	else if (m_object->chatType == CHAT_TAG_SYSTEM)
	{
		color = Color3B::RED;
	}
	else
	{
		color = Color3B::WHITE;
	}

	m_chatTypeTTF->setColor(color);
	m_speakerTTF->setColor(color);
	m_contentTTF->setColor(color);
}