#ifndef __mir_mobile__OtherPlayer__
#define __mir_mobile__OtherPlayer__

#include <iostream>
#include "Monomer.h"

class OtherPlayer: public Monomer
{
public:
	OtherPlayer();
	virtual ~OtherPlayer();

	CREATE_FUNC(OtherPlayer);

	bool initWithOtherPlayer(const char* roleFile, const char* weaponFile){return true;}

};


#endif