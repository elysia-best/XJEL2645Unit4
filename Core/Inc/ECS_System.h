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
  EntityManager();

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

class ComponentManager {
 public:
  template<typename T>
  void RegisterComponent();

  template<typename T>
  ComponentType GetComponentType();

  template<typename T>
  void AddComponent(Entity entity, T component);

  template<typename T>
  void RemoveComponent(Entity entity);

  template<typename T>
  T &GetComponent(Entity entity);

  void EntityDestroyed(Entity entity);

 private:
  std::unordered_map<const char *, ComponentType> mComponentTypes;
  std::unordered_map<const char *, std::shared_ptr<IComponentArray>> mComponentArrays;
  ComponentType mNextComponentType{};

  template<typename T>
  std::shared_ptr<ComponentArray<T>> GetComponentArray();
};

// Systems
class System {
 public:
  std::set<Entity> mEntities;

  virtual void Update(float dt) = 0;
};

class SystemManager {
 public:
  template<typename T>
  std::shared_ptr<T> RegisterSystem();

  template<typename T>
  void SetSignature(Signature signature);

  void EntityDestroyed(Entity entity);

  void EntitySignatureChanged(Entity entity, Signature entitySignature);

 private:
  // Map from system type string pointer to a signature
  std::unordered_map<const char *, Signature> mSignatures{};

  // Map from system type string pointer to a system pointer
  std::unordered_map<const char *, std::shared_ptr<System>> mSystems{};
};

// ECS World
class Coordinator {
 public:
 SINGLETON(Coordinator)

  void Init();

  // Entity methods
  Entity CreateEntity();
  void DestroyEntity(Entity entity);

  // Component methods
  template<typename T>
  void RegisterComponent();

  template<typename T>
  void AddComponent(Entity entity, T component);

  template<typename T>
  void RemoveComponent(Entity entity);

  template<typename T>
  T &GetComponent(Entity entity);

  template<typename T>
  ComponentType GetComponentType();

  // System methods
  template<typename T>
  std::shared_ptr<T> RegisterSystem();

  template<typename T>
  void SetSystemSignature(Signature signature);

 private:
  std::unique_ptr<ComponentManager> mComponentManager;
  std::unique_ptr<EntityManager> mEntityManager;
  std::unique_ptr<SystemManager> mSystemManager;
};

#endif //XJEL2645NEWVERSION_CORE_SRC_ECS_SYSTEM_H_
