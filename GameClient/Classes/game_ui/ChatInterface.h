#ifndef __mir9__ChatInterface__
#define __mir9__ChatInterface__

#include <deque>
#include "UiControl/LyfTableView.h"
#include "UiControl/LyfTextFieldTTF.h"

#include "publicDef/PublicDef.h"

#define CHAT_TAG_WORLD 0Xf01
#define CHAT_TAG_GUILD 0Xf02
#define CHAT_TAG_SYSTEM 0Xf03

class ChatInfoObject
{
public:
    
    string nickName;
    
    string content;
    
    unsigned int userID;
    
    unsigned int chatType;
    
    ChatInfoObject(unsigned int chatType, unsigned int userID, string nickName, string content)
    :chatType(chatType)
    ,userID(userID)
    ,nickName(nickName)
    ,content(content)
    {;}
    
    ~ChatInfoObject()
    {;}
};

class ChatInterface:
public LayerColor,
public LyfTableViewDataSource,
public TextFieldDelegate
{
    //init cell callback
    virtual LyfTableViewCell* tableCellAtIndex(LyfTableView *table, unsigned int idx);
    
    //get cell total num call
    virtual unsigned int numberfRowsInTableView(LyfTableView *table);
    
    virtual bool onTextFieldDetachWithIME(TextFieldTTF *sender);
    
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen);
    
   // bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
protected:
    
    Node* m_selectedBtn;
    
    Sprite* m_selected;
    
    Scale9Sprite* m_tooBar;
    
    deque<ChatInfoObject>* m_dataWorldArray;
    
    deque<ChatInfoObject>* m_dataGuildArray;
    
    deque<ChatInfoObject>* m_dataSystemArray;
    
    deque<ChatInfoObject>* m_dataArray;
    
    LyfTableView* m_worldTableView;
    
    LyfTableView* m_guildTtableView;
    
    LyfTableView* m_systemTableView;
    
    LyfTableView* m_tableView;
    
    LyfTextFieldTTF* m_textField;
    
    ControlButton* m_worldBtn;
    
    ControlButton* m_guildBtn;
    
    ControlButton* m_systemBtn;
    
    ControlButton* m_sendNewsBtn;
    
    ControlButton* m_switchSizeBtn;
    
protected:
    
    virtual void onEnterTransitionDidFinish();

   // void registerWithTouchDispatcher(void);
    
    void switchInterface(Ref* sender,Control::EventType controlEvent);
    
    void switchSize(Ref* sender,Control::EventType controlEvent);
    
    //void sendNews(CCObject* sender);
	void sendNews(Ref* sender,Control::EventType controlEvent);
    
public:
    
    ChatInterface();
    
    ~ChatInterface();
    
    CREATE_FUNC(ChatInterface);
    
    bool init();
    
    void hideChatInterface();
    
    void showChatInterface();
    
protected:
    
    ControlButton* createBtn(const char* title, Point point, int tag);
    
    LyfTableView* createTableView();
    
    void setSizeMin();
    
    void setSizeMax();
    
};


#endif /* defined(__mir9__ChatInterface__) */
