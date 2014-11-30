#ifndef _PUBLIC_DEF_HEAD_
#define _PUBLIC_DEF_HEAD_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <iostream>

USING_NS_CC;
USING_NS_CC_EXT;


using namespace std;
using namespace cocostudio;


#define SCREEN_WIDTH		Director::getInstance()->getWinSize().width			//屏幕宽(指定分辨率)
#define SCREEN_HEIGHT		480													//屏幕高(指定分辨率)
#define SCREEN_CENTER		ccp(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f)			//屏幕中心
#define NAME_LEN			128
#define _NF_ABS(x)			(    (x)>=0 ? (x) : -(x)   )



class ToUTF8
{
private:
	void StrToUTF8(std::string& dest, const std::wstring& src);
public:
	
	ToUTF8();
	~ToUTF8();

	std::string WStrToUTF8(const std::wstring& str);

	static ToUTF8* getInstance();

	void destroyInstance();
};



//消息接收
class MsgReceiver
{
public:
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize)
	{

	}
};

//消息发送
class MsgSender
{
protected:
	MsgReceiver * m_pReceiver;;

public:
	void SetMsg(MsgReceiver * pReceiver)
	{
		m_pReceiver = pReceiver;
	}

	bool SendMsg(int nMsgID,void* pInfo,int nSize)
	{
		if (m_pReceiver == NULL)
		{
			return false;
		}
		else
		{
			m_pReceiver->RecMsg(nMsgID,pInfo,nSize);
			return true;
		}
	}
};


//静态数据管理
#define _TR_STATIC_DATABASE_FILE_NAME_		"nf_static.db"
std::string getStaticDBName();

//存储数据管理
#define _NF_SAVE_DATABASE_FILE_NAME_		"nf_save.db"
std::string getSaveDBName();


#endif