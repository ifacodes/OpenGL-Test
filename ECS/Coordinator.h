#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class Coordinator
{
public:

	void Init()
	{
		mEntityManager		= std::make_unique<EntityManager>();
		mComponentManager	= std::make_unique<ComponentManager>();
		mSystemManager		= std::make_unique<SystemManager>();
	}

	// Entity Methods
	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity e)
	{
		mEntityManager->DestroyEntity(e);
		mComponentManager->EntityDestroyed(e);
		mSystemManager->EntityDestroyed(e);
	}

	// Component Methods
	template <typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template <typename T>
	void AddComponent(Entity e, T component)
	{
		mComponentManager->AddComponent<T>(e, component);

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
	}

	template <typename T>
	void RemoveComponent(Entity e)
	{
		mComponentManager->RemoveComponent<T>(e);

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
	}

	template <typename T>
	T& GetComponent(Entity e)
	{
		return mComponentManager->GetComponent<T>(e);
	}

	template <typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	// System Methods
	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template <typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}

private:
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<SystemManager> mSystemManager;
};