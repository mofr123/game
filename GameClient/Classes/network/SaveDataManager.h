#ifndef __SaveDataManager__
#define __SaveDataManager__

#include "publicDef/PublicDef.h"
#include "publicDef/gameConfig.h"

class SaveDataManager
{
	SaveDataManager();

protected:
	tagSaveUserInfo				m_SaveUserInfo;				//�û���Ϣ
	tagSaveServerInfo			m_SaveServerInfo;			//��������Ϣ

public:
	static SaveDataManager* getInstance();
	void initBasicData();			
	static void releaseData();		

public:
	//���ñ����û���Ϣ
	bool setSaveUserInfo(tagSaveUserInfo UserInfo);

	//��ñ����û���Ϣ
	bool getSaveUserInfo(tagSaveUserInfo &UserInfo);

	//��ȡ�����û���Ϣ
	bool readSaveUserFromDB();

	static int loadSaveUserCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//���÷�������Ϣ
	bool setSaveServerInfo(tagSaveServerInfo ServerInfo);

	//��÷�������Ϣ
	bool getSaveServerInfo(tagSaveServerInfo &ServerInfo);

	//��ȡ��������Ϣ
	bool readSaveServerFromDB();

	static int loadSaveServerCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	bool execTable(std::string sql);

};


#endif