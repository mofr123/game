#ifndef __mir9__ProgressAutomatic__
#define __mir9__ProgressAutomatic__

#include "publicDef/PublicDef.h"

class ProgressAutomatic: public ProgressTimer {
    
    float m_fInterval;
    
protected:
    
    void updateCoolingAction(float dt);
    
    void setCoolingFalse();
    
public:
    
    static ProgressAutomatic* create(Sprite* sp);
    
    void RunCoolingAction(float delay);
    
    void RunCoolingNotAction(float delay);
};

#endif /* defined(__mir9__ProgressAutomatic__) */
