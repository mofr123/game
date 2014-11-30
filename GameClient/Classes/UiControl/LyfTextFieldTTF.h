#ifndef __mir_mobile__LTextField__
#define __mir_mobile__LTextField__

#include "publicDef/PublicDef.h"

//NS_CC_BEGIN

class LyfTextFieldTTF: public TextFieldTTF//, public CCTargetedTouchDelegate 
{
    
public:
    
	LyfTextFieldTTF();
    
	virtual ~LyfTextFieldTTF();
    
	static LyfTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const Size& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
	static LyfTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
    
	virtual void onEnter();
    
	virtual void onExit();
    
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
	/**
	 @brief    Open keyboard and receive input text.
	 */
	virtual bool attachWithIME();
    
	/**
	 @brief    End text input and close keyboard.
	 */
	virtual bool detachWithIME();
    
	virtual void setString(const char *text);
    
	virtual void deleteBackward();
    
    virtual void setPlaceHolder(const char * text);
    
    //virtual void setTouchDispatcher(bool bSwallowsTouches);
    
   // void registerWithTouchDispatcher();
    
    //void unRegisterWithTouchDispatcher();
    
    CC_SYNTHESIZE(int, m_nMaxLength, MaxLength);
    
    CC_SYNTHESIZE(Size, m_nSize, Size);
    
    CC_SYNTHESIZE(int, m_nType, Type);
    
private:
    
	void initCursor(int height);
    
	bool isInTextField(Touch *pTouch);
    
    void setContentSize(const Size &contentSize);
    
private:
	// click start pos
	Point m_beginPos;
    
	// Cursor Sprite
	Sprite *m_pCursorSprite;
    
	// Cursor Action
	Action *m_pCursorAction;
    
    bool m_isInputState;
    
    bool m_nbSwallowsTouches;
    
    bool m_IsRehisterTouchDispatcher;
    
    Point m_touchBeginUIPoint;
};

//NS_CC_END

#endif /* defined(__mir_mobile__LTextField__) */
