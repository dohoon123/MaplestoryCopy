#pragma once
#include <string>
class Creature abstract
{
public:
	virtual void Start()	abstract;
	virtual bool Update()	abstract;
	virtual void End()		abstract;

public:
	bool is_Alive = true;
	bool is_Flipped = false;
	bool is_Falling = true;
};

