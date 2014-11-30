#ifndef __mir9__SkillTableView__
#define __mir9__SkillTableView__

#include "publicDef/PublicDef.h"

//NS_CC_EXT_BEGIN


class SkillTableView:
public Layer,//Node
//public CCTargetedTouchDelegate,
public TableViewDataSource,
public TableViewDelegate
{
    

	virtual void scrollViewDidScroll(extension::ScrollView* view);

	virtual void scrollViewDidZoom(extension::ScrollView* view);

	virtual void tableCellTouched(extension::TableView* table, extension::TableViewCell* cell);

	virtual Size tableCellSizeForIndex(extension::TableView *table, ssize_t idx);
	virtual extension::TableViewCell* tableCellAtIndex(extension::TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(extension::TableView *table);

    virtual Size cellSizeForTable(extension::TableView *table);
    
    
protected:
    
    extension::TableView* m_skillTableView;
    
    std::vector<Sprite*> m_skillVec;
    
    Sprite* m_pEditingSkill;
    
    ControlButton* m_pEditingBtn;
    
public:
    
    SkillTableView();
    
    ~SkillTableView();
	
	CREATE_FUNC(SkillTableView);
    
    virtual bool init();
    
    void onEnter();
    
    void onExit();

    
protected:
    
    //void registerWithTouchDispatcher(void);

    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    
	void onTouchEnded(Touch *pTouch, Event *pEvent);
};

#endif /* defined(__mir9__SkillTableView__) */
