#ifndef __mir_mobile__FlutteringFairy__
#define __mir_mobile__FlutteringFairy__

#include "publicDef/PublicDef.h"


typedef enum {
	TypeAddExp,
	TypeAddBlood,
	TypeSubBlood,
	TypeSubMagic
}FairyType;


class FlutteringFairy: public Node {

	CC_SYNTHESIZE_READONLY(Node*, m_fairy, Fairy);
    
	void initWithFairy(FlutteringFairy *fairy ,FairyType type, int value, Ref* target, SEL_CallFunc callfunc);
    
public:

    FlutteringFairy();
    
    virtual ~FlutteringFairy();

	static FlutteringFairy* addFairy(Node* parent, Point point, FairyType type, int value, Ref* target, SEL_CallFunc callfunc);
    
	virtual const Size& getContentSize() const;

};


#endif /* defined(__mir_mobile__FlutteringFairy__) */
