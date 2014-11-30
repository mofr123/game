#include "SaveDataManager.h"

#include "sqlite3.h"



/************************************************************************/
/*					数据库校验                                          */
/************************************************************************/
//整数
int getIntFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name)
{
	for (int i = 0; i < n_column; i++)
	{
		if (strcmp(column_name[i],szItemName)==0)//比较
		{
			return atoi(column_value[i]);
		}
	}
	log("Can't Find int [%s]",szItemName);
	return 0;
}

//小数
float getFloatFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name)
{
	for(int i=0;i<n_column;i++)
	{
		if(strcmp(column_name[i],szItemName)==0)
		{
			return atof(column_value[i]);
		}
	}
	log("Can't Find float [%s]",szItemName);
	return 0;
}

//字符串
string getStringFromTable( char * szItemName, int n_column, char ** column_value, char ** column_name)
{
	for(int i=0;i<n_column;i++)
	{
		if(strcmp(column_name[i],szItemName)==0)
		{
			return column_value[i];
		}
	}
	log("Can't Find string [%s]",szItemName);
	return NULL;
}



static SaveDataManager * g_pSaveDataManager=NULL;
SaveDataManager::SaveDataManager()
{
}

SaveDataManager * SaveDataManager::getInstance()
{
	if(g_pSaveDataManager==NULL)
	{
		g_pSaveDataManager = new SaveDataManager();
	}
	return g_pSaveDataManager;
}
void SaveDataManager::releaseData()
{
}

void SaveDataManager::initBasicData()
{
	do 
	{
		//读取用户信息
		readSaveUserFromDB();

		//读取服务器信息
		readSaveServerFromDB();

		return ;
	} while (false);
	log("Fun SaveDataManager::initBasicData Error!");
}


bool SaveDataManager::setSaveUserInfo(tagSaveUserInfo UserInfo)
{
	char *szErrMsg = NULL;
	do 
	{
		char szSQLCmd[NAME_LEN] = {0};
		sqlite3 *pDB = NULL;			
		int result = sqlite3_open(getSaveDBName().c_str(), &pDB);
		CC_BREAK_IF(result!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		sprintf(szSQLCmd,"update t_user_info set s_open_id='%s',s_username='%s',s_password='%s'",UserInfo.strOpenID.c_str(),UserInfo.strUserName.c_str(),UserInfo.strPassWord.c_str());
		CC_BREAK_IF(sqlite3_exec(pDB,szSQLCmd,NULL, NULL, &szErrMsg )!=SQLITE_OK);
		
		sqlite3_close(pDB);

		g_pSaveDataManager->m_SaveUserInfo = UserInfo;

		return true;
	} while (false);
	log("CNFSaveDataManager::SetSaveUserInfo Run Error! Msg=%s",szErrMsg);
	return false;
}

bool SaveDataManager::getSaveUserInfo( tagSaveUserInfo &UserInfo )
{
	UserInfo = m_SaveUserInfo;
	return true;
}

bool SaveDataManager::readSaveUserFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(getSaveDBName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_user_info",SaveDataManager::loadSaveUserCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		
		return true;
	} while (false);
	log("SaveDataManager::readSaveUserFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int SaveDataManager::loadSaveUserCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pSaveDataManager==NULL)
	{
		g_pSaveDataManager = new SaveDataManager();
	} 

	tagSaveUserInfo Info;
	Info.strOpenID = getStringFromTable("s_open_id",n_column,column_value,column_name);
	Info.strUserName = getStringFromTable("s_username",n_column,column_value,column_name);
	Info.strPassWord = getStringFromTable("s_password",n_column,column_value,column_name);

	g_pSaveDataManager->m_SaveUserInfo = Info;
	return 0;
}


bool SaveDataManager::setSaveServerInfo(tagSaveServerInfo ServerInfo)
{
	char *szErrMsg=NULL;			    
	do 
	{
		char szSQLCmd[NAME_LEN]={0};

		sqlite3 *pDB = NULL;
		CC_BREAK_IF(sqlite3_open(getSaveDBName().c_str(), &pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		sprintf(szSQLCmd,"update t_server_info set s_server_id=%d,s_server_name='%s'",ServerInfo.nServerID,ServerInfo.strServerName.c_str());
		CC_BREAK_IF(sqlite3_exec(pDB,szSQLCmd,NULL, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		g_pSaveDataManager->m_SaveServerInfo = ServerInfo;

		return true;
	} while (false);
	log("SaveDataManager::setSaveServerInfo Run Error! Msg=%s",szErrMsg);
	return false;
}
bool SaveDataManager::getSaveServerInfo(tagSaveServerInfo &ServerInfo)
{
	ServerInfo = m_SaveServerInfo;
	return true;
}
bool SaveDataManager::readSaveServerFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(getSaveDBName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_server_info",SaveDataManager::loadSaveServerCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	log("SaveDataManager::readSaveServerFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}


int SaveDataManager::loadSaveServerCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pSaveDataManager==NULL)
	{
		g_pSaveDataManager = new SaveDataManager();
	} 

	tagSaveServerInfo Info;
	Info.nServerID = getIntFromTable("s_server_id",n_column,column_value,column_name);
	Info.strServerName = getStringFromTable("s_server_name",n_column,column_value,column_name);

	g_pSaveDataManager->m_SaveServerInfo = Info;
	return 0;
}

//this->execTable("insert into t_user_info(s_username,s_password) values('123456x','123456x')");
bool SaveDataManager::execTable(std::string sql)//
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pdb=NULL;//1
		//std::string path= FileUtils::getInstance()->getWritablePath()+"save.db";//2

		int result;
		result=sqlite3_open(getSaveDBName().c_str(),&pdb);//3
		if(result!=SQLITE_OK)
		{
			log("open database failed,  number%d",result);
		}

		result=sqlite3_exec(pdb,sql.c_str(),NULL,NULL,&szErrMsg);//1
		if(result!=SQLITE_OK)
			log("create table failed");

		return true;
	} while (false);
	log("SaveDataManager::execTable Run Error! Msg=%s ",szErrMsg);
	return false;
}


