/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.cpp   
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#include "GameEngine.h"
#include "Components.h"
#include "Systems.h"
#include "log.h"
#include "GlobalDefines.h"
#include "Spirits.h"
#include "Events.h"

void Engine::GameManager::Init() {
  log_info("Initializing GameEngine...");
  log_info("AppName: %s", GAME_NAME);
  log_info("AppVersion: %s", VERSION_NUMBER);
  log_info("AppAuthor: %s", AUTHOR);

  m_initPeripherals();

  ecs = ECS::World::createWorld();
  m_registerSystems();

  m_initEarlyData();
}
Engine::GameManager::~GameManager() {
  ecs->destroyWorld();
  delete ecs;

  delete m_Player;

  m_freePeripherals();
}

void Engine::GameManager::m_initPeripherals() {
  log_info("Initializing Peripherals...");

  log_info("Initializing LCD...");
  lcd = new N5110(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
  lcd->init(LPH7366_1);
  lcd->setContrast(0.45);

  log_info("Initializing Keys");
  keys[0] = new DigitalIn(PA_5);
  keys[1] = new DigitalIn(PA_6);
  keys[2] = new DigitalIn(PA_7);
  keys[3] = new DigitalIn(PC_8);
  for (auto & key : keys) {
    key->mode(PinMode::PullDown);
  }

  log_info("Initializing LED");
  led_bgr = new BusOut{PC_14, PC_15, PH_0};
  led_bgr->write(0b000);

  log_info("Initializing Buzzer");
  buzzer = new PwmOut(PA_15);

  log_info("Initializing Joysticks");
  joystics[0] = new Joystick(PC_1, PC_0);  // y  x
  joystics[1] = new Joystick(PA_4, PB_0);
  for (auto &p : joystics)
    p->init();

  log_info("Successfully Initialized Peripherals...");
}
void Engine::GameManager::m_freePeripherals() const {
  delete lcd;

  for (auto &p : keys)
    delete p;

  delete led_bgr;

  delete buzzer;

  for (auto &p : joystics)
    delete p;
}
void Engine::GameManager::m_initEarlyData() {
  log_info("Initializing Early Data...");
  using namespace Components;

  m_Player = new Components::Player;
  m_Player->Exp = 0;
  m_Player->Level = 0;

  log_info("Loading Main menu...");
  m_makeMainMenu();
}

void Engine::GameManager::m_makeMainMenu() {
  using namespace Components;
  using namespace ECS;

  // Init basic menu display
  auto ent = ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();

  trans->Position = {0, 0, 0};

  trans->Rotation = {0, 0, 0};

  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_mainMenu;
  render->Visible = true;
  render->x = 84;
  render->y = 48;

  // Init selectable items
  auto ent2 = ecs->create();
  auto ent2_UISelect = ent2->assign<Components::UIRender>();

  std::tuple<float, float, float> pos[4] = {{31, 13, 0}, {22, 24, 0}, {30, 34, 0}, {43, 31, 0}};

  int i = 0;
  for (auto &p : pos) {
    Components::UIRender::UIComp_t comp;
    comp.trans.Position = p;
    comp.trans.Rotation = {0, 0, 0};
    comp.trans.Scale = {1, 1, 1};

    comp.spirit_Data = (i < 3) ? m_mainMenu_SelectIndicator : m_mainMenu_SelectIndicatorUp;
    comp.id = i;

    comp.x = (i < 3) ? 3 : 5;
    comp.y = (i < 3) ? 5 : 3;

    switch (i) {
      case 0:
        comp.callback_functionA = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeHelpInfo();
        };
        break;
      case 1:
        comp.callback_functionA = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_settingMenu();
        };
        break;
      case 2:
        comp.callback_functionA = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeAboutInfo();
        };
        break;
      case 3:
        comp.callback_functionA = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeSelectionMenu();
        };
        break;
    }

    ent2_UISelect->m_comps.emplace_back(comp);
    ++i;
  }

  ent2_UISelect->selected = 3;

  auto ent3 = GameManager::getInstance()->ecs->create();
  trans = ent3->assign<Components::Transform>();
  render = ent3->assign<Components::Render>();

  trans->Position = {65, 2, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "LV:";
  render->Visible = true;
  render->Override = true;
  render->size = 0;

  ent3 = GameManager::getInstance()->ecs->create();
  trans = ent3->assign<Components::Transform>();
  render = ent3->assign<Components::Render>();

  trans->Position = {65, 4, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Exp:";
  render->Visible = true;
  render->Override = true;
  render->size = 0;

  ent3 = GameManager::getInstance()->ecs->create();
  trans = ent3->assign<Components::Transform>();
  render = ent3->assign<Components::Render>();

  trans->Position = {65, 3, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  auto level = to_string(Engine::GameManager::getInstance()->m_Player->Level);
  render->Data.text_Data = level;
  render->Visible = true;
  render->Override = true;
  render->size = 0;

  ent3 = GameManager::getInstance()->ecs->create();
  trans = ent3->assign<Components::Transform>();
  render = ent3->assign<Components::Render>();

  trans->Position = {65, 5, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = to_string(Engine::GameManager::getInstance()->m_Player->Exp).c_str();
  render->Visible = true;
  render->Override = true;
  render->size = 0;
}

void Engine::GameManager::m_registerSystems() {
  m_PeripheralCheckSystem_UI = ecs->registerSystem(new Systems::PeripheralCheckSystem_UI);
  m_PeripheralCheckSystem_Game = ecs->registerSystem(new Systems::PeripheralCheckSystem_Game);
  // m_TransformSystem = ecs->registerSystem(new Systems::TransformSystem);
  m_RenderSystem = ecs->registerSystem(new Systems::RenderSystem);
  m_UIControlSystem = ecs->registerSystem(new Systems::UIControlSystem);
  m_GameControlSystem = ecs->registerSystem(new Systems::GameControlSystem);

  ecs->disableSystem(m_GameControlSystem);
  ecs->disableSystem(m_PeripheralCheckSystem_Game);
}

void Engine::GameManager::m_makeSelectionMenu() {
  auto ent = GameManager::getInstance()->ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();

  trans->Position = {0, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_songSelection;
  render->Visible = true;
  render->x = 84;
  render->y = 48;

  // Make song items
  ent = GameManager::getInstance()->ecs->create();
  trans = ent->assign<Components::Transform>();
  render = ent->assign<Components::Render>();

  trans->Position = {3, 20, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->size = 0;
  render->Visible = true;
  render->Data.spirit_Data = m_song1Font;
  render->x = 23;
  render->y = 5;

  ent = GameManager::getInstance()->ecs->create();
  trans = ent->assign<Components::Transform>();
  render = ent->assign<Components::Render>();

  trans->Position = {45, 15, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->size = 0;
  render->Visible = true;
  render->Data.spirit_Data = m_song1Font;
  render->x = 23;
  render->y = 5;

  auto ent2 = GameManager::getInstance()->ecs->create();
  auto render2 = ent2->assign<Components::UIRender>();
  std::tuple<float, float, float> pos[1] = {{80, 15, 0}};

  int i = 0;
  for (auto &p : pos) {
    Components::UIRender::UIComp_t comp;
    comp.trans.Position = p;
    comp.trans.Rotation = {0, 0, 0};
    comp.trans.Scale = {1, 1, 1};

    comp.spirit_Data = m_mainMenu_SelectIndicator;
    comp.id = i;

    comp.x = 3;
    comp.y = 5;

    switch (i) {
      case 0:
        comp.callback_functionA = [&, i]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeGameLevel(i);
        };
        comp.callback_functionB = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeMainMenu();
        };
        break;
    }

    render2->m_comps.emplace_back(comp);
    ++i;
  }

  render2->selected = 0;

  auto ent3 = GameManager::getInstance()->ecs->create();
  trans = ent3->assign<Components::Transform>();
  render = ent3->assign<Components::Render>();

  trans->Position = {3, 5, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "BASIC";
  render->Visible = true;
  render->Override = true;
  render->size = 0;
}

void Engine::GameManager::m_makeGameLevel(int level) {
  // Make backround
  auto ent = GameManager::getInstance()->ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();

  trans->Position = {0, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_songPlaying;
  render->Visible = true;
  render->x = 84;
  render->y = 48;
  render->Override = true;

  GameManager::getInstance()->ecs->enableSystem(m_GameControlSystem);
  GameManager::getInstance()->ecs->enableSystem(m_PeripheralCheckSystem_Game);

  ((Systems::GameControlSystem*)GameManager::getInstance()->m_GameControlSystem)->initialGameLevel(level);

  GameManager::getInstance()->ecs->disableSystem(m_UIControlSystem);
  GameManager::getInstance()->ecs->disableSystem(m_PeripheralCheckSystem_UI);
}

void Engine::GameManager::m_makeAboutInfo() {
  // Init basic menu display
  auto ent = ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();

  trans->Position = {0, 0, 0};

  trans->Rotation = {0, 0, 0};

  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_blank;
  render->Visible = true;
  render->x = 84;
  render->y = 48;
  // Display About
  auto ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "About ArcEmbed";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {6, 1, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "A Music Game";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 3, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Brought to you by";
  render->Visible = true;
  render->Override = true;
  render->size = 0;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {6, 4, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Yuanzhen Gan";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {18, 5, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "(C) 2024";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  auto ent2 = GameManager::getInstance()->ecs->create();
  auto render2 = ent2->assign<Components::UIRender>();
  std::tuple<float, float, float> pos[1] = {{80, 15, 0}};

  int i = 0;
  for (auto &p : pos) {
    Components::UIRender::UIComp_t comp;
    comp.Visible = false;

    switch (i) {
      case 0:
        comp.callback_functionB = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeMainMenu();
        };
        break;
    }

    render2->m_comps.emplace_back(comp);
    ++i;
  }

  render2->selected = 0;
}

void Engine::GameManager::m_makeHelpInfo() {
  // Init basic menu display
  auto ent = ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();

  trans->Position = {0, 0, 0};

  trans->Rotation = {0, 0, 0};

  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_blank;
  render->Visible = true;
  render->x = 84;
  render->y = 48;

  // Display Help
  auto ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {4, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "ArcEmbed Help";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 1, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Key A: Enter";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 2, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Key B: Back";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 4, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Column Mapping";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 5, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "ABCD -> 1234";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  auto ent2 = GameManager::getInstance()->ecs->create();
  auto render2 = ent2->assign<Components::UIRender>();
  std::tuple<float, float, float> pos[1] = {{80, 15, 0}};

  int i = 0;
  for (auto &p : pos) {
    Components::UIRender::UIComp_t comp;
    comp.Visible = false;

    switch (i) {
      case 0:
        comp.callback_functionB = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeMainMenu();
        };
        break;
    }

    render2->m_comps.emplace_back(comp);
    ++i;
  }

  render2->selected = 0;
}

void Engine::GameManager::m_settingMenu() {
  // Init basic menu display
  auto ent = ecs->create();
  auto trans = ent->assign<Components::Transform>();
  auto render = ent->assign<Components::Render>();

  trans->Position = {0, 0, 0};

  trans->Rotation = {0, 0, 0};

  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Spirit;
  render->Data.spirit_Data = m_blank;
  render->Visible = true;
  render->x = 84;
  render->y = 48;

  // Display Setting
  auto ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {0, 0, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "ArcEmbed Sett.";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  ent1 = GameManager::getInstance()->ecs->create();
  trans = ent1->assign<Components::Transform>();
  render = ent1->assign<Components::Render>();

  trans->Position = {4, 3, 0};
  trans->Rotation = {0, 0, 0};
  trans->Scale = {1, 1, 1};

  render->Type = Components::Render::Type_e::Text;
  render->Data.text_Data = "Not available";
  render->Visible = true;
  render->Override = true;
  render->size = 1;

  auto ent2 = GameManager::getInstance()->ecs->create();
  auto render2 = ent2->assign<Components::UIRender>();
  std::tuple<float, float, float> pos[1] = {{80, 15, 0}};

  int i = 0;
  for (auto &p : pos) {
    Components::UIRender::UIComp_t comp;
    comp.Visible = false;

    switch (i) {
      case 0:
        comp.callback_functionB = [&]() -> void {
          GameManager::getInstance()->ecs->each<Components::Render>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::Render> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->ecs->each<Components::UIRender>(
              [&](ECS::Entity *ent,
                  ECS::ComponentHandle<Components::UIRender> render) -> void {
                GameManager::getInstance()->ecs->destroy(ent, true);
              }
          );

          GameManager::getInstance()->lcd->clear();

          GameManager::getInstance()->m_makeMainMenu();
        };
        break;
    }

    render2->m_comps.emplace_back(comp);
    ++i;
  }

  render2->selected = 0;
}

