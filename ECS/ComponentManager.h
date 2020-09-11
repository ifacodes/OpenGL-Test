#pragma once

#include <cassert>
#include <unordered_map>
#include <memory>

#include "ComponentArray.h"

class ComponentManager
{
public:

	template <typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typename) == mComponentTypes.end() && "Component already Registered!");

		mComponentTypes.insert({ typeName, mNextComponentType });
		mComponentArrays.insert({ typeName, std::make_shared < ComponentArray<T>() });

		++mNextComponentType;
		
	}

	template <typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typename) != mComponentTypes.end() && "Component not Registered!");

		return mComponentTypes[typeName];
	}

	template <typename T>
	void AddComponent(Entity e, T component)
	{
		GetComponentArray<T>()->InsertData(e, component);
	}

	template <typename T>
	void RemoveComponent(Entity e)
	{
		GetComponentArray<T>()->RemoveData(e);
	}

	template <typename T>
	T& GetComponent(Entity e)
	{
		return GetComponentArray<T>()->GetData;
	}

	void EntityDestroyed(Entity e)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(e);
		}
	}

private:

	std::unordered_map<const char*, ComponentType> mComponentTypes{}; //string pointer typename to actual component type
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{}; //string pointer to component array

	ComponentType mNextComponentType{}; //increases which each component type;

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component Not Registered");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}

};