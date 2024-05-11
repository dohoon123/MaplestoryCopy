#pragma once
#include "Monster.h"
#include "PoolableActor/BossBullet.h"

class DeformedStompy final : public Monster
{
public:
	virtual void Start()	override;
	virtual bool Update()	override;
	virtual void End()		override;
public:
	ObjectPool* bullet_Pooling;
	float pattern_Time = 4;// time to select next pattern
	float bullet_Interval = 0.1f;
	int bullet_Num = 12;
	E_MSTATE prev_Pattern = M_ATTACK;
};