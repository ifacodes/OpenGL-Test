#pragma once
#include "ESC_Definitions.h"
#include <unordered_map>
#include <cassert>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity e) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity e, T component)
	{	
		assert(mEntitytoIndexMap.find(e) == mEntitytoIndexMap.end() && "Component added to the same Entity more than once!");
		
		size_t newIndex = mSize;
		mEntitytoIndexMap[e] = newIndex;
		mIndextoEntityMap[newIndex] = e;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	void RemoveData(Entity e)
	{
		assert(mEntityToIndexMap.find(e) != mEntitytoIndexMap.end() && "Removing non-existant Component!");

		// move element at end to the removed elements position
		size_t indexOfRemovedEntity = mEntitytoIndexMap[e];
		size_t indexOfLastElement = size_t - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// update to point to new spot
		Entity entityOfLastElement = mIndextoEntityMap[indexOfLastElement];
		mEntitytoIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndextoEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntitytoIndexMap.erase(e);
		mIndextoEntityMap.erase(indexOfLastElement);

		--mSize;
	}
	T& GetData(Entity e)
	{
		assert(mEntitytoIndexMap[e] != mEntitytoIndexMap.end() && "Retrieving non-existant Component!");

		return mComponentArray[mEntitytoIndexMap[e]];
	}

	void EntityDestroyed(Entity e) override
	{
		if (mEntitytoIndexMap.find(e) != mEntitytoIndexMap.end())
		{
			RemoveData(e);
		}
	}

private:

	//each instance of Component Array is based on a Component Type (Array per Type)
	std::array<T, MAX_ENTITIES> mComponentArray{};

	std::unordered_map<Entity, size_t> mEntitytoIndexMap;	// ID to Array Index
	std::unordered_map<size_t, Entity> mIndextoEntityMap;	// Array Index to Entity

	// total size of current entries
	size_t mSize;
};