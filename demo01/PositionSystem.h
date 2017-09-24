///-------------------------------------------------------------------------------------------------
/// File:	PositionSystem.h.
///
/// Summary:	Declares the position system class.

#ifndef __POS_SYS_H__
#define __POS_SYS_H__

#include "ECS.h"

#include "Vec3.h"

#include "PositionComponent.h"
#include "RigidbodyComponent.h"

#include "EntityBellowSeaLevelEvent.h"

///-------------------------------------------------------------------------------------------------
/// Class:	PositionSystem
///
/// Summary:	A position system.
///
/// Author:	Tobias Stein
///
/// Date:	9/09/2017

class PositionSystem : public ECS::System<PositionSystem>
{

public:
	
	PositionSystem()
	{
		LogTrace("PositionSystem created!");
	}

	virtual ~PositionSystem()
	{
		LogTrace("PositionSystem destroyed!");
	}

	virtual void Tick(float dt) override
	{
		for (auto it = ECS::ECS_Engine->GetComponentManager()->begin<PositionComponent>(); it != ECS::ECS_Engine->GetComponentManager()->end<PositionComponent>(); ++it)
		{

			//assert(*it != nullptr && "Entity has an invalid PositionComponent.");

			// ignore inactive entities
			if (it->GetOwner()->IsActive() == false)
				continue;

			// Check if entity has RigidBodyComponent
			RigidBodyComponent* rbComp = it->GetOwner()->GetComponent<RigidBodyComponent>();
			if (rbComp != nullptr)
			{
				Position3d pos = it->GetPosition();
				
				Vec3_t vel = rbComp->GetVelocity();

				pos.x += vel.x * dt;
				pos.y += vel.y * dt;
				pos.z += vel.z * dt;

				it->SetPosition(pos);

				// check if entity is bellow sea level
				if (pos.y < 0.0f)
				{
					LogTrace("Entity [%d] is bellow sea level.", it->GetOwner()->GetEntityId());
					ECS::ECS_Engine->SendEvent<EntityBellowSeaLevelEvent>(it->GetOwner()->GetEntityId(), pos.y);
				}
			}
		}
	}
};

#endif // __POS_SYS_H__