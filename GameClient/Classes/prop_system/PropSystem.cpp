#include "PropSystem.h"

static PropSystem* _propSystem = NULL;

PropInfo::PropInfo(const rapidjson::Value &json)
{
	m_propNumber = json["ID"].GetInt();
	m_iconNumber = json["Icon"].GetInt();
	m_avatarNumber = json["AvatarID"].GetInt();
	m_propName = json["Name"].GetString();
	m_propType = json["Type"].GetInt();
	m_depict = json["Depict"].GetString();
	m_levelRequirements = json["Nlevel"].GetInt();
	m_attackRequirements = json["Nattack"].GetInt();
	m_magicRequirements = json["Nmaige"].GetInt();
	m_taoismRequirements = json["Ntaoism"].GetInt();
	m_gender = json["Gender"].GetInt();
	m_lasting = json["Lasting"].GetInt();
	m_weight = json["Weight"].GetInt();
	m_specialRequirements = json["Nspecial"].GetInt();
	m_coin = json["Coin"].GetInt();
	m_accurate = json["Accurate"].GetInt();
	m_dodge = json["Dodge"].GetInt();
	m_magicDodge = json["Mdodge"].GetInt();
	m_defenseMax = json["MaxDefense"].GetInt();
	m_defenseMin = json["MinDefense"].GetInt();
	m_magicDefenseMax = json["MaxMDefense"].GetInt();
	m_magicDefenseMin = json["MinMDefense"].GetInt();
	m_attackMax = json["MaxAttack"].GetInt();
	m_attackMin = json["MinAttack"].GetInt();
	m_magicMax = json["MaxMaige"].GetInt();
	m_magicMin = json["MinMaige"].GetInt();
	m_taoismMax = json["MaxTaoism"].GetInt();
	m_taoismMin = json["MinTaoism"].GetInt();
	m_lucky = json["Lucky"].GetInt();
	m_SE = json["SE"].GetInt();
	m_JS = json["JS"].GetInt();
}

PropInfo::~PropInfo()
{

}

PropSystem* PropSystem::sharePropSystem()
{
    if (_propSystem == NULL)
    {
        _propSystem = new PropSystem();
    }
    return _propSystem;
}

PropSystem::PropSystem()
{
    //CSJson::Reader reader;
    //CSJson::Value json;
	rapidjson::Document json;
    string file = FileUtils::getInstance()->fullPathForFilename("game_data/prop_info.json");
    CCString* filePath = CCString::createWithContentsOfFile(file.c_str());
    CCAssert(filePath, "file is open fail!");
    json.Parse<0>(filePath->getCString());
	if (json.HasParseError()) 
	{
		log("PropSystem GetParseError %s\n",json.GetParseError());
	}
	
	int nArrayCount = json.Size();
    for (int i=0; i<nArrayCount; i++)
    {
		const rapidjson::Value &ArrayJson = json[i];
        this->addPropInfo(ArrayJson);
    }
    
}

PropSystem::~PropSystem()
{
    map<unsigned int, PropInfo*>::iterator itr;
    for (itr=m_propMap.begin(); itr!=m_propMap.end(); itr++)
    {
        delete itr->second;
        m_propMap.erase(itr);
    }
    _propSystem = NULL;
}

void PropSystem::addPropInfo(const rapidjson::Value &json)
{
    PropInfo* _propInfo = new PropInfo(json);

    unsigned int key = _propInfo->getPropNumber();
    m_propMap[key] = _propInfo;
}

PropInfo* PropSystem::getPropInfo(const unsigned int propID)
{
    if (_propSystem == NULL)
    {
        PropSystem::sharePropSystem();
    }
    
    map<unsigned int, PropInfo*>::iterator itr;
    
    itr = _propSystem->m_propMap.find(propID);
    
    if (itr != _propSystem->m_propMap.end())
    {
        return (*itr).second;
    }
    
    return NULL;
}
