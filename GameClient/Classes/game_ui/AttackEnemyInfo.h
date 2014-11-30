#ifndef __mir9__AttackEnemyInfo__
#define __mir9__AttackEnemyInfo__


#include "figure/Monomer.h"
#include "publicDef/PublicDef.h"

using namespace ui;

class AttackEnemyInfo: public Layer {
    
    LoadingBar* m_bloodPro;
    
    TextAtlas* m_level;
    
    Text* m_nickName;
    
    Layout* m_widget;

public:
    
    AttackEnemyInfo();
    
    virtual ~AttackEnemyInfo();
    
    CREATE_FUNC(AttackEnemyInfo);
    
    bool init();
    
    void hide();
    
    void showAttackInfo(Monomer* sender);
    
protected:
    
    void updateAttackInfo(float delay);
};

#endif /* defined(__mir9__AttackEnemyInfo__) */
