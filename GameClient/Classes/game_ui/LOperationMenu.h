#ifndef __mir_mobile__LOperationMenu__
#define __mir_mobile__LOperationMenu__

#include <iostream>
#include "ProgressAutomatic.h"

#define TAG_PROGRESS_SKILL 0xfffffff

class LOperationMenu: public Layer {
    
    bool m_isHangUpAttack;

    int m_attackMethods;

    Node* m_direction;
    
    Node* m_operation;
    
    ControlButton* m_commonAttackBtn;
    
    ControlButton* m_skillAttackBtn[3];
    
    CC_SYNTHESIZE_READONLY(ControlButton*, m_drugsBtn, DrugsBtn);
    
    CC_SYNTHESIZE_READONLY(ControlButton*, m_deliveryBtn, DeliveryBtn);
    
    CC_SYNTHESIZE_READONLY(ProgressAutomatic*, m_currProgress, CurrProgress);
    
    CC_SYNTHESIZE_READONLY(int, m_moveMethods, MoveMethods);
    
public:
    
    LOperationMenu();
    
    virtual ~LOperationMenu();
    
    CREATE_FUNC(LOperationMenu);
    
    bool init();
    
    void attack();
    
    void hangUpAttack(Ref* sender, Control::EventType controlEvents);
    
    void moveMethods(Ref* sender, Control::EventType controlEvents);
    
    void intelligentSearch();
    
    void cancelHangUP();
    
    void hideOperationMenu();
    
    void showOperationMenu();
    
    void joinEditState();
    
    void touchCommonAttack(Ref* sender, Control::EventType controlEvents);
	void commonAttack();
    
    void skillAttack(Ref* sender, Control::EventType controlEvents);
    
    void useDrugs(Ref* sender, Control::EventType controlEvents);
    
    void usedeliveryProp(Ref* sender, Control::EventType controlEvents);
    
    ControlButton* getSkillAttackBtn(unsigned int number);
    
    void addSkillIcon(ControlButton* btn, int skillNumber);
    
    void addSkillIcon(int btnNumber, int skillNumber);
    
    void addDrugs(int drugsNumber);
    
    void skillAttackCooling(ProgressTimer* progress);
    
};

#endif /* defined(__mir_mobile__LOperationMenu__) */
