#pragma once

#include "pch.h"



//Interface for components
class Component 
{
public:
	virtual std::string GetType() = 0;
	virtual void DisableComponent() = 0;
	virtual void EnableComponent() = 0;
	virtual bool Enabled() = 0;
protected:
	bool enabled = false;
};
