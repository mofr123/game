
#ifndef __mir_mobile__BgMapFloorTile__
#define __mir_mobile__BgMapFloorTile__

#include "publicDef/PublicDef.h"

#define OFF_SIZE Point(128, 128)

class BgMapFloorTile: public Node {
    
protected:
    
    bool m_nIsDisplay;

    Sprite* m_sprite;
    
public:
    
    BgMapFloorTile();
    
    virtual ~BgMapFloorTile();
    
    CREATE_FUNC(BgMapFloorTile);

    CC_SYNTHESIZE_RETAIN(CCString*, m_fileName, FloorTileFileName);
    
    void IntelligentDisplay(Rect& showRect, Rect& hideRect);
    
protected:
    
    void displayImageView();
    
    void hideImageView();
    
    void initWithImageView(Texture2D* texture);
    
};


#endif /* defined(__mir_mobile__BgMapFloorTile__) */
