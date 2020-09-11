#pragma once

#include <memory>
#include <cassert>
#include <unordered_map>

#include "System.h"

class SystemManager
{
public:

	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end() && "System already Registered!");

		auto system = std::make_shared<T>();
		mSystems.insert({ typename, system });
		return system;
	}

	template <typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end() && "System not Registered!");

		mSignatures.insert({ typeName, signature });

	}

	void EntityDestroyed(Entity e)
	{
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(e);
		}
	}

	void EntitySignatureChanged(Entity e, Signature eSignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((eSignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(e);
			}
			else
			{
				system->mEntities.erase(e);
			}
		}
	}

private:

	std::unordered_map<const char*, Signature> mSignatures{};
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};

};