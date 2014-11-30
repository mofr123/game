#ifndef __mir9__NpcFigure__
#define __mir9__NpcFigure__

#include "publicDef/PublicDef.h"

#include "NpcInfoSystem.h"


typedef enum
{
    NPCDirDownAndLeft     = 5,
    NPCDirDown            = 4,
    NPCDirRightAndDown    = 3
}NPCDirectionType;

class NpcFigure : public Sprite//, public CCTargetedTouchDelegate
{
    
    //CCArray* m_pArray;
	Vector<SpriteFrame*> m_pArray;

    NPCDirectionType m_eTpye;
    
    bool m_nIsDisplay;
    
    Sprite* m_sprite;
    
    Sprite* m_spriteHigh;
    
    Point m_fAnchorPoint;
    
    Rect m_fSpriteRect;
    
    CC_SYNTHESIZE_READONLY(NpcInfo*, m_npcInfo, NpcInfo);
    
public:
    
    NpcFigure(unsigned int roleNumber, NPCDirectionType type);
    
    virtual ~NpcFigure();
    
    void onEnter();
    
    void onExit();
    
    static NpcFigure* create(unsigned int roleNumber, NPCDirectionType type);
    
    CC_SYNTHESIZE_RETAIN(CCString*, m_fileName, NpcFileName);
    
    CC_SYNTHESIZE_RETAIN(CCString*, m_plistName, NpcPlistName);
    
    void IntelligentDisplay(Rect& showRect, Rect& hideRect);
    
protected:
    
    void displayImageView();
    
    void hideImageView();
    
    void initWithImageView(Texture2D* texture);
    
    Action* getActions();
    
    void setHighlight();
    
    void setNormal();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    void alertCallBack(const int number);
};

#endif /* defined(__mir9__NpcFigure__) */
