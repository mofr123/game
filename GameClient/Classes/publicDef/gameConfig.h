#ifndef mir_mobile_gameConfig_h
#define mir_mobile_gameConfig_h

#include "PublicDef.h"

#define MAIN_FONT_NAME "Arial"


/************************************************************************/
/*					ͨ�ű�ǩ�궨��                                      */
/************************************************************************/
#define				_NF_HTTP_TAG_REGISTER_				"http_register"				//ע��
#define				_NF_HTTP_TAG_LOGIN_					"http_login"				//��¼
#define				_NF_HTTP_TAG_SERVERLIST_			"http_serverlist"			//�������б�
#define				_NF_HTTP_TAG_CHECK_USER_EXIST_		"http_check_user_exist"		//�û����
#define				_NF_HTTP_TAG_CREATE_HERO_			"http_create_hero"			//������ɫ
#define				_NF_HTTP_TAG_START_GAME_			"http_start_game"			//��ʼ��Ϸ

/************************************************************************/
/*				ͨѶ�ࣺ
				�������ӷ������õ���Ϣ								*/
/************************************************************************/
//���幫�ù��캯��
#define HTTP_CREATE(__TYPE__) \
	static bool SendMsgToServer(Node* pOwner,void *pDataBuffer) \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
	if (pRet!=NULL && pRet->init()) \
{ \
	pRet->autorelease(); \
	pOwner->addChild(pRet,100000); \
	pRet->SendHttpMsg(pOwner,pDataBuffer); \
	return true; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return false; \
} \
}



/************************************************************************/
/*						http��Ϣ���ͽṹ��                                                                     */
/************************************************************************/
//ע��
struct THttpRegister 
{
	string			strUserName;		//�û�����
	string			strPassWord;		//�û�����
};

//��¼
struct THttpLogin
{
	string			strUserName;		//�û�����
	string			strPassWord;		//�û�����
};

//������ɫ
struct TTHttpCreateHero
{
	int				nHeroID;			//��ɫID
	string			strHeroName;		//��ɫ����
};


/************************************************************************/
/*					�洢���ݽṹ��                                       */
/************************************************************************/
//�û�����������
struct tagSaveUserInfo 
{
	string		strOpenID;					//�û�ID��Ψһ��ʶ
	string		strUserName;				//�û�����
	string		strPassWord;				//�û�����
};

//��������Ϣ
struct tagSaveServerInfo
{
	int			nServerID;					//������ID
	string		strServerName;				//����������
};



//��Ϣ��ǩ�����ݷ��������ݣ����в���
enum 
{
	enHttpMsg_CheckUserExist = 1,				//�û���ɫ���
	enHttpMsg_CreateHero,	
};

#endif