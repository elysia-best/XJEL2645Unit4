/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Systems.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-04-09
 * Modify Record:
 */
#include "Systems.h"
#include "Components.h"
#include "GameEngine.h"
#include "log.h"
#include "GlobalDefines.h"
#include "ECS.h"
#include "tuple"
#include "Spirits.h"
#include "arm_math.h"
#include <random>

void Systems::RenderSystem::tick(ECS::World *world, float deltaTime) {
  world->each<Components::Transform, Components::Render>(
      [&](ECS::Entity *ent,
          ECS::ComponentHandle<Components::Transform> trans, ECS::ComponentHandle<Components::Render> render) -> void {
        using render_t = Components::Render::Type_e;
        if (render->Visible) {
          switch (render->Type) {
            case render_t::Spirit :
              Engine::GameManager::getInstance()->lcd->drawSprite(std::get<0>(trans->Position),
                                                                  std::get<1>(trans->Position),
                                                                  render->y,
                                                                  render->x,
                                                                  render->Data.spirit_Data,
                                                                  render->Override);
              break;
            case render_t::Text :
              if (render->size == 1)
                Engine::GameManager::getInstance()->lcd->printString(render->Data.text_Data.c_str(),
                                                                     std::get<0>(trans->Position),
                                                                     std::get<1>(trans->Position));
              else
                Engine::GameManager::getInstance()->lcd->printString5x5(render->Data.text_Data.c_str(),
                                                                        std::get<0>(trans->Position),
                                                                        std::get<1>(trans->Position));
          }

        }
      }
  );
}

void Systems::UIControlSystem::tick(ECS::World *world, float deltaTime) {
  world->each<Components::UIRender>(
      [&](ECS::Entity *ent, ECS::ComponentHandle<Components::UIRender> render) -> void {
        for (auto &p : render->m_comps) {
          if (p.id == render->selected && p.Visible) {
            Engine::GameManager::getInstance()->lcd->drawSprite(std::get<0>(p.trans.Position),
                                                                std::get<1>(p.trans.Position),
                                                                p.y,
                                                                p.x,
                                                                p.spirit_Data, false);
          }
        }

      }

  );
}
void Systems::UIControlSystem::receive(ECS::World *world, const Events::JoystickUpdateEvent &event) {
  int8_t control_result = 0;
  if (event.direction == Direction::E || event.direction == Direction::S) {
    control_result = 1;
  } else if (event.direction == Direction::W || event.direction == Direction::N) {
    control_result = -1;
  }

  if (control_result == 0)
    return;

  world->each<Components::UIRender>(
      [=](ECS::Entity *ent, ECS::ComponentHandle<Components::UIRender> render) -> void {
        if (control_result == -1) {
          render->selected = ((render->selected + 1) < (render->m_comps.size())) ? (render->selected + 1) : 0;
        } else if (control_result == 1) {
          render->selected = ((render->selected - 1) >= 0) ? (render->selected - 1) : (render->m_comps.size() - 1);
        }
      }
  );
}

void Systems::UIControlSystem::receive(ECS::World *world, const Events::KeypressEvent &event) {
  std::list<ECS::ComponentHandle<Components::UIRender>> renders;
  world->each<Components::UIRender>(
      [&](ECS::Entity *ent, ECS::ComponentHandle<Components::UIRender> render) -> void {
        renders.emplace_back(render);
      }
  );
  switch (event.id) {
    case 0:
      for (auto &render : renders) {
        auto p = render->m_comps.begin();
        std::advance(p, render->selected);
        p->callback_functionA();
      }
      break;
    case 1:
      for (auto &render : renders) {
        auto p = render->m_comps.begin();
        std::advance(p, render->selected);
        p->callback_functionB();
      }
      break;
    case 2:
      for (auto &render : renders) {
        auto p = render->m_comps.begin();
        std::advance(p, render->selected);
        p->callback_functionC();
      }
      break;
    case 3:
      for (auto &render : renders) {
        auto p = render->m_comps.begin();
        std::advance(p, render->selected);
        p->callback_functionD();
      }
      break;

  }
}

void Systems::GameControlSystem::tick(ECS::World *world, float deltaTime) {

  //log_info("Current FPS: %f", 1/(deltaTime/1000));

  if(isGameInitiazing) {
    // First run of the level, generate a note first.
    log_info("Init Game Level");
    isGameInitiazing = false;
    m_startLevel();
  }

  // Update states of notes
  world->each<Components::Transform, Components::Note>([=](ECS::Entity *ent,
                                                           ECS::ComponentHandle<Components::Transform> trans,
                                                           ECS::ComponentHandle<Components::Note> note) -> void {
    // Destroy note if out ranged.
    if (get<1>(trans->Position) > 48)
      world->destroy(ent);
    else if ((get<1>(trans->Position) > 38)) {
      start_to_play_notes = true;
    }

    get<1>(trans->Position) += deltaTime * note->Speed;
  });

  m_scores->get<Components::Render>()->Data.text_Data = to_string(m_currentScore);
}

void Systems::GameControlSystem::initialGameLevel(int level) {
  currentLevelInfo = LeveInfo(m_levelInfos[level]);
  isGameInitiazing = true;
}

Systems::GameControlSystem::GameControlSystem() {
  //  ToDo: Still have error when first key needs
  m_levelInfos.emplace_back(LeveInfo({
    std::vector<int>(
        {NOTE_DS4, NOTE_AS4, NOTE_FS5, NOTE_AS4,
            NOTE_DS4, NOTE_AS4,NOTE_AS5, NOTE_AS4,
            NOTE_B3, NOTE_FS4, NOTE_DS6, NOTE_FS4,
            NOTE_B3, NOTE_FS4, NOTE_CS6, NOTE_FS4,
            NOTE_FS4, NOTE_CS5, NOTE_AS5, NOTE_CS5,
            NOTE_FS4, NOTE_CS5, NOTE_AS5, NOTE_CS5,
            NOTE_CS4, NOTE_GS4, NOTE_F5, NOTE_GS4,
            NOTE_CS4, NOTE_GS4, NOTE_F5, NOTE_GS4,
            }),
    {0, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,
                1, 1, 1, 1,},
 std::vector<rtos::Kernel::Clock::duration_u32>(
     {330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms,
         330ms, 330ms, 330ms, 330ms}),
    32
  }));
}

void Systems::GameControlSystem::m_playNote(int freq, rtos::Kernel::Clock::duration_u32 duration) {
  Engine::GameManager::getInstance()->buzzer->period_us((float) 1000000.0f/ (float) freq); //set the period of the pwm signal (in us)
  Engine::GameManager::getInstance()->buzzer->pulsewidth_us(Engine::GameManager::getInstance()->buzzer->read_period_us()/2);  //set pulse width of the pwm to 1/2 the period
  //log_info("Playing tone: %d", freq);
  ThisThread::sleep_for(duration);  //play sound for duration ms
}

void Systems::GameControlSystem::m_createNote(int index, int pos) {
  auto ent = Engine::GameManager::getInstance()->ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();
  auto note = ent->assign<Components::Note>();

  // log_info("Creating note: %d at: %d", index, pos);

  switch (pos) {
    case 0:
      trans->Position = {28, 0, 0};
      break;
    case 1:
      trans->Position = {35, 0, 0};
      break;
    case 2:
      trans->Position = {42, 0, 0};
      break;
    case 3:
      trans->Position = {49, 0, 0};
      break;
  }

  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_keyNote;
  render->Visible = true;
  render->x = 6;
  render->y = 5;

  note->Type = 0;
  note->Score = 10;
  note->Speed = currentLevelInfo.key_speeds[index];
}

void Systems::GameControlSystem::m_startLevel() {
  start_to_play_notes = false;
  m_currentScore = 0;

  m_scores = Engine::GameManager::getInstance()->ecs->create();
  auto trans = m_scores->assign<Components::Transform>();
  auto render = m_scores->assign<Components::Render>();

  trans->Position = {65, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = to_string(m_currentScore);
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  m_percents = Engine::GameManager::getInstance()->ecs->create();
  trans = m_percents->assign<Components::Transform>();
  render = m_percents->assign<Components::Render>();

  trans->Position = {0, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = to_string((int)(0/currentLevelInfo.total_keys));
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  auto ent = Engine::GameManager::getInstance()->ecs->create();
  trans = ent->assign<Components::Transform>();
  render = ent->assign<Components::Render>();

  trans->Position = {0, 1, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "%";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  // Calculate speed for this level;
  for (volatile int i = 0; i < currentLevelInfo.total_keys; i++) {
    if (i == 0) {
      currentLevelInfo.key_speeds.emplace_back(38. / chrono::duration_cast<chrono::milliseconds>(currentLevelInfo.key_duration[i]).count());
    }
    currentLevelInfo.key_speeds.emplace_back((38. / chrono::duration_cast<chrono::milliseconds>(currentLevelInfo.key_duration[i-1]).count()));
  }

  // Generate position for this play
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> distrib(0, 3);

  const int RAND_NUM = 4;

  for (volatile int i = 0; i < currentLevelInfo.total_keys; i++) {
    if (i == 0) {
      currentLevelInfo.key_pos.emplace_back(distrib(gen));
    }
    currentLevelInfo.key_pos.emplace_back(distrib(gen));
  }

  if (currentLevelInfo.key_notes[0]) {
    m_createNote(0, currentLevelInfo.key_pos[0]);
  } else {
    start_to_play_notes = true;
  }


  buzzer_thread = new Thread();
  // Considered touched the line
  buzzer_thread->start([&, this]()->void{
    // waiting to play
    while (true) {
      if (this->start_to_play_notes) break;
    }


    for(int i = 0; i < this->currentLevelInfo.total_keys; i++){         //iterate through the C_major_scale array
      if(i != 0 && i != this->currentLevelInfo.total_keys - 1 && this->currentLevelInfo.key_notes[i+1])
        this->m_createNote(i+1, this->currentLevelInfo.key_pos[i+1]);
      this->m_playNote(this->currentLevelInfo.key_tones[i], this->currentLevelInfo.key_duration[i]);    //pass the note at position C_major_scale[i] to function
      // Update displayed infos
      this->m_percents->get<Components::Render>()->Data.text_Data = to_string((int)(((double)(i+1)/this->currentLevelInfo.total_keys)*100));
    }
    Engine::GameManager::getInstance()->buzzer->pulsewidth_us(0);  //turn off buzzer

    // Finalize game
    this->finishedGameLevel();
    delete this->buzzer_thread;
  });
}

void Systems::GameControlSystem::finishedGameLevel() {
  Engine::GameManager::getInstance()->ecs->disableSystem(Engine::GameManager::getInstance()->m_GameControlSystem);
  Engine::GameManager::getInstance()->ecs->disableSystem(Engine::GameManager::getInstance()->m_PeripheralCheckSystem_Game);

  Engine::GameManager::getInstance()->ecs->each<Components::Transform>([=](ECS::Entity *ent,
                                                                                             ECS::ComponentHandle<Components::Transform> trans) -> void {
    // Destroy all
    Engine::GameManager::getInstance()->ecs->destroy(ent);
  });

  Engine::GameManager::getInstance()->ecs->each<Components::Render>(
      [&](ECS::Entity *ent,
          ECS::ComponentHandle<Components::Render> render) -> void {
        Engine::GameManager::getInstance()->ecs->destroy(ent, true);
      }
  );

  Engine::GameManager::getInstance()->ecs->each<Components::UIRender>(
      [&](ECS::Entity *ent,
          ECS::ComponentHandle<Components::UIRender> render) -> void {
        Engine::GameManager::getInstance()->ecs->destroy(ent, true);
      }
  );

  Engine::GameManager::getInstance()->lcd->clear();

  // Store exp
  log_info("Score for this play: %d", m_currentScore);
  Engine::GameManager::getInstance()->m_Player->Exp += m_currentScore;
  if (Engine::GameManager::getInstance()->m_Player->Exp > 500) {
    // Level upgrade
    Engine::GameManager::getInstance()->m_Player->Level ++;
    Engine::GameManager::getInstance()->m_Player->Exp -= 500;
  }

  Engine::GameManager::getInstance()->m_makeSelectionMenu();

  Engine::GameManager::getInstance()->ecs->enableSystem(Engine::GameManager::getInstance()->m_UIControlSystem);
  Engine::GameManager::getInstance()->ecs->enableSystem(Engine::GameManager::getInstance()->m_PeripheralCheckSystem_UI);
}

void Systems::GameControlSystem::receive(ECS::World *world, const Events::KeypressEvent &event) {
  // ToDo: Multi Keys may not working
  world->each<Components::Transform, Components::Note>(
      [&](ECS::Entity *ent, ECS::ComponentHandle<Components::Transform> trans, ECS::ComponentHandle<Components::Note> note) -> void {
        float dis;
        switch (event.id) {
          case 0:
            if (get<0>(trans->Position) == 28) {
              dis = std::fabs((get<1>(trans->Position) - 38));
              if (dis <= 3) {
                m_currentScore += note->Score;
                world->destroy(ent);
              }
            }
            break;
          case 1:
            if (get<0>(trans->Position) == 35) {
              dis = std::fabs((get<1>(trans->Position) - 38));
              if (dis <= 3) {
                m_currentScore += note->Score;
                world->destroy(ent);
              }
            }
            break;
          case 2:
            if (get<0>(trans->Position) == 42) {
              dis = std::fabs((get<1>(trans->Position) - 38));
              if (dis <= 3) {
                m_currentScore += note->Score;
                world->destroy(ent);
              }
            }
            break;
          case 3:
            if (get<0>(trans->Position) == 49) {
              dis = std::fabs((get<1>(trans->Position) - 38));
              if (dis <= 3) {
                m_currentScore += note->Score;
                world->destroy(ent);
              }
            }
            break;
        }
      }
  );

}

void Systems::PeripheralCheckSystem_UI::tick(ECS::World *world, float deltaTime) {
  for (int8_t i = 0; i < 2; ++i) {
    if (Engine::GameManager::getInstance()->joystics[i]->get_direction() != Direction::CENTRE) {
      ThisThread::sleep_for(150ms);
      if (Engine::GameManager::getInstance()->joystics[i]->get_direction() != Direction::CENTRE)
        Engine::GameManager::getInstance()->ecs->emit<Events::JoystickUpdateEvent>(
            {
                i,
                Engine::GameManager::getInstance()->joystics[i]->get_direction(),
                Engine::GameManager::getInstance()->joystics[i]->get_mag(),
                Engine::GameManager::getInstance()->joystics[i]->get_angle(),
                Engine::GameManager::getInstance()->joystics[i]->get_coord(),
                Engine::GameManager::getInstance()->joystics[i]->get_mapped_coord(),
                Engine::GameManager::getInstance()->joystics[i]->get_polar()
            }
        );
    }
  }

  for (int8_t i = 0; i < 4; ++i) {
    if (!Engine::GameManager::getInstance()->keys[i]->read()) {
      ThisThread::sleep_for(80ms);
      if (!Engine::GameManager::getInstance()->keys[i]->read())
        Engine::GameManager::getInstance()->ecs->emit<Events::KeypressEvent>({i});
    }
  }
}

void Systems::PeripheralCheckSystem_Game::tick(ECS::World *world, float deltaTime) {
  for (int8_t i = 0; i < 4; ++i) {
    if (!Engine::GameManager::getInstance()->keys[i]->read()) {
        Engine::GameManager::getInstance()->ecs->emit<Events::KeypressEvent>({i});
    }
  }
}
