#include "NpcInfoSystem.h"

static NpcInfoSystem* _npcInfoSystem = NULL;

NpcInfo::NpcInfo(const rapidjson::Value &json)
:m_nID(json["ID"].GetUint())
,m_nSID(json["SID"].GetUint())
,m_nQID(json["QID"].GetUint())
,m_nRID(json["RID"].GetUint())
,m_sSentence(json["Sentence"].GetString())
,m_sName(json["Name"].GetString())
{
    log("%s,%s",getNpcName().c_str(), json["Sentence"].GetString());
}

NpcInfo::~NpcInfo()
{

}

NpcInfoSystem* NpcInfoSystem::shareNpcInfoSystem()
{
    if (_npcInfoSystem == NULL)
    {
        _npcInfoSystem = new NpcInfoSystem();
    }
    return _npcInfoSystem;
}

NpcInfoSystem::NpcInfoSystem()
{
    //CSJson::Reader reader;
   // CSJson::Value json;
	rapidjson::Document json;
    string file = FileUtils::getInstance()->fullPathForFilename("game_data/npc_info.json");
	CCString* filePath = CCString::createWithContentsOfFile(file.c_str());
	CCAssert(filePath, "file is open fail!");
	json.Parse<0>(filePath->getCString());
    //if (!reader.parse(filePath->getCString(), json, false))
    //{
        //CCAssert(false, "Json::Reader Parse error!");
    //}
	if (json.HasParseError()) 
	{
		log("NpcInfoSystem NpcInfoSystem %s\n",json.GetParseError());
	}
    int nArrayCount = json.Size();
    for (int i=0; i<nArrayCount; i++)
    {
		const rapidjson::Value &ArrayJson = json[i];//DICTOOL->getSubDictionary_json(json,"ID",i);
        this->addNpcInfo(ArrayJson);
    }
    
}

NpcInfoSystem::~NpcInfoSystem()
{
    map<unsigned int, NpcInfo*>::iterator itr;
    for (itr=m_npcInfoMap.begin(); itr!=m_npcInfoMap.end(); itr++)
    {
        delete itr->second;
        m_npcInfoMap.erase(itr);
    }
    _npcInfoSystem = NULL;
}

void NpcInfoSystem::addNpcInfo(const rapidjson::Value &json)
{
    NpcInfo* _npcInfo = new NpcInfo(json);
    unsigned int key = _npcInfo->getID();
    m_npcInfoMap[key] = _npcInfo;
}

NpcInfo* NpcInfoSystem::getNpcInfo(const unsigned int npcID)
{
    if (_npcInfoSystem == NULL)
    {
        NpcInfoSystem::shareNpcInfoSystem();
    }
    
    map<unsigned int, NpcInfo*>::iterator itr;
    
    itr = _npcInfoSystem->m_npcInfoMap.find(npcID);
    
    if (itr != _npcInfoSystem->m_npcInfoMap.end())
    {
        return (*itr).second;
    }
    
    return NULL;
}

