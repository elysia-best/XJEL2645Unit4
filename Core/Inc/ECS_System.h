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
#include "Components.h"
#include <cstdint>
#include <cassert>
#include <queue>
#include <bitset>
#include <array>
#include <unordered_map>
#include <set>
#include <memory>

struct MetatypeHash {
  size_t name_hash{0};
  size_t matcher_hash{0};

  bool operator==(const MetatypeHash &other) const {
    return name_hash == other.name_hash;
  }
  template<typename T>
  static constexpr const char *name_detail() {
#ifdef __GNUC__
    return __PRETTY_FUNCTION__;
#else
    return LLVM_PRETTY_FUNCTION;
#endif
  }

#ifndef _WIN32
  template<typename T>
  static constexpr auto name_pretty() {
    //name_detail() is like: static constexpr const char* ecs::MetatypeHash::name_detail() [with T = rstudio::math::Vector3]
    std::string_view pretty_name = name_detail<T>();
    std::string_view prefix = "static constexpr const char* MetatypeHash::name_detail() [with T = ";
    std::string_view suffix = "]";
    pretty_name.remove_prefix(prefix.size());
    pretty_name.remove_suffix(suffix.size());
    return pretty_name;
  }
#else
  template<typename T>
  static constexpr auto name_pretty()
  {
    //name_detail() is like "const char *__cdecl ecs::MetatypeHash::name_detail<class rstudio::math::Vector3>(void)"
    std::string_view pretty_name = name_detail<T>();
    std::string_view prefix = "const char *__cdecl MetatypeHash::name_detail<";
    std::string_view suffix = ">(void)";

    pretty_name.remove_prefix(prefix.size());
    pretty_name.remove_suffix(suffix.size());

    size_t start_of = pretty_name.find_first_of(' ') + 1;
    return pretty_name.substr(start_of);
  }
#endif

  template<typename T>
  static constexpr size_t hash() {

    static_assert(!std::is_reference<T>(), "dont send references to hash");
    static_assert(!std::is_const<T>(), "dont send const to hash");
    return hash_fnv1a(name_detail<T>());
  }
};

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
 public:
  EntityManager() {
    // Initialize the queue with all possible entity IDs
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
      mAvailableEntities.push(entity);
    }
  }

  Entity CreateEntity() {
    assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

    // Take an ID from the front of the queue
    Entity id = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mLivingEntityCount;

    return id;
  }

  void DestroyEntity(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    // Invalidate the destroyed entity's signature
    mSignatures[entity].reset();

    // Put the destroyed ID at the back of the queue
    mAvailableEntities.push(entity);
    --mLivingEntityCount;
  }

  void SetSignature(Entity entity, Signature signature) {
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    // Put this entity's signature into the array
    mSignatures[entity] = signature;
  }

  Signature GetSignature(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    // Get this entity's signature from the array
    return mSignatures[entity];
  }

 private:
  // Queue of unused entity IDs
  std::queue<Entity> mAvailableEntities{};

  // Array of signatures where the index corresponds to the entity ID
  std::array<Signature, MAX_ENTITIES> mSignatures{};

  // Total living entities - used to keep limits on how many exist
  uint32_t mLivingEntityCount{};
};
// Components

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
  void InsertData(Entity entity, T component) {
    assert(
        mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

    // Put new entry at end and update the maps
    size_t newIndex = mSize;
    mEntityToIndexMap[entity] = newIndex;
    mIndexToEntityMap[newIndex] = entity;
    mComponentArray[newIndex] = component;
    ++mSize;
  }

  void RemoveData(Entity entity) {
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

    // Copy element at end into deleted element's place to maintain density
    size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
    size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // Update map to point to moved spot
    Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entity);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;
  }

  T &GetData(Entity entity) {
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

    // Return a reference to the entity's component
    return mComponentArray[mEntityToIndexMap[entity]];
  }

  void EntityDestroyed(Entity entity) override {
    if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
      // Remove the entity's component if it existed
      RemoveData(entity);
    }
  }

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
class ComponentManager {
 public:
  template<typename T>
  void RegisterComponent() {
    const char *typeName = MetatypeHash::name_detail<T>();

    assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

    // Add this component type to the component type map
    mComponentTypes.insert({typeName, mNextComponentType});

    // Create a ComponentArray pointer and add it to the component arrays map
    mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

    // Increment the value so that the next component registered will be different
    ++mNextComponentType;
  }

  template<typename T>
  ComponentType GetComponentType() {
    const char *typeName = MetatypeHash::name_detail<T>();

    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    // Return this component's type - used for creating signatures
    return mComponentTypes[typeName];
  }

  template<typename T>
  void AddComponent(Entity entity, T component) {
    // Add a component to the array for an entity
    GetComponentArray<T>()->InsertData(entity, component);
  }

  template<typename T>
  void RemoveComponent(Entity entity) {
    // Remove a component from the array for an entity
    GetComponentArray<T>()->RemoveData(entity);
  }

  template<typename T>
  T &GetComponent(Entity entity) {
    // Get a reference to a component from the array for an entity
    return GetComponentArray<T>()->GetData(entity);
  }

  void EntityDestroyed(Entity entity) {
    // Notify each component array that an entity has been destroyed
    // If it has a component for that entity, it will remove it
    for (auto const &pair : mComponentArrays) {
      auto const &component = pair.second;

      component->EntityDestroyed(entity);
    }
  }

 private:
  // Map from type string pointer to a component type
  std::unordered_map<const char *, ComponentType> mComponentTypes{};

  // Map from type string pointer to a component array
  std::unordered_map<const char *, std::shared_ptr<IComponentArray>> mComponentArrays{};

  // The component type to be assigned to the next registered component - starting at 0
  ComponentType mNextComponentType{};

  // Convenience function to get the statically casted pointer to the ComponentArray of type T.
  template<typename T>
  std::shared_ptr<ComponentArray<T>> GetComponentArray() {
    const char *typeName = MetatypeHash::name_detail<T>();

    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
  }
};
// Systems
class System {
 public:
  std::set<Entity> mEntities;

  virtual bool Update(float dt) = 0;
};

class SystemManager {
 public:
  template<typename T>
  std::shared_ptr<T> RegisterSystem() {
    const char *typeName = MetatypeHash::name_detail<T>();

    assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();
    mSystems.insert({typeName, system});
    return system;
  }

  template<typename T>
  void SetSignature(Signature signature) {
    const char *typeName = MetatypeHash::name_detail<T>();

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

class Coordinator {
 public:
  void Init() {
    // Create pointers to each manager
    mComponentManager = std::make_unique<ComponentManager>();
    mEntityManager = std::make_unique<EntityManager>();
    mSystemManager = std::make_unique<SystemManager>();
  }

  // Entity methods
  Entity CreateEntity() {
    return mEntityManager->CreateEntity();
  }

  void DestroyEntity(Entity entity) {
    mEntityManager->DestroyEntity(entity);

    mComponentManager->EntityDestroyed(entity);

    mSystemManager->EntityDestroyed(entity);
  }

  // Component methods
  template<typename T>
  void RegisterComponent() {
    mComponentManager->RegisterComponent<T>();
  }

  template<typename T>
  void AddComponent(Entity entity, T component) {
    mComponentManager->AddComponent<T>(entity, component);

    auto signature = mEntityManager->GetSignature(entity);
    signature.set(mComponentManager->GetComponentType<T>(), true);
    mEntityManager->SetSignature(entity, signature);

    mSystemManager->EntitySignatureChanged(entity, signature);
  }

  template<typename T>
  void RemoveComponent(Entity entity) {
    mComponentManager->RemoveComponent<T>(entity);

    auto signature = mEntityManager->GetSignature(entity);
    signature.set(mComponentManager->GetComponentType<T>(), false);
    mEntityManager->SetSignature(entity, signature);

    mSystemManager->EntitySignatureChanged(entity, signature);
  }

  template<typename T>
  T &GetComponent(Entity entity) {
    return mComponentManager->GetComponent<T>(entity);
  }

  template<typename T>
  ComponentType GetComponentType() {
    return mComponentManager->GetComponentType<T>();
  }

  // System methods
  template<typename T>
  std::shared_ptr<T> RegisterSystem() {
    return mSystemManager->RegisterSystem<T>();
  }

  template<typename T>
  void SetSystemSignature(Signature signature) {
    mSystemManager->SetSignature<T>(signature);
  }

 private:
  std::unique_ptr<ComponentManager> mComponentManager;
  std::unique_ptr<EntityManager> mEntityManager;
  std::unique_ptr<SystemManager> mSystemManager;
};

template unsigned char ComponentManager::GetComponentType<Components::Render>();
#endif //XJEL2645NEWVERSION_CORE_SRC_ECS_SYSTEM_H_
