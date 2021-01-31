#pragma once
#include <Engine/Camera2D.hpp>
#include <Engine/OGLGame.h>
#include <string>

#include "GameObject.h"

/**
 *  An OpenGL Game based on ASGE.
 */
class AngryBirds : public ASGE::OGLGame
{
 public:
  explicit AngryBirds(ASGE::GameSettings settings);
  ~AngryBirds() final;
  bool init();

  float game_width  = static_cast<float>(ASGE::SETTINGS.window_width);
  float game_height = static_cast<float>(ASGE::SETTINGS.window_height);

  float GRAVITY = 9.8f;

  double start_x_pos = 0;
  double start_y_pos = 0;

  double end_x_pos = 0;
  double end_y_pos = 0;

  // float target_distance = 0;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void MoveHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);

  bool clickOn(const ASGE::Sprite* sprite, double mouse_x, double mouse_y);
  bool isOverlapping(ASGE::Sprite*, ASGE::Sprite*);

  void update(const ASGE::GameTime&) override;
  void render() override;

  void initSprites();
  bool initBackgrounds();
  bool initBirds();
  bool initPigs();

  void birdMovement(const ASGE::GameTime& game_time);

  void restartGame();

  GameObject backgrounds[4];

  GameObject birds[3];
  int bird_count           = 3;
  int number_of_birds_left = 3;

  float bird_x_velocity = 0;
  float bird_y_velocity = 0;

  int selected_bird = 0;

  GameObject pigs[3];
  int pig_count           = 3;
  int number_of_pigs_left = 3;

  int key_callback_id   = -1; /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */
  int move_callback_id  = -1;

  // int menu_option = 0;
  bool in_menu    = true;
  bool in_game    = false;
  int level_count = 1;

  int score       = 0;
  bool win_state  = false;
  bool lose_state = false;

  double x_move;
  double y_move;

  double x_pos;
  double y_pos;

  bool mouse_down = false;

  ASGE::Camera2D camera;
  void logStartup() const;

  ASGE::Text intro_text{};
  ASGE::Text score_text{};
  ASGE::Text win_text{};
  ASGE::Text lose_text{};
  ASGE::Text lose_text_2{};
};