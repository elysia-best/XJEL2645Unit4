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

void EntityManager::DestroyEntity(Entity entity)  {
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
void ComponentArray<T>::RemoveData(Entity entity){
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
void ComponentArray<T>::EntityDestroyed(Entity entity)  {
  if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
    // Remove the entity's component if it existed
    RemoveData(entity);
  }
}