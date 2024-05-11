#pragma once

class Scene abstract
{
public:
	virtual void Start()		abstract;
	virtual bool Update()		abstract;
	virtual void End()			abstract;
};