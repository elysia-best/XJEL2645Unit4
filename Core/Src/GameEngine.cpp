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
  m_freePeripherals();
}

void Engine::GameManager::m_initPeripherals() {
  log_info("Initializing Peripherals...");

  log_info("Initializing LCD...");
  lcd = new N5110(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
  lcd->init(LPH7366_1);
  lcd->setContrast(0.4);

  log_info("Initializing Keys");
  keys[0] = new InterruptIn(PA_5);
  keys[1] = new InterruptIn(PA_6);
  keys[2] = new InterruptIn(PA_7);
  keys[3] = new InterruptIn(PC_8);
  for (auto &p: keys)
    p->mode(PinMode::PullDown);

  log_info("Initializing LED");
  led_bgr = new BusOut{PC_14, PC_15, PH_0};
  led_bgr->write(0b000);

  log_info("Initializing Buzzer");
  buzzer = new PwmOut(PA_15);

  log_info("Initializing Joysticks");
  joystics[0] = new Joystick(PC_1, PC_0);  // y  x
  joystics[1] = new Joystick(PA_4, PB_0);
  for (auto &p: joystics)
    p->init();

  log_info("Successfully Initialized Peripherals...");
}
void Engine::GameManager::m_freePeripherals() const {
  delete lcd;

  for (auto &p: keys)
    delete p;

  delete led_bgr;

  delete buzzer;

  for (auto &p: joystics)
    delete p;
}
void Engine::GameManager::m_initEarlyData() {
  log_info("Initializing Early Data...");
  using namespace Components;

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
  for (auto &p  : pos) {
    Components::UIRender::UIComp_t comp;
    comp.trans.Position = p;
    comp.trans.Rotation = {0, 0, 0};
    comp.trans.Scale = {1, 1, 1};

    comp.spirit_Data = (i < 3) ? m_mainMenu_SelectIndicator : m_mainMenu_SelectIndicatorUp;
    comp.id = i;

    comp.x = (i < 3) ? 3 : 5;
    comp.y = (i < 3) ? 5 : 3;

    ent2_UISelect->m_comps.emplace_back(comp);
    ++i;
  }

  ent2_UISelect->selected = 3;
  ent2_UISelect->render_function = [=](){};
}

void Engine::GameManager::m_registerSystems() {
  ecs->registerSystem(new Systems::TransformSystem);
  ecs->registerSystem(new Systems::RenderSystem);
  ecs->registerSystem(new Systems::UIControlSystem);
}

void Engine::GameManager::m_checkPeripherals() {
  for (int8_t i = 0; i < 2; ++i) {
    if (joystics[i]->get_direction() != Direction::CENTRE) {
      thread_sleep_for(150);
      if (joystics[i]->get_direction() != Direction::CENTRE) {
        ecs->emit<Events::JoystickUpdateEvent>(
            {
                i,
                joystics[i]->get_direction(),
                joystics[i]->get_mag(),
                joystics[i]->get_angle(),
                joystics[i]->get_coord(),
                joystics[i]->get_mapped_coord(),
                joystics[i]->get_polar()
            }
        );
      }
    }
  }
}
