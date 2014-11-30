#include "PuzzleRPC.h"

#include "publicDef/gameConfig.h"
#include "SaveDataManager.h"

#include "json/stringbuffer.h"
#include "json/writer.h"

static std::string serverUrl = "http://localhost:8099/";

PuzzleRPC* PuzzleRPC::getInstance()
{
	static PuzzleRPC* _ins = NULL;
	if (!_ins)
	{
		_ins = new PuzzleRPC();
	}
	return _ins;
}

PuzzleRPC::PuzzleRPC()
{
}

void PuzzleRPC::OnTimeOut(int seconds)
{
	HttpClient::getInstance()->setTimeoutForConnect(seconds);
	HttpClient::getInstance()->setTimeoutForRead(seconds*2);
}

int PuzzleRPC::OnTimeOut()
{
	return HttpClient::getInstance()->getTimeoutForConnect();
}

void PuzzleRPC::setServerURL(const std::string& server_endpoint)
{
	serverUrl = "http://" + serverUrl + "/";
}

void PuzzleRPC::login(const std::string& type, const std::string& udid,const std::string& password)
{
	do 
	{
		rapidjson::Document params;
		params.SetObject();
		rapidjson::Document::AllocatorType &allocator = params.GetAllocator();

		params.AddMember("type", type.c_str() , allocator);
		params.AddMember("account", udid.c_str() , allocator);
		params.AddMember("password", password.c_str() , allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		params.Accept(writer);

		DealRequest(buffer.GetString());
	} while (false);
	log("PuzzleRPC::login error !");
}

bool PuzzleRPC::DealRequest(const string &str)
{
	do 
	{
		//得到当前存储的用户信息
		tagSaveUserInfo UserInfo;
		SaveDataManager::getInstance()->getSaveUserInfo(UserInfo);

		tagSaveServerInfo ServerInfo;
		SaveDataManager::getInstance()->getSaveServerInfo(ServerInfo);

		HttpRequest* request = new HttpRequest();
		request->setUrl(serverUrl.c_str());
		request->setRequestType(HttpRequest::Type::POST);

		
		log("PuzzleRPC::DealRequest! - %s",str.c_str());

		vector<string> headers;
		headers.push_back("Content-Type: application/json; charset=utf-8");
		headers.push_back("Connection: keep-alive");
		request->setHeaders(headers);  

		//char postData[1024]={0};
		const char* postData = str.c_str();
		//sprintf(postData,"%s",str);	
		//const char* postData = "visitor=cocos2d&TestSuite=ExtensionsTest/NetworkTest";
		request->setRequestData(postData, strlen(postData));
		//创建请求Tag
		request->setTag(_NF_HTTP_TAG_CHECK_USER_EXIST_);

		request->setResponseCallback(CC_CALLBACK_2(PuzzleRPC::onHttpRequestCompleted,this));
		HttpClient::getInstance()->send(request);

		request->release();

		return true;
	} while (false);
	log("PuzzleRPC::DealRequest error!");
	return false;
}

void PuzzleRPC::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	do 
	{
		if (!response)
		{
			log("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))
		{
			//log("%s completed", response->getHttpRequest()->getTag());
		}

		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_CHECK_USER_EXIST_)!=0)
		{
			log("ERROR: %s Connect Error!",_NF_HTTP_TAG_CHECK_USER_EXIST_);
			break ;
		}

		if (response->getResponseCode() != 200)
		{
			log("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			log("ERROR: response failed");
			log("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		log("check_user_exist =\n %s",szInput.c_str());
		//{"data":{"existUser":0},"errno":0,"error":"","error_data":"null","server_time":1393467132,"server":""}

		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());
		if (json.HasParseError()) 
		{
			log("Fun Register Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)
		{
			//tagServerListInfo ServerListInfo;

			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			int	nExistUser = DICTOOL->getIntValue_json(DataJson,"existUser",-1);
			if (nExistUser==0)	
			{
				log("PuzzleRPC::onHttpRequestCompleted:%d line ",157);
				// 发消息通知
				//SendMsg(1,NULL,0);
			}
			else if (nExistUser==-1)		//角色存在，则跳转到角色选择界面
			{
				//CNFServerRoleSelectInfoVec RoleSelectVec;

				//角色数组
				int nRoleArrayCount = DICTOOL->getArrayCount_json(DataJson,"Hero");
				for (int j = 0; j < nRoleArrayCount; j++)
				{
					const rapidjson::Value &RoleArray = DICTOOL->getSubDictionary_json(DataJson,"Hero", j);

					//tagServerRoleSelectInfo Info;
					//Info.nRoleID = DICTOOL->getIntValue_json(RoleArray, "Hero_id");
					//RoleSelectVec.push_back(Info);
				}
				//ServerDataManager::getInstance()->setRoleSelectInfo(NULL);
			}	
		}
		else
		{
			break;
		}
		OnConnectRight();
		return;
	} while (false);
	log("Fun PuzzleRPC::onHttpRequestCompleted Error");
	OnConnectError();
}


void PuzzleRPC::OnConnectRight()
{
	log("Connect : Connect Right!");
}
void PuzzleRPC::OnConnectError()
{
	log("Connect : Connect Error!");
}