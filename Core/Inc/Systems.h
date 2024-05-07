/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Systems.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-09
 * Modify Record:
 */
#ifndef _SYSTEMS_H_
#define _SYSTEMS_H_

#include "ECS.h"
#include "Events.h"
#include <vector>

namespace Systems {


//ecs_ret_t TransformSystem(ecs_t *ecs,
//                       ecs_id_t *entities,
//                       int entity_count,
//                       ecs_dt_t dt,
//                       void *udata);
//
//ecs_ret_t RenderSystem(ecs_t *ecs,
//                       ecs_id_t *entities,
//                       int entity_count,
//                       ecs_dt_t dt,
//                       void *udata);

struct TransformSystem : public ECS::EntitySystem {
  ~TransformSystem() override = default;

  void tick(ECS::World *world, float deltaTime) override;
};

struct RenderSystem : public ECS::EntitySystem {
  ~RenderSystem() override = default;

  void tick(ECS::World *world, float deltaTime) override;
};

struct UIControlSystem : public ECS::EntitySystem,
                         public ECS::EventSubscriber<Events::JoystickUpdateEvent>,
                         public ECS::EventSubscriber<Events::KeypressEvent> {
  ~UIControlSystem() override = default;

  void tick(ECS::World *world, float deltaTime) override;

  virtual void configure(ECS::World *world) override {
    world->subscribe<Events::JoystickUpdateEvent>(this);
    world->subscribe<Events::KeypressEvent>(this);
  }

  virtual void unconfigure(ECS::World *world) override {
    world->unsubscribeAll(this);
    // You may also unsubscribe from specific events with world->unsubscribe<MyEvent>(this), but
    // when unconfigure is called you usually want to unsubscribe from all events.
  }

  virtual void receive(class ECS::World *world, const Events::JoystickUpdateEvent &event) override;

  virtual void receive(class ECS::World *world, const Events::KeypressEvent &event) override;
};

struct GameControlSystem : public ECS::EntitySystem {


 public:

  class LeveInfo {
   public:
    LeveInfo(std::vector<int> _key_tones, std::vector<bool> _key_notes, std::vector<rtos::Kernel::Clock::duration_u32> _key_duration, int _total_keys) :
        key_tones{_key_tones},
        key_notes{_key_notes},
        key_duration{_key_duration},
        total_keys{_total_keys} {};

    LeveInfo(const LeveInfo &info) {
      this->key_tones = info.key_tones;
      this->key_notes = info.key_notes;
      this->key_duration = info.key_duration;
      this->key_speeds = info.key_speeds;
      this->total_keys = info.total_keys;
    }

    LeveInfo() = default;
    std::vector<int> key_tones;
    std::vector<bool> key_notes;
    std::vector<rtos::Kernel::Clock::duration_u32> key_duration;
    std::vector<float> key_speeds;
    int total_keys;
  };

  void initialGameLevel(int level);

  static void m_startPlayMusic(bool* start_to_play);

  void tick(ECS::World *world, float deltaTime) override;

  GameControlSystem();
  ~GameControlSystem() override = default;

  void m_playNote(int freq, rtos::Kernel::Clock::duration_u32 );

  void m_createNote(int index, int pos);

  void m_startLevel();

 private:

  std::vector<LeveInfo> m_levelInfos;

  LeveInfo currentLevelInfo;

  int m_currentMusicNoteIndex = 0;

  bool isGameInitiazing;

  bool start_to_play_notes = false;

};

struct PeripheralCheckSystem_UI : public ECS::EntitySystem {
  ~PeripheralCheckSystem_UI() override = default;

  void tick(ECS::World *world, float deltaTime) override;
};
}

#endif