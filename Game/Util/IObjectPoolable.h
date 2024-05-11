#pragma once

class IObjectPoolable abstract
{
public:
	bool CanRecyclable = false;

public:
	virtual ~IObjectPoolable() = default;

	virtual void SetRecycle()
	{
		CanRecyclable = true;
	}

	virtual void OnRecycle()
	{
		CanRecyclable = false;
	}

	virtual void Update()
	{
		if (CanRecyclable) return;
	}

};