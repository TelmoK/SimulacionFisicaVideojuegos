#include "RenderBody.h"

#include "../EntitySystem/ForceGenerators/ForceGenerator.h"

void RenderBody::receiveForceFrom(ForceGenerator* force_generator, float t)
{
	force_generator->applyForce(this, t);
}
