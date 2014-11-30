#ifndef __mir9__GameOptions__
#define __mir9__GameOptions__

#include "publicDef/PublicDef.h"

class GameOptions: public Node
{
    
    Sprite* m_pBg;
    
public:
    
    CREATE_FUNC(GameOptions);
    
    void onEnterTransitionDidFinish();
};

#endif /* defined(__mir9__GameOptions__) */
