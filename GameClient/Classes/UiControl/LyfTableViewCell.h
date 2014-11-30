#ifndef __LTabelView__LyfTableViewCell__
#define __LTabelView__LyfTableViewCell__

#include "publicDef/PublicDef.h"

#include "LyfIndexPath.h"

#define dvalue(x) x*2 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )//|| CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#define TOUCH_PROTECTION_ACCURACY 0
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define TOUCH_PROTECTION_ACCURACY dvalue(2)
#endif

class LyfTableViewCell;
typedef enum {
    tNone = 0,
    tCell = 1,
    tLead = 2,
    tDel = 3
}tTouchModel;

typedef enum {
    tTop = 0,
    tCenter = 1,
    tBottom = 2
    
}tVerticalText;

class LyfTableViewCellDelegate {
    
public:
    virtual void setTouchPitchOn(LyfTableViewCell* cell) = 0;
    
    virtual void setTouchImageView(LyfTableViewCell* cell) = 0;
    
    virtual void removeTableCell(LyfTableViewCell* cell) = 0;
    
    virtual void joinEditing(LyfTableViewCell* cell) = 0;
    
    virtual bool isEditing() = 0;
    
    virtual bool isCanEdit() = 0;
    
    virtual void closeEditing() = 0;
    
    virtual bool isCanSelected() = 0;
    
    void removeDelegate(LyfTableViewCell* cell);
};


class LyfTableViewCell: public Sprite//, public CCTargetedTouchDelegate
{
    
public:
      
    LyfTableViewCell();
    
    virtual ~LyfTableViewCell();
    
    static LyfTableViewCell* create(const char *fileName);
    
    static LyfTableViewCell* createWithSpriteFrameName(const char *fileName);
    
    static LyfTableViewCell* createTmp();
    
    static LyfTableViewCell* createWithTexture(Texture2D *pTexture);
    
    void setDelegate(LyfTableViewCellDelegate * tableView);
    
public:
    
    //click effect
    virtual void setTouchedEffect(){};
    
    virtual void setNoTouchedEffect(){};
    
    //select effect
    virtual void setPitchOnEffect(){};
    
    virtual void setNoPitchOnEffect(){};
    
    //click effect
    virtual void setClickEffect(){};

    //not click effect
    virtual void setNoClickEffect(){};
    
    //touch priority
    //virtual void setPriority(int nPriority);
    
public:
    
    void setImageView(Sprite* sprite);
    
    Sprite* getImageView();
    
    void setTextLabel(Label* textLabel);
    
    Label* getTextLabel();
    
    void setDetailTextLabel(Label* detailTextLabel);
    
    Label* getDetailTextLabel();
    
    void setTouchModel(tTouchModel tEnum);
    
    tTouchModel m_touchModel;
    
    void showDelBtn();
    
    void removeDelBtnParent();
    
public:
    
    //click effect switch
    CC_SYNTHESIZE(bool, m_specially, TouchSpecially);
    
    //string flag
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    //attr
    CC_SYNTHESIZE(LyfIndexPath, m_sign, Sign);
    
    //touch switch
    CC_PROPERTY(bool, m_bIsTouchEnabled, TouchEnabled);
    
    //is or not helight
    CC_SYNTHESIZE_READONLY(bool, m_touchBegin, TouchBegin);
    
    //is or select
    CC_PROPERTY(bool, m_pitchOn, PitchOn);
    
    bool isTextTagEqual(const char* text);
    
    //ref cell of point,point is or not in tableView scope
    bool isInTheRangeOfTableView(Point point);
    
    void setOpacity(GLubyte opacity);
    
    void removeThisAndActions();
    
    void removeThis(float dt);
    
protected:
    
    virtual void onExit();
    
    virtual void onEnter();
    
    Sprite* m_imageView;
    
    Label* m_textLabel;
    
    Label* m_detailTextLabel;
    
    LyfTableViewCellDelegate* m_delegate;
    
    int m_nTouchPriority;
    
    bool m_IsRehisterTouchDispatcher;
    
    Sprite* m_delBtn;
    
    Rect m_delRect;
    
    void delShowAction(float dt);
    
    void delHideAction(float dt);
    
protected:
    
    bool isOverstep(Point point);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    //void registerWithTouchDispatcher();
    
    //void unRegisterWithTouchDispatcher();
    
    inline Rect getRect();
    
    inline Rect getBtnRect(Sprite* sprite);
    
    inline Rect getDelRect();
    
    Point m_touchBeginUIPoint;
    
};

#endif /* defined(__LTabelView__LyfTableViewCell__) */
