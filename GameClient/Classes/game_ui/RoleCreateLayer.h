#ifndef __mir9__RoleCreateLayer__
#define __mir9__RoleCreateLayer__

#include "publicDef/PublicDef.h"

class Figure;

class  RoleCreateLayer : public Layer
{
	Ref* m_selected;

	Vector<Figure*> m_roleArray;

	ui::Button* m_textButton;

	ui::TextField* m_textField;

	std::string m_nickName;

	int m_roleID;
public:
	 RoleCreateLayer();
	virtual~ RoleCreateLayer();

	CREATE_FUNC(RoleCreateLayer);
	bool init();

	void joinGame();
protected:
	void runActionsForFigure(Ref* pSender);

	void setSelector(Ref* pSender, Control::EventType controlEvent);

	void randomNickName(Ref *pSender, ui::Widget::TouchEventType type);

	void sendMessage(Ref *pSender, ui::Widget::TouchEventType type);

	void eventListenerTextField(Ref* pSender,ui::TextField::EventType type);

	void attachWithIMEEvent(Ref* pSender);

	void detachWithIMEEvent(Ref* pSender);

	void insertTextEvent(Ref* pSender);

	void deleteBackwardEvent(Ref* pSender);
};



#endif