#include "AttackSkillSystem.h"

static AttackSkillSystem* _attackSkillSystem = NULL;

AttackSkillInfo::AttackSkillInfo(const rapidjson::Value &json)
{
	m_skillNumber = json["skillNumber"].GetUint();
	m_skillName = json["skillName"].GetString();
	m_skillType=json["skillType"].GetUint();
	m_attackType= json["attackType"].GetUint();
	m_skillLevel=json["skillLevel"].GetUint();
	m_upgradeTrainingPoint=json["upgradeTrainingPoint"].GetUint();
	m_openLevel=json["openLevel"].GetUint();
	m_magicConsumption=json["magicConsumption"].GetUint();
	m_coolingTime=json["coolingTime"].GetDouble();//getFloat
	m_attackDistance=json["attackDistance"].GetUint();
	m_flightSpeed=json["flightSpeed"].GetUint();

	int b =json["isTailing"].GetInt();
	m_isTailing = (b == 0 ? false:true);
	m_explosionRadius=json["explosionRadius"].GetUint();
	m_explosionFanAngle=json["explosionFanAngle"].GetUint();

	b = json["isThirdParty"].GetInt();
	m_isThirdParty= (b == 0 ? false:true);

	m_effectiveTime=json["effectiveTime"].GetUint();
	m_effectOfCamp=json["effectOfCamp"].GetUint();
	m_locusSpecificID=json["locusSpecificID"].GetUint();
	m_explosionSpecificID=json["explosionSpecificID"].GetUint();
	m_casterSpecificID=json["casterSpecificID"].GetUint();

	b = json["isAttackBody"].GetInt();
	m_isAttackBody= (b == 0 ? false:true);
	
}

AttackSkillInfo::~AttackSkillInfo()
{

}

AttackSkillSystem* AttackSkillSystem::shareAttackSkillSystem()
{
    if (_attackSkillSystem == NULL)
    {
        _attackSkillSystem = new AttackSkillSystem();
    }
    return _attackSkillSystem;
}

AttackSkillSystem::AttackSkillSystem()
{
    //rapidjson::Reader reader;
    rapidjson::Document json;
    string file = FileUtils::getInstance()->fullPathForFilename("game_data/skill_info.json");
    CCString* filePath = CCString::createWithContentsOfFile(file.c_str());
    CCAssert(filePath, "file is open fail!");

	json.Parse<0>(filePath->getCString());
	if (json.HasParseError()) 
	{
		log("AttackSkillSystem GetParseError %s\n",json.GetParseError());
	}
	/*
    if (!reader.parse(filePath->getCString(), json, false))
    {
        CCAssert(false, "Json::Reader Parse error!");
    }*/
    
    int nArrayCount = json.Size();// modify
    for (int i=0; i<nArrayCount; i++)
    {
		const rapidjson::Value &ArrayJson = json[i];
        this->addAttackSkillInfo(ArrayJson);
    }
    
}

AttackSkillSystem::~AttackSkillSystem()
{
    map<unsigned int, AttackSkillInfo*>::iterator itr;
    for (itr=m_attackSkillMap.begin(); itr!=m_attackSkillMap.end(); itr++)
    {
        delete itr->second;
        m_attackSkillMap.erase(itr);
    }
    _attackSkillSystem = NULL;
}

void AttackSkillSystem::addAttackSkillInfo(const rapidjson::Value& root)
{
    AttackSkillInfo* _attackSkillInfo = new AttackSkillInfo(root);
    
    _attackSkillInfo->m_coolingTime = 1.5f;
    
    unsigned int key = _attackSkillInfo->getSkillNumber();
    m_attackSkillMap[key] = _attackSkillInfo;
}

AttackSkillInfo* AttackSkillSystem::getAttackSkillInfo(const unsigned int skillID)
{
    if (_attackSkillSystem == NULL)
    {
        AttackSkillSystem::shareAttackSkillSystem();
    }
    
    map<unsigned int, AttackSkillInfo*>::iterator itr;
    
    itr = _attackSkillSystem->m_attackSkillMap.find(skillID);
    
    if (itr != _attackSkillSystem->m_attackSkillMap.end())
    {
        return (*itr).second;
    }
    
    return NULL;
}