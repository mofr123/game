#ifndef __LMenu__
#define __LMenu__

#include "publicDef/PublicDef.h"

class LMenu;

typedef enum {
    TouchBegin = 0,
    TouchMoved = 1,
    TouchMovedOutSide = 2,
    TouchUpInSide = 3,
    TouchUpSide = 4,
}LMenuType;

typedef enum {
    LMenuStateNormal = 0,
    LMenuStateHighlighted = 1,
    LMenuStateDisabled = 2,
    LMenuStateSelected = 3
}LMenuTouchState;

typedef bool (Ref::*SEL_LMenu)(LMenu*, Point);
#define LMenu_selector(_SELECTOR) (SEL_LMenu)(&_SELECTOR)

class LMenu: public Sprite//, public CCTargetedTouchDelegate
{
    
public:
    
    LMenu(void);
    
    virtual ~LMenu(void);
    
    /**
     *  按钮无背景 设置按钮层区域
     */
    
    static LMenu* create(const Size &size);
    
    /**
     *  按钮背景 任何基于CCRGBAProtocol
     */
    
    static LMenu* createWithRGBAProtocol(Node* backGround);
    
    /**
     *  按钮背景 Sprite
     */
    
    static LMenu* createWithSprite(Sprite* backGround);
    
    /**
     *  按钮背景 CCScale9Sprite
     */
    
    static LMenu* createWithScale9Sprite(Scale9Sprite* backGround);
    
    /**
     *  按钮背景 CCLayerColor
     */
    
    static LMenu* createWithLayerColor(LayerColor* backGround);
    
public:

    /**
     *  按钮背景 Node
     *
     *  @param 类型 任何基于CCNode
     *  @param 例如 CCSprite、CCScale9Sprite、CCLayerColor等
     */
    
    bool initWithBackGround(Node* backGround);//CCRGBAProtocol
    
    /**
     *  按钮背景 Sprite
     */
    
    bool initWithSpriteBackGround(Sprite* backGround);
    
    /**
     *  按钮背景 Scale9Sprite
     */
    
    bool initWithScale9SpriteBackGround(Scale9Sprite* backGround);
    
    /**
     *  按钮背景 CCLayerColor
     */
    
    bool initWithLayerColorBackGround(LayerColor* backGround);
    
    /**
     *  按钮在各种状态背景 Node
     *
     *  @param 类型 任何基于CCNode
     *  @param 例如 CCSprite、CCScale9Sprite、CCLayerColor等
     */
    
    bool setBackGround(LMenuTouchState touchState, Node *var);//CCRGBAProtocol

    /**
     *  按钮在各种状态背景 Sprite
     */
    
    bool setSpriteBackGround(LMenuTouchState touchState, Sprite *var);
    
    /**
     *  按钮在各种状态背景 Scale9Sprite
     */
    
    bool setScale9SpriteBackGround(LMenuTouchState touchState, Scale9Sprite *var);
    
    /**
     *  按钮在各种状态背景 LayerColor
     */
    
    bool setLayerColorBackGround(LMenuTouchState touchState, LayerColor *var);
    
    /**
     *  获取按钮背景
     */
    
    Node* getBackGround(LMenuTouchState touchState);
    
    /**
     *  按钮上层 Sprite
     *
     *  @param 类型 任何基于CCNodeRGBA
     *  @param 例如 CCSprite、CCScale9Sprite
     *  @param 默认坐标 居中
     */
    
    void setSprite(LMenuTouchState touchState, Node* var);//CCNodeRGBA
    
    /**
     *  按钮上层 Sprite
     *
     *  @param 类型 任何基于CCNodeRGBA
     *  @param 例如 CCSprite、CCScale9Sprite
     *  @param 设置坐标，各种状态坐标需分别设置
     */
    
    void setSprite(LMenuTouchState touchState, Node* var, Point point);
    
    /**
     *  获取按钮上层 Sprite
     */
    
    Node* getSprite(LMenuTouchState touchState);
    
public:
    
    /**
     *  按钮标记值，int型
     */
    
    CC_SYNTHESIZE(int, m_sign, Sign);
    
    /**
     *  按钮标记值，字符串型
     */
    
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    /**
     *  按钮点击音效开关与开关状态获取，bool
     */
    
    CC_SYNTHESIZE(bool, m_specially, TouchSpecially);
    
    /**
     *  按钮点击音效开关与开关状态获取，bool
     */
    
    CC_SYNTHESIZE(bool, m_closeTapSound, CloseTapSound);
    
    /**
     *  按钮选中状态开关与开关状态获取，bool
     */
    
    CC_SYNTHESIZE(bool, m_stateSelected, StateSelected);
    
    /**
     *  按钮触摸事件开关与开关状态获取，bool
     */
    
    CC_PROPERTY(bool, m_bIsTouchEnabled, TouchEnabled);
    
    /**
     *  按钮触摸状态开关与开关状态获取，bool
     */
    
    CC_PROPERTY(bool, m_touchClick, TouchClick);
    
    /**
     *  按钮状态开关与开关状态获取，enum
     */
    
    CC_PROPERTY(LMenuTouchState, m_touchState, TouchState);//按钮状态

    /**
     *  按钮大小设置与获取，CCSize
     */
    
    CC_PROPERTY(Size, m_preferredSize, PreferredSize);//设置缩放后大小
    
    /**
     *  获取按钮状态：普通
     */
    
    bool isNormal();
    
    /**
     *  获取按钮状态：高亮
     */
    
    bool isHighlighted();
    
    /**
     *  获取按钮状态：选中
     */
    
    bool isSelected();
    
    /**
     *  获取按钮状态：不可点击
     */
    
    bool isDisabled();

    /**
     *  判断按钮字符串标记是否与其他字符串相同
     */
    
    bool isTextTagEqual(const char* text);
    
    /**
     *  按钮点击映射的方法设置，枚举区分各种触摸状态
     */
    
    void addTarget(void* target, SEL_LMenu selector, LMenuType type);
    
    /**
     *  按钮触摸事件阻断与触摸优先级修改
     */
    
    //void setTouchDispatcher(bool bSwallowsTouches, int nPriority=kCCMenuHandlerPriority);
    
    /**
     *  按钮透明度设置
     */
    
    void setOpacity(GLubyte opacity);
    
    /**
     *  按钮色值设置
     */
    
    void setColor(const Color3B &color3);
    
    /**
     *  按钮色值获取
     */
    
    Color3B& getColor();
    
protected:
    
    virtual void onExit();
    
    virtual void onEnter();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    virtual void setNormalEffect();
    
    virtual void setHighlightedEffect();
    
    virtual void setDisabledEffect();
    
    virtual void setSelectedEffect();
    
    virtual void setNormalColor();
    
    virtual void setHighlightedColor();

    virtual void setDisabledColor();

    virtual void setSelectedColor();
    
protected:
    
    bool m_nbSwallowsTouches;
    
    int m_nTouchPriority;
    
    Color3B m_color;
    
    Node* m_bgNormal;
    
    Node* m_bgHighlighted;
    
    Node* m_bgDisabled;
    
    Node* m_bgSelected;
    
    Node* m_spriteNormal;//CCNodeRGBA
    
    Node* m_spriteHighlighted; //CCNodeRGBA
    
    Node* m_spriteDisabled;//CCNodeRGBA
    
    Node* m_spriteSelected;//CCNodeRGBA
    
    Point m_spriteNPoint;
    
    Point m_spriteHPoint;

    Point m_spriteDPoint;
    
    Point m_spriteSPoint;

private:
    
    void updateWithPreferredSize();
    
    void updateWithPoint();
    
    void setTouchMoved(Point point);
    
    void setTouchMovedOutSide(Point point);
    
    void setTouchUpSide(Point point);
    
    void setTouchUpInSide(Point point);
    
    bool setTouchBegin(Point point);
    
    //void registerWithTouchDispatcher(void);
    
    //void unRegisterWithTouchDispatcher(void);
    
    bool m_IsRehisterTouchDispatcher;
    
    Rect getRect();
    
private:
    
    void* m_target;
    
    SEL_LMenu m_touchBegin;
    
    SEL_LMenu m_touchMoved;
    
    SEL_LMenu m_touchMovedOutSide;
    
    SEL_LMenu m_touchUpInSide;
    
    SEL_LMenu m_touchUpSide;
    
};

#endif /* defined(__LMenu__) */
