/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: ECS_System.h
 * Author: Yuanzhen Gan
 * Date: 24-4-19
 * Modified form Sources:
 *  - https://austinmorlan.com/posts/entity_component_system
 * Modify Record:
 */

#ifndef XJEL2645NEWVERSION_CORE_SRC_ECS_SYSTEM_H_
#define XJEL2645NEWVERSION_CORE_SRC_ECS_SYSTEM_H_

#include "singleton.h"
#include <cstdint>
#include <cassert>
#include <queue>
#include <bitset>
#include <array>
#include <unordered_map>
#include <set>

// Defines
// A simple type alias
using Entity = std::uint32_t;

// Used to define the size of arrays later on
#define MAX_ENTITIES (Entity)256

// A simple type alias
using ComponentType = std::uint8_t;

// Used to define the size of arrays later on
#define MAX_COMPONENTS (ComponentType)32

// A simple type alias
using Signature = std::bitset<MAX_COMPONENTS>;

// EntityManager Class Definitions
class EntityManager {
 private:
  EntityManager();

 public:
 SINGLETON(EntityManager)

  Entity CreateEntity();

  void DestroyEntity(Entity entity);

  void SetSignature(Entity entity, Signature signature);

  Signature GetSignature(Entity entity);

 private:
  // Queue of unused entity IDs
  std::queue<Entity> mAvailableEntities{};

  // Array of signatures where the index corresponds to the entity ID
  std::array<Signature, MAX_ENTITIES> mSignatures{};

  // Total living entities - used to keep limits on how many exist
  uint32_t mLivingEntityCount{};
};

// The one instance of virtual inheritance in the entire implementation.
// An interface is needed so that the ComponentManager (seen later)
// can tell a generic ComponentArray that an entity has been destroyed
// and that it needs to update its array mappings.
class IComponentArray {
 public:
  virtual ~IComponentArray() = default;
  virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
 public:
  void InsertData(Entity entity, T component);

  void RemoveData(Entity entity);

  T &GetData(Entity entity);

  void EntityDestroyed(Entity entity) override;

 private:
  // The packed array of components (of generic type T),
  // set to a specified maximum amount, matching the maximum number
  // of entities allowed to exist simultaneously, so that each entity
  // has a unique spot.
  std::array<T, MAX_ENTITIES> mComponentArray;

  // Map from an entity ID to an array index.
  std::unordered_map<Entity, size_t> mEntityToIndexMap;

  // Map from an array index to an entity ID.
  std::unordered_map<size_t, Entity> mIndexToEntityMap;

  // Total size of valid entries in the array.
  size_t mSize;
};

// Systems
class System {
 public:
  std::set<Entity> mEntities;
};

class SystemManager {
 public:
  template<typename T>
  std::shared_ptr<T> RegisterSystem() {
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();
    mSystems.insert({typeName, system});
    return system;
  }

  template<typename T>
  void SetSignature(Signature signature) {
    const char *typeName = typeid(T).name();

    assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

    // Set the signature for this system
    mSignatures.insert({typeName, signature});
  }

  void EntityDestroyed(Entity entity) {
    // Erase a destroyed entity from all system lists
    // mEntities is a set so no check needed
    for (auto const &pair : mSystems) {
      auto const &system = pair.second;

      system->mEntities.erase(entity);
    }
  }

  void EntitySignatureChanged(Entity entity, Signature entitySignature) {
    // Notify each system that an entity's signature changed
    for (auto const &pair : mSystems) {
      auto const &type = pair.first;
      auto const &system = pair.second;
      auto const &systemSignature = mSignatures[type];

      // Entity signature matches system signature - insert into set
      if ((entitySignature & systemSignature) == systemSignature) {
        system->mEntities.insert(entity);
      }
        // Entity signature does not match system signature - erase from set
      else {
        system->mEntities.erase(entity);
      }
    }
  }

 private:
  // Map from system type string pointer to a signature
  std::unordered_map<const char *, Signature> mSignatures{};

  // Map from system type string pointer to a system pointer
  std::unordered_map<const char *, std::shared_ptr<System>> mSystems{};
};

#endif //XJEL2645NEWVERSION_CORE_SRC_ECS_SYSTEM_H_
