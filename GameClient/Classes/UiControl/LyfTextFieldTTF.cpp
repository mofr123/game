#include "LyfTextFieldTTF.h"

//NS_CC_BEGIN

const static float DELTA = 20.0f;

static int _calcCharCount(const char * pszText) {
	int n = 0;
	char ch = 0;
	while ((ch = *pszText)) {
		CC_BREAK_IF(! ch);
        
		if (0x80 != (0xC0 & ch)) {
			++n;
		}
		++pszText;
	}
	return n;
}

LyfTextFieldTTF::LyfTextFieldTTF()
:m_pCursorSprite(0)
,m_pCursorAction(0)
,m_nMaxLength(60)
,m_nSize(Size::ZERO)

,m_isInputState(false)
,m_nbSwallowsTouches(false)
,m_IsRehisterTouchDispatcher(false)
,m_nType(0)
{
    m_beginPos= Point::ZERO;
}

LyfTextFieldTTF::~LyfTextFieldTTF() {
}

LyfTextFieldTTF * LyfTextFieldTTF::textFieldWithPlaceHolder( const char *placeholder, const Size& dimensions,
                                              CCTextAlignment alignment, const char *fontName, float fontSize) {
    
	LyfTextFieldTTF *pRet = new LyfTextFieldTTF();
    
	if (pRet && pRet->initWithPlaceHolder("", CCSizeZero, alignment, fontName,
                                          fontSize))
    {
        pRet->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		pRet->initCursor(fontSize);
        pRet->setSize(dimensions);
        if (placeholder)
        {
			pRet->setPlaceHolder(placeholder);
		}
        pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

LyfTextFieldTTF * LyfTextFieldTTF::textFieldWithPlaceHolder(
                   const char *placeholder, const char *fontName, float fontSize) {
    
	LyfTextFieldTTF *pRet = new LyfTextFieldTTF();
	if (pRet)// && pRet->initWithString("", fontName, fontSize)
    {
        pRet->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		pRet->initCursor(fontSize);
        if (placeholder)
        {
			pRet->setPlaceHolder(placeholder);
		}
        pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void LyfTextFieldTTF::setPlaceHolder(const char *text){
    
    TextFieldTTF::setPlaceHolder(text);
    Rect rect=CCRectZero;
    rect.origin.y=(this->getContentSize().height-this->getSize().height)/2;
    //rect.size=Size(this->getTextureRect().size.width, this->getSize().height);
    //this->setTextureRect(rect);
    this->setContentSize(this->getSize());
}

void LyfTextFieldTTF::initCursor(int height) {
	// 初始化光标
    //
	int column = 4;
	int **pixels;
	//int pixels[height][column];

	 pixels = new int*[height]; //申请行的空间
	 for(int i=0; i < height; i++)//每行的列申请空间
	 {
		 pixels[i] = new int[column];
	 }
    
	for (int i = 0; i < height; ++i)
    {
		for (int j = 0; j < column; ++j)
        {
			pixels[i][j] = 0xffffffff;
		}
	}
    
	Texture2D *texture = new Texture2D();
	//kCCTexture2DPixelFormat_RGB888,the secth args
	texture->initWithData(pixels,1, Texture2D::PixelFormat::RGB888, 1, 1,
                          CCSizeMake(column, height));
    
	m_pCursorSprite = Sprite::createWithTexture(texture);
	Size winSize = getContentSize();
	Point cursorPos = ccp(0, winSize.height / 2);
	m_pCursorSprite->setPosition(cursorPos);
	//this->addChild(m_pCursorSprite);
    
	m_pCursorAction = RepeatForever::create((ActionInterval *) Sequence::create(FadeOut::create(0.40f),
                                                                                      FadeIn::create(0.40f), NULL));
    
	m_pCursorSprite->runAction(m_pCursorAction);
	m_pCursorSprite->setVisible(false);
}

void LyfTextFieldTTF::setContentSize(const Size &contentSize){
    
    TextFieldTTF::setContentSize(contentSize);
}

void LyfTextFieldTTF::onEnter() {
    
	TextFieldTTF::onEnter();
	//this->registerWithTouchDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches( true);
	listener->onTouchBegan = CC_CALLBACK_2 (LyfTextFieldTTF ::onTouchBegan, this );
	//listener->onTouchMoved = CC_CALLBACK_2 (LyfTextFieldTTF ::onTouchMoved, this );
	listener->onTouchEnded = CC_CALLBACK_2 (LyfTextFieldTTF ::onTouchEnded, this );
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void LyfTextFieldTTF::onExit() {
    
	this->detachWithIME();
	//this->unRegisterWithTouchDispatcher();
    TextFieldTTF::onExit();
}

bool LyfTextFieldTTF::isInTextField(Touch *pTouch) {
    
    Size size = getSize();
	Rect rect =
    Rect(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);
	Point pTouchPt = convertTouchToNodeSpaceAR(pTouch);
	return rect.containsPoint(pTouchPt);
}

bool LyfTextFieldTTF::onTouchBegan(Touch *pTouch, Event *pEvent) {
    
    m_touchBeginUIPoint = pTouch->getLocationInView();
	m_beginPos = pTouch->getLocation();
    
    if (!this->isVisible())
        return false;
    
    if (!m_isInputState)
    {
        if (isInTextField(pTouch))
            return true;
        
        return false;
    }
    
	return true;
}

void LyfTextFieldTTF::onTouchEnded(Touch *pTouch, Event *pEvent) {
    
	log("CNewEditBox::ccTouchEnded");
	Vec2 endPos = pTouch->getLocation();
    
	// is click event?
	if (::abs(endPos.x - m_beginPos.x) > DELTA || ::abs(endPos.y - m_beginPos.y) > DELTA)
    {
		// not is click event
		m_beginPos.x = m_beginPos.y = -1; 
		return;
	}
    
    //	CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height); 
	//is open input text or close inputText
	isInTextField(pTouch) ? attachWithIME() : detachWithIME();
}

bool LyfTextFieldTTF::attachWithIME() {
    
	if (TextFieldTTF::attachWithIME())
    {
		m_pCursorSprite->setVisible(true);
        
		if (_inputText.size() <= 0)//m_pInputText
        {
			//CCLabelTTF::setString("");
			Label::create()->setString("");
            m_pCursorSprite->setPosition(Point(-getSize().width*getAnchorPoint().x, getSize().height*(0.5-getAnchorPoint().y)));
            this->setContentSize(Size::ZERO);
        }
        m_isInputState=true;
		return true;
	}
	return false;
}

bool LyfTextFieldTTF::detachWithIME() {
    
	if (TextFieldTTF::detachWithIME())//after rec user input text
    {
		m_pCursorSprite->setVisible(false);
        
		//if (m_pInputText && m_pInputText->length() <= 0)
		if(_inputText.size()<=0)
        {
			//LabelTTF::setString(m_pPlaceHolder);//m_pPlaceHolder
			Label::create()->setString(_placeHolder);
            Rect rect=Rect::ZERO;//CCRectZero
            rect.origin.y=(this->getContentSize().height-this->getSize().height)/2;
            //rect.size=Size(this->getTextureRect().size.width, this->getSize().height);
            //this->setTextureRect(rect);
            this->setContentSize(this->getSize());
        }
        m_isInputState=false;
		return true;
	}
	return false;
}

void LyfTextFieldTTF::deleteBackward() {
    
	int nStrLen = _inputText.size();
    
	if (!nStrLen)
    {
		// there is no string
		return;
	}
    
	// get the delete byte number
	int nDeleteLen = 1; // default, erase 1 byte
    
	while (0x80 == (0xC0 & _inputText.at(nStrLen - nDeleteLen)))
    {
		++nDeleteLen;
	}
    
	if (_delegate
        && _delegate->onTextFieldDeleteBackward(this,
                                                  _inputText.c_str() + nStrLen - nDeleteLen, nDeleteLen))
    {
		// delegate doesn't wan't to delete backwards
		return;
	}
    
	// set new input text
	std::string sText(_inputText.c_str(), nStrLen - nDeleteLen);
	setString(sText.c_str());
}

void LyfTextFieldTTF::setString(const char *text) {
    
    
    
	static char bulletString[] = { (char) 0xe2, (char) 0x80, (char) 0xa2,
        (char) 0x00 };
	std::string displayText;
	int length;
    
	//CC_SAFE_DELETE(_inputText);
    
	if (text) {
		_inputText = *(new std::string(text));//
		displayText = _inputText;
		if (_secureTextEntry)
        {
			displayText = "";
			length = _inputText.size();
			while (length)
            {
				displayText.append(bulletString);
				--length;
			}
		}
	}
    else
    {
		_inputText = *(new std::string());
	}
    
	// if there is no input text, display placeholder instead
    
	if (_inputText.size()<0)
    {
		if (m_pCursorSprite && m_pCursorSprite->isVisible())
        {
			//CCLabelTTF::setString("");
			Label::create()->setString("");
        }
		else
        {
			//CCLabelTTF::setString(m_pPlaceHolder->c_str());
			Label::create()->setString(_placeHolder);
        }
	}
    else
    {
        if (_inputText.size()<m_nMaxLength)
        {
            //CCLabelTTF::setString(displayText.c_str());
			Label::create()->setString(displayText);
        }
        else
        {
            displayText.assign(_inputText, _inputText.size()-m_nMaxLength, m_nMaxLength);
            //CCLabelTTF::setString(displayText.c_str());
			Label::create()->setString(displayText);
		}
	}
	_charCount = _calcCharCount(_inputText.c_str());
    
    float width=this->getSize().width;
    float height=this->getSize().height;
    float x=this->getContentSize().width;
    float y=height/2;
    
    if (x>width)
    {
        x=width;
    }
    
    if (m_pCursorSprite)
    {
		m_pCursorSprite->setPosition(CCPoint(x, y));
	}
    
    if (x==width)
    {
        x=this->getContentSize().width-x;
    }
    else
    {
        x=0;
        width=this->getContentSize().width;
    }
    
    y=(this->getContentSize().height-height)/2;
    
    Rect rect=Rect(x, y, width, height);
    
    //this->setTextureRect(rect);
    this->setContentSize(this->getSize());
    
    if (strlen(text) > m_nMaxLength)
    {
        this->deleteBackward();
    }
};


//NS_CC_END
