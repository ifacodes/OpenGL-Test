#pragma once

#include <queue>
#include <cassert>
#include <array>
#include "ESC_Definitions.h"


class EntityManager
{
public:
	EntityManager()
	{
		for (Entity e = 0; e < MAX_ENTITIES; ++e)
		{
			mAvailableEntities.push(e);
		}
	}
	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many Entities Exist.");

		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}

	void DestroyEntity(Entity e)
	{
		assert(e < MAX_ENTITIES && "Entity out of range.");

		mSignatures[e].reset();
		mAvailableEntities.push(e);
		--mLivingEntityCount;
	}
	
	void SetSignature(Entity e, Signature s)
	{
		assert(e < MAX_ENTITIES && "Entity out of range.");

		mSignatures[e] = s;
	}

	Signature GetSignature(Entity e)
	{
		assert(e < MAX_ENTITIES && "Entity out of range.");

		return mSignatures[e];
	}

private:
	std::queue<Entity> mAvailableEntities{};
	std::array<Signature, MAX_ENTITIES> mSignatures{};
	uint32_t mLivingEntityCount{};
};