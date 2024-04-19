/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: ECS_System.cpp
 * Author: Yuanzhen Gan
 * Date: 24-4-19
 * Modified form Sources:
 *  - https://austinmorlan.com/posts/entity_component_system
 * Modify Record:
 */

#include "ECS_System.h"

EntityManager::EntityManager() {
  // Initialize the queue with all possible entity IDs
  for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
    mAvailableEntities.push(entity);
  }
}

Entity EntityManager::CreateEntity() {
  assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

  // Take an ID from the front of the queue
  auto id = mAvailableEntities.front();
  mAvailableEntities.pop();
  ++mLivingEntityCount;

  return id;
}

void EntityManager::DestroyEntity(Entity entity) {
  assert(entity < MAX_ENTITIES && "Entity out of range.");

  // Invalidate the destroyed entity's signature
  mSignatures[entity].reset();

  // Put the destroyed ID at the back of the queue
  mAvailableEntities.push(entity);
  --mLivingEntityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature) {
  assert(entity < MAX_ENTITIES && "Entity out of range.");

  // Put this entity's signature into the array
  mSignatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) {
  assert(entity < MAX_ENTITIES && "Entity out of range.");

  // Get this entity's signature from the array
  return mSignatures[entity];
}

template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T component) {
  assert(
      mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

  // Put new entry at end and update the maps
  size_t newIndex = mSize;
  mEntityToIndexMap[entity] = newIndex;
  mIndexToEntityMap[newIndex] = entity;
  mComponentArray[newIndex] = component;
  ++mSize;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity) {
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

template<typename T>
T &ComponentArray<T>::GetData(Entity entity) {
  assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

  // Return a reference to the entity's component
  return mComponentArray[mEntityToIndexMap[entity]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity) {
  if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
    // Remove the entity's component if it existed
    RemoveData(entity);
  }
}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component) {
  GetComponentArray<T>()->InsertData(entity, component);
}

template<typename T>
void ComponentManager::RemoveComponent(Entity entity) {
  GetComponentArray<T>()->RemoveData(entity);
}

template<typename T>
T &ComponentManager::GetComponent(Entity entity) {
  return GetComponentArray<T>()->GetData(entity);
}

template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray() {
  const char *typeName = MetatypeHash::name_detail<T>();
  assert(mComponentArrays.find(typeName) != mComponentArrays.end() && "Component not registered before use.");
  return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
}

template<typename T>
std::shared_ptr<T> SystemManager::RegisterSystem() {
  const char *typeName = MetatypeHash::name_detail<T>();

  assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

  // Create a pointer to the system and return it so it can be used externally
  auto system = std::make_shared<T>();
  mSystems.insert({typeName, system});
  return system;
}

template<typename T>
void SystemManager::SetSignature(Signature signature) {
  const char *typeName = MetatypeHash::name_detail<T>();

  assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

  // Set the signature for this system
  mSignatures.insert({typeName, signature});
}

void SystemManager::EntityDestroyed(Entity entity) {
  // Erase a destroyed entity from all system lists
  // mEntities is a set so no check needed
  for (auto const &pair : mSystems) {
    auto const &system = pair.second;

    system->mEntities.erase(entity);
  }
}
void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature) {
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

template<typename T>
void Coordinator::RegisterComponent() {
  mComponentManager->RegisterComponent<T>();
}

template<typename T>
void Coordinator::AddComponent(Entity entity, T component) {
  mComponentManager->AddComponent<T>(entity, component);

  auto signature = mEntityManager->GetSignature(entity);
  signature.set(mComponentManager->GetComponentType<T>(), true);
  mEntityManager->SetSignature(entity, signature);

  mSystemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
void Coordinator::RemoveComponent(Entity entity) {
  mComponentManager->RemoveComponent<T>(entity);

  auto signature = mEntityManager->GetSignature(entity);
  signature.set(mComponentManager->GetComponentType<T>(), false);
  mEntityManager->SetSignature(entity, signature);

  mSystemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
T &Coordinator::GetComponent(Entity entity) {
  return mComponentManager->GetComponent<T>(entity);
}

template<typename T>
ComponentType Coordinator::GetComponentType() {
  return mComponentManager->GetComponentType<T>();
}

template<typename T>
std::shared_ptr<T> Coordinator::RegisterSystem() {
  return mSystemManager->RegisterSystem<T>();
}

template<typename T>
void Coordinator::SetSystemSignature(Signature signature) {
  mSystemManager->SetSignature<T>(signature);
}
