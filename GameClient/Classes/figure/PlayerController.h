#ifndef __mir_mobile__PlayerController__
#define __mir_mobile__PlayerController__

#include "publicDef/PublicDef.h"

class PlayerController: public Ref {
    
    
    CC_SYNTHESIZE(unsigned int, m_playerNumber, PlayerNumber);
    
    CC_SYNTHESIZE(unsigned int, m_hairNumber, HairNumber);
    
    CC_SYNTHESIZE(unsigned int, m_weaponsNumber, WeaponsNumber);
    
    CC_SYNTHESIZE(std::string, m_playerName, PlayerName);
    
    CC_SYNTHESIZE_READONLY(int, m_playerLevel, PlayerLevel);
    
    CC_SYNTHESIZE_READONLY(int, m_playerShowID, PlayerShowID);
    
    CC_SYNTHESIZE_READONLY(int, m_playerUID, PlayerLevelUID);
    
    CC_SYNTHESIZE_READONLY(int, m_carryingWeaponsID, CarryingWeaponsID);
    
public:
    
    PlayerController();
    
    virtual ~PlayerController();
    
    static PlayerController* sharePlayerController();

protected:
    
    bool init();
    
    CREATE_FUNC(PlayerController);

};




#endif /* defined(__mir_mobile__PlayerController__) */
