#ifndef __LTabelView__LyfTableView__
#define __LTabelView__LyfTableView__

#include "publicDef/PublicDef.h"

#include "LyfIndicator.h"
#include "LyfIndexPath.h"
#include "LyfTableViewCell.h"


typedef enum {
    TableViewEditNone = 0,
    TableViewEdit
}TableViewEditState;

class LyfTableView;
class LyfTableViewCell;
class LyfIndexPath;

class LyfTableViewDelegate
{
public:
    //cell选中代理回调
	virtual void tableCellTouched(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx){};
    
    //多选时，点击已经选中的cell时取消选中状态代理回调
	virtual void tableCellNoTouched(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx){};
    
    //点击cell的ImageView时代理会掉
	virtual void tableCellImageViewTouchUpInSide(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx){};
    
    //翻页是回调，可用此回调方法更新显示页
	virtual void tableViewUpdate(LyfTableView* table){};
	
	virtual bool removeTableCell(LyfTableView* table, LyfTableViewCell* cell, unsigned int idx){return false;}
};

class LyfTableViewDataSource
{
public:
    
    //init cell callback
    virtual LyfTableViewCell* tableCellAtIndex(LyfTableView *table, unsigned int idx) = 0;
    
    //get cell total num
    virtual unsigned int numberfRowsInTableView(LyfTableView *table) = 0;
    
    void removeDelegate(LyfTableView* table);
    
};

class LyfTableView:
public LayerColor,
public LyfTableViewCellDelegate
{
    LyfTableViewDataSource* m_pDataSource;
    
    LyfTableViewDelegate* m_pTableViewDelegate;
    
    Node* m_backNode;

    LyfIndicator* m_indicator;
    
    float m_nIndicator_offX;
    
    float m_nIndicator_scaleX;
    
    int m_numOfRows;
    
    vector<LyfTableViewCell*> m_TableViewCells;
    
    vector<LyfIndexPath> m_SelectedIndexPaths;

    vector<LyfIndexPath> m_DisabledIndexPaths;
    
    void cellGoHome(float dt);
    
    Point getHomeFromCellPos();
    
    Point getFirmFromCellPos();

    //void visit();
	void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);

    Rect getRect();
    
    //int m_nTouchPriority;
    
   // bool m_IsRehisterTouchDispatcher;
    
    void registerTouch();
    
    void unRegisterTouch();
    
    void setTableViewCells();
    
    void updateTableViewCellPos();
    
    void setIndicator();
    
    void updateIndicator();
    
    void setPullShow();
    
    void updatePullShow();
    
    void pullDownShow();
    
    void pullUpShow();
    
    void PullShowHideFalse();
    
    void update(float delta);
    
    void updatePitchOn();
    
    ActionInterval* cellAction(Point point, float time=0.25f);
    
protected:
    
    virtual void onExit();
    
    virtual void onEnter();

    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    Point m_touchBeginUIPoint;
    
public:
    
    LyfTableView();
    
    virtual ~LyfTableView();
    
    static LyfTableView* create(float contentSizeHeight);
    
    bool initWithTableView(float contentSizeHeight);
    
    void setContentSizeHeight(float contentSizeHeight);
    
    //可设置右侧指示条x轴偏移
	void setIndicatorPointOff(float offX);
    
	//可设置右侧指示条x轴缩放值
	void setIndicatorScaleX(float scaleX);
    
    //数据代理
	void setTableViewDataSource(LyfTableViewDataSource* node);
    
    //可扩展功能代理，如touch事件
	void setTableViewDelegate(LyfTableViewDelegate* node);
    
    //获取当前处于第几页
	int getPage();
    
    //设置要翻到第几页
	void setPage(const int page);
    
    //获取总页数
	int getMaxPage();

    //刷新tableView，传入flase不回到起始位置
	void readData(bool flag = true);
    
public:
    
    //获取cell总数
	int getNumberOfRows();
    
    //
	float getRowHight(unsigned int idx);
    
    //获取tableView高度
	float getTableViewHight();
    
    //获取tableView的父节点
	Node* getNode();
    
    //获取当前选中的cell数
	int getSelectedCountInCurrent();
    
    //根据在tableView中的位置获取tableViewCell
	LyfTableViewCell* getTableViewCellAtIndex(int idx);
    
    //设置cell位置偏移
    void setNodePosY(float y);
    
    //传入被选中Cell的LyfIndexPath数组，让部分cell被选中
    void setSelectedArray(CCArray *array);
    
    //传入被选中Cell的LyfIndexPath数组，让部分cell不可点击
    void setNoClickOtherArray(CCArray* array);
    
    //取消某个cell的选中状态
    void setNoPitchOnOfArray(LyfTableViewCell* cell);
    
    LyfTableViewCell* insertTableCellAtIndex(unsigned int idx);

    LyfTableViewCell* insertFirstTableCell();
    
    LyfTableViewCell* insertLastTableCell();
    
    void removeTableCellAtIndex(unsigned int idx, bool actions=false);
    
    void removeFirstTableCell(bool actions=false);
    
    void removeLastTableCell(bool actions=false);
    
    void replaceTableCellAtIndex(unsigned int idx);
    
    void exchangeTableCellAtIndex(unsigned int idx1, unsigned int idx2);
    
    //设置tableView触摸优先级，可随时设置
    //void setPriority(int nPriority);
    
    void setOpacity(GLubyte opacity);
    
public:
    
    //tableView编辑状态开关，目前支持滑动删除某个cell
    CC_SYNTHESIZE(bool, m_nEdit, Edit);
    
    //tableView滑动回弹开关
    CC_SYNTHESIZE(bool, m_IsCanSkip, CanSkip);
    
    //tableView滑动开关
    CC_SYNTHESIZE(bool, m_IsTouchMoved, TouchMoved);
    
    //设置tableView每页最大cell数，默认为30
    CC_SYNTHESIZE(int, m_maxNumberOfPages, MaxNumberOfPages);
    
    //设置tableView最大可选cell数
    CC_PROPERTY(int, m_selectedCount, SelectedCount);
    
private:

    void setTouchPitchOn(LyfTableViewCell* cell);
    
    void setTouchImageView(LyfTableViewCell* cell);
    
    void removeTableCell(LyfTableViewCell* cell);
    
    void joinEditing(LyfTableViewCell* cell);
    
    bool m_IsRunning;
    
    int m_CellEditing;
    
    bool isEditing();
    
    bool isCanEdit();
    
    void closeEditing();
    
    bool isCanSelected();
    
    std::deque<Point> m_nPointOffset;
    
    float m_nInertiaLength;
    
    void inertia(float dt);
    
    int m_page;
    
    int m_maxPage;
    
    float m_sizeHeight;
    
    void reLoadData();
    
    void readDataInTurnLastPage(float dt);
    
    void readDataInTurnNextpage(float dt);

    void turnLastpage();
    
    void turnNextPage();
    
    bool isTurnLastPage();
    
    bool isTurnNextPage();
    
    Sprite* pullUpShowSprite;
    
    Sprite* pullDownShowSprite;

};



#endif /* defined(__LTabelView__LyfTableView__) */
