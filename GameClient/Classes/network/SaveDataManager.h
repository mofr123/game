#ifndef __SaveDataManager__
#define __SaveDataManager__

#include "publicDef/PublicDef.h"
#include "publicDef/gameConfig.h"

class SaveDataManager
{
	SaveDataManager();

protected:
	tagSaveUserInfo				m_SaveUserInfo;				//用户信息
	tagSaveServerInfo			m_SaveServerInfo;			//服务器信息

public:
	static SaveDataManager* getInstance();
	void initBasicData();			
	static void releaseData();		

public:
	//设置本地用户信息
	bool setSaveUserInfo(tagSaveUserInfo UserInfo);

	//获得本地用户信息
	bool getSaveUserInfo(tagSaveUserInfo &UserInfo);

	//读取本地用户信息
	bool readSaveUserFromDB();

	static int loadSaveUserCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//设置服务器信息
	bool setSaveServerInfo(tagSaveServerInfo ServerInfo);

	//获得服务器信息
	bool getSaveServerInfo(tagSaveServerInfo &ServerInfo);

	//读取服务器信息
	bool readSaveServerFromDB();

	static int loadSaveServerCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	bool execTable(std::string sql);

};


#endif