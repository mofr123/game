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


#define SCREEN_WIDTH		Director::getInstance()->getWinSize().width			//��Ļ��(ָ���ֱ���)
#define SCREEN_HEIGHT		480													//��Ļ��(ָ���ֱ���)
#define SCREEN_CENTER		ccp(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f)			//��Ļ����
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



//��Ϣ����
class MsgReceiver
{
public:
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize)
	{

	}
};

//��Ϣ����
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


//��̬���ݹ���
#define _TR_STATIC_DATABASE_FILE_NAME_		"nf_static.db"
std::string getStaticDBName();

//�洢���ݹ���
#define _NF_SAVE_DATABASE_FILE_NAME_		"nf_save.db"
std::string getSaveDBName();


#endif