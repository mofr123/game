#ifndef __mir_mobile__GameSocket__
#define __mir_mobile__GameSocket__

#include <iostream>
#include "figure/Monomer.h"

class GameSocket {
    
public:
    
    static void attack(Monomer* one, Monomer* two, int skillNumber);
    
    static void attackGroup(Monomer* one, vector<Monomer*> two, int skillNumber);
    
    static void sendRoleCreate(Node* node, int roleID, const char* nickName);
};

#endif /* defined(__mir_mobile__GameSocket__) */
