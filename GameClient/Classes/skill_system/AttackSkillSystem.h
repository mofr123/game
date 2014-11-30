#ifndef __mir_mobile__AttackSkillSystem__
#define __mir_mobile__AttackSkillSystem__

#include "publicDef/PublicDef.h"

#include "json/rapidjson.h"
#include "json/document.h"

class AttackSkillSystem;

class AttackSkillInfo
{
	friend class AttackSkillSystem;

	//skill_no
	CC_SYNTHESIZE_READONLY(unsigned int, m_skillNumber, SkillNumber);

	//Skill Name
	CC_SYNTHESIZE_READONLY(string, m_skillName, SkillName);

	//Skill Type
	CC_SYNTHESIZE_READONLY(unsigned int, m_skillType, SkillType);

	//Attack Type
	CC_SYNTHESIZE_READONLY(unsigned int, m_attackType, AttackType);

	//Skill Level
	CC_SYNTHESIZE_READONLY(unsigned int, m_skillLevel, SkillLevel);

	//Upgrade Training Point
	CC_SYNTHESIZE_READONLY(unsigned int, m_upgradeTrainingPoint, UpgradeTrainingPoint);

	//Open Level
	CC_SYNTHESIZE_READONLY(unsigned int, m_openLevel, OpenLevel);

	//Magic Consumption
	CC_SYNTHESIZE_READONLY(int, m_magicConsumption, MagicConsumption);

	//Cooling Time
	CC_SYNTHESIZE_READONLY(float, m_coolingTime, CoolingTime);

	//Attack Distance
	CC_SYNTHESIZE_READONLY(unsigned int, m_attackDistance, AttackDistance);

	//Is Attack Body
	CC_SYNTHESIZE_READONLY(bool, m_isAttackBody, IsAttackBody);

	//Flight Speed
	CC_SYNTHESIZE_READONLY(unsigned int, m_flightSpeed, FlightSpeed);

	//Is Tailing
	CC_SYNTHESIZE_READONLY(bool, m_isTailing, IsTailing);

	//explosionRadius
	CC_SYNTHESIZE_READONLY(unsigned int, m_explosionRadius, ExplosionRadius);

	//fang angle
	CC_SYNTHESIZE_READONLY(unsigned int, m_explosionFanAngle, ExplosionFanAngle);

	//is Third party
	CC_SYNTHESIZE_READONLY(bool, m_isThirdParty, IsThirdParty);

	//BUFF time,normal is 0
	CC_SYNTHESIZE_READONLY(unsigned int, m_effectiveTime, EffectiveTime);

	//
	CC_SYNTHESIZE_READONLY(unsigned int, m_effectOfCamp, EffectOfCamp) // 1£¬2£¬3

		//Caster Specific
		CC_SYNTHESIZE_READONLY(unsigned int, m_casterSpecificID, CasterSpecificID);

	//Locus Specific
	CC_SYNTHESIZE_READONLY(unsigned int, m_locusSpecificID, LocusSpecificID);

	//
	CC_SYNTHESIZE_READONLY(unsigned int, m_explosionSpecificID, ExplosionSpecificID);


public:
	AttackSkillInfo(const rapidjson::Value &json);
	virtual~AttackSkillInfo();

};


class AttackSkillSystem {

	map<unsigned int, AttackSkillInfo*> m_attackSkillMap;

	void addAttackSkillInfo(const rapidjson::Value &json);

public:

	AttackSkillSystem();

	virtual ~AttackSkillSystem();

	static AttackSkillSystem* shareAttackSkillSystem();

	static AttackSkillInfo* getAttackSkillInfo(const unsigned int skillID);

};


#endif