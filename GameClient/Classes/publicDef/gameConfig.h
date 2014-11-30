#ifndef mir_mobile_gameConfig_h
#define mir_mobile_gameConfig_h

#include "PublicDef.h"

#define MAIN_FONT_NAME "Arial"


/************************************************************************/
/*					通信标签宏定义                                      */
/************************************************************************/
#define				_NF_HTTP_TAG_REGISTER_				"http_register"				//注册
#define				_NF_HTTP_TAG_LOGIN_					"http_login"				//登录
#define				_NF_HTTP_TAG_SERVERLIST_			"http_serverlist"			//服务器列表
#define				_NF_HTTP_TAG_CHECK_USER_EXIST_		"http_check_user_exist"		//用户检测
#define				_NF_HTTP_TAG_CREATE_HERO_			"http_create_hero"			//创建角色
#define				_NF_HTTP_TAG_START_GAME_			"http_start_game"			//开始游戏

/************************************************************************/
/*				通讯类：
				负责连接服务器得到信息								*/
/************************************************************************/
//定义公用构造函数
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
/*						http消息发送结构体                                                                     */
/************************************************************************/
//注册
struct THttpRegister 
{
	string			strUserName;		//用户名称
	string			strPassWord;		//用户密码
};

//登录
struct THttpLogin
{
	string			strUserName;		//用户名称
	string			strPassWord;		//用户密码
};

//创建角色
struct TTHttpCreateHero
{
	int				nHeroID;			//角色ID
	string			strHeroName;		//角色名称
};


/************************************************************************/
/*					存储数据结构体                                       */
/************************************************************************/
//用户姓名和密码
struct tagSaveUserInfo 
{
	string		strOpenID;					//用户ID，唯一标识
	string		strUserName;				//用户姓名
	string		strPassWord;				//用户密码
};

//服务器信息
struct tagSaveServerInfo
{
	int			nServerID;					//服务器ID
	string		strServerName;				//服务器名称
};



//消息标签：根据服务器数据，进行操作
enum 
{
	enHttpMsg_CheckUserExist = 1,				//用户角色检测
	enHttpMsg_CreateHero,	
};

#endif