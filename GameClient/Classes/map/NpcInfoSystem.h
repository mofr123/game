#ifndef __mir9__NpcInfoSystem__
#define __mir9__NpcInfoSystem__


#include "publicDef/PublicDef.h"

#include "json/rapidjson.h"

class NpcInfo {

    CC_SYNTHESIZE_READONLY(unsigned int, m_nID, ID);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nSID, SID);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nQID, QID);
    
    CC_SYNTHESIZE_READONLY(unsigned int, m_nRID, RID);
    
    CC_SYNTHESIZE_READONLY(string, m_sSentence, Sentence);
    
    CC_SYNTHESIZE_READONLY(string, m_sName, NpcName);
    
public:
    
    NpcInfo(const rapidjson::Value &json);
    
    virtual ~NpcInfo();
};

class NpcInfoSystem {
    
    map<unsigned int, NpcInfo*> m_npcInfoMap;
    
    void addNpcInfo(const rapidjson::Value &json);
    
public:
    
    NpcInfoSystem();
    
    virtual ~NpcInfoSystem();
    
    static NpcInfoSystem* shareNpcInfoSystem();
    
    static NpcInfo* getNpcInfo(const unsigned int npcID);
};

#endif /* defined(__mir9__NpcInfoSystem__) */
