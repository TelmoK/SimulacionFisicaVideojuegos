#pragma once
class DynamicEntity
{
public:
	DynamicEntity() {}
	virtual ~DynamicEntity() {}

	virtual void update(float t) {}
};