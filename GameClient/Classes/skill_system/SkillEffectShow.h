#ifndef __mir_mobile__SkillEffectShow__
#define __mir_mobile__SkillEffectShow__

#include <iostream>
#include "AttackSkillSystem.h"
#include "figure/Monomer.h"

class SkillEffectShow : public Node
{
	AttackSkillInfo m_skillInfo;

	Node* m_attacker;

	Node* m_victim;

	Sprite* m_showSprite;

	Sprite* m_explosionSprite;

	Scale9Sprite* m_tailSprite;

	float m_hitDelay;

public:
	SkillEffectShow(AttackSkillInfo& skillInfo, float hitDelay);
	virtual ~SkillEffectShow();

	static SkillEffectShow* playSkillEffect(AttackSkillInfo& skillInfo, Monomer* attacker, Monomer* victim, float hitDelay);

	void onEnterTransitionDidFinish();

	void onExitTransitionDidStart();


protected:

	bool init(Monomer* attacker, Monomer* victim);

	void emission(float delay = 0);

	void track(float delay = 0);

	void hit();

	void playTailing();

	void updateTailing(float delay = 0);

	void initWithShowSprite();

	void playCasterSpecific(const char *path);

	void playLocusSpecific(const char *path);

	void playExplosionSpecific(const char *path);

	void sendMessage();

	void releaseThis();

protected:

	int getRotationWithLocusSpecific();

	inline const Point getDestination();

	inline const Point getDeparture();

};


#endif