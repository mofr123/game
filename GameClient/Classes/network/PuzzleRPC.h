#ifndef __PuzzleRPC__
#define __PuzzleRPC__

#include "publicDef/PublicDef.h"
#include "network/HttpClient.h"


using namespace cocos2d::network;

class PuzzleRPC : public cocos2d::Ref
{
public:
	static void setServerURL(const std::string& server_endpoint);

public:
	static PuzzleRPC* getInstance();

	PuzzleRPC();


	bool isHostNameReachable(const std::string& hostName);

	//���ӳ�ʱ
	int OnTimeOut();
	void OnTimeOut(int seconds);

	//������ȷ
	void OnConnectRight();

	//���Ӵ���
	void OnConnectError();

	virtual bool DealRequest(const string &str);

	//Http Response��Ϣ�ص�
	virtual void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);

	void login(const std::string& type, const std::string& udid,const std::string& password);

	void registerUser(const std::string& udid, const std::string password);

};


#endif