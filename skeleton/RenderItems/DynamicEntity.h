#pragma once
class ForceGenerator;

class DynamicEntity
{
public:
	DynamicEntity() {}
	virtual ~DynamicEntity() {}

	virtual void update(float t) {}

	/* 
		Técnica del doble despacho:
		Para facilitar el polimorfismo en EntitySistem es cada objeto de tipo
		DynamicEntity, que conoce su clase heredada de DynamicEntity, llama a la 
		función sobre cargada applyForce() de ForceGenerator.
	*/
	virtual void receiveForceFrom(ForceGenerator* force_generator, float t) = 0;
};