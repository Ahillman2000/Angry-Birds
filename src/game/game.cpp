#include <string>

#include "game.h"
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Logger.hpp>
#include <Engine/Point2D.h>
#include <Engine/Sprite.h>
#include <cmath>
/**
 *   @brief   Default Constructor.
 */
AngryBirds::AngryBirds(ASGE::GameSettings settings) : OGLGame(settings)
{
  renderer->setClearColour(ASGE::COLOURS::SLATEGRAY);
  inputs->use_threads = false;
  toggleFPS();

  initBackgrounds();

  // create a camera pointing at the mid-point of the screen
  camera = ASGE::Camera2D{ static_cast<float>(ASGE::SETTINGS.window_width),
                           static_cast<float>(ASGE::SETTINGS.window_height) };

  camera.lookAt(
    ASGE::Point2D{ static_cast<float>(-ASGE::SETTINGS.window_width) / 2.f,
                   static_cast<float>(-ASGE::SETTINGS.window_height) / 2.f });

  logStartup();
}

void AngryBirds::logStartup() const
{
  // these are just examples of the logging system.. you dont need to keep em
  std::ostringstream stream{ "initialised complete" };
  Logging::ERRORS(stream.str());
  Logging::WARN(stream.str());
  Logging::INFO(stream.str());
  Logging::DEBUG(stream.str());
  Logging::TRACE(stream.str());
  Logging::log(
    Logging::timestamp() + " \x1b[35;1m[CUSTOM]\x1b[0m " + stream.str() + '\n');
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
AngryBirds::~AngryBirds()
{
  // delete menu_background;
  // DELETE SPRITES
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));

  this->inputs->unregisterCallback(static_cast<unsigned int>(move_callback_id));
}

bool AngryBirds::initBackgrounds()
{
  if (!backgrounds[0].initialiseSprite(renderer.get(), "menu.jpg"))
  {
    return false;
  }
  if (!backgrounds[1].initialiseSprite(renderer.get(), "lvl1.png"))
  {
    return false;
  }
  if (!backgrounds[2].initialiseSprite(renderer.get(), "lvl2.png"))
  {
    return false;
  }
  if (!backgrounds[3].initialiseSprite(renderer.get(), "lvl3.png"))
  {
    return false;
  }

  for (int i = 0; i < 4; i++)
  {
    backgrounds[i].getSprite()->width(
      static_cast<float>(ASGE::SETTINGS.window_width));
    backgrounds[i].getSprite()->height(
      static_cast<float>(ASGE::SETTINGS.window_height));
  }
  return true;
}

bool AngryBirds::initBirds()
{
  if (!birds[0].initialiseSprite(
        renderer.get(), "kenney_animalpackredux/PNG/Round/chicken.png"))
  {
    return false;
  }
  else if (!birds[1].initialiseSprite(
             renderer.get(), "kenney_animalpackredux/PNG/Round/parrot.png"))
  {
    return false;
  }
  else if (!birds[2].initialiseSprite(
             renderer.get(), "kenney_animalpackredux/PNG/Round/penguin.png"))
  {
    return false;
  }

  birds[0].getSprite()->width(64);
  birds[0].getSprite()->height(72.5);
  birds[0].getSprite()->xPos(20);
  birds[0].getSprite()->yPos(game_height - 180);

  birds[1].getSprite()->width(64.5);
  birds[1].getSprite()->height(64);
  birds[1].getSprite()->xPos(90);
  birds[1].getSprite()->yPos(game_height - 165);

  birds[2].getSprite()->width(70);
  birds[2].getSprite()->height(64);
  birds[2].getSprite()->xPos(160);
  birds[2].getSprite()->yPos(game_height - 164);

  return true;
}

bool AngryBirds::initPigs()
{
  for (int i = 0; i < pig_count; i++)
  {
    if (!pigs[i].initialiseSprite(
          renderer.get(), "kenney_animalpackredux/PNG/Round/pig.png"))
    {
      return false;
    }
    pigs[i].getSprite()->width(72.0);
    pigs[i].getSprite()->height(64.0);
  }
  pigs[0].getSprite()->xPos(game_width - 300);
  pigs[0].getSprite()->yPos(game_height - 145);

  pigs[1].getSprite()->xPos(game_width - 200);
  pigs[1].getSprite()->yPos(game_height - 160);

  pigs[2].getSprite()->xPos(game_width - 100);
  pigs[2].getSprite()->yPos(game_height - 170);

  return true;
}

void AngryBirds::initSprites()
{
  initBackgrounds();
  initBirds();
  initPigs();
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool AngryBirds::init()
{
  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &AngryBirds::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &AngryBirds::clickHandler, this);

  move_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_MOVE, &AngryBirds::MoveHandler, this);

  initSprites();

  intro_text = { renderer->getDefaultFont(),
                 "PRESS ENTER TO START",
                 static_cast<int>(game_width / 2 - 95),
                 static_cast<int>(game_height * 0.8),
                 ASGE::COLOURS::BLACK };
  intro_text.setZOrder(5);

  score_text = { renderer->getDefaultFont(),
                 "score: " + std::to_string(score),
                 1100,
                 690,
                 ASGE::COLOURS::GREEN };
  score_text.setZOrder(5);

  win_text = { renderer->getDefaultFont(),
               "YOU WIN",
               static_cast<int>(game_width / 2 - 20),
               static_cast<int>(game_height * 0.82),
               ASGE::COLOURS::YELLOW };
  win_text.setZOrder(5);

  lose_text = { renderer->getDefaultFont(),
                "YOU LOSE",
                static_cast<int>(game_width / 2 - 45),
                static_cast<int>(game_height * 0.8),
                ASGE::COLOURS::GREEN };
  lose_text.setZOrder(5);

  lose_text_2 = { renderer->getDefaultFont(),
                  "PRESS R TO RESTART LEVEL",
                  static_cast<int>(game_width / 2 - 120),
                  static_cast<int>(game_height * 0.9),
                  ASGE::COLOURS::GREEN };
  lose_text_2.setZOrder(5);

  return true;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void AngryBirds::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (key->key == ASGE::KEYS::KEY_ENTER && in_menu)
  {
    in_menu = false;
    in_game = true;
  }

  if (key->key == ASGE::KEYS::KEY_ENTER && win_state)
  {
    win_state = false;
    in_menu   = true;

    level_count++;
    // std::cout << "level" + std::to_string(level_count) << std::endl;
    restartGame();
  }

  if (key->key == ASGE::KEYS::KEY_R && lose_state)
  {
    lose_state = false;
    in_menu    = true;

    restartGame();
  }
}

void AngryBirds::MoveHandler(ASGE::SharedEventData data)
{
  auto move = static_cast<const ASGE::MoveEvent*>(data.get());

  x_move = move->xpos;
  y_move = move->ypos;

  // Logging::DEBUG("x_move: " + std::to_string(x_move));
  // Logging::DEBUG("y_move: " +std::to_string(y_move));
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void AngryBirds::clickHandler(ASGE::SharedEventData data)
{
  // action of the mouse to tell difference between click and drag
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  x_pos = click->xpos;
  y_pos = click->ypos;

  if (in_game)
  {
    if (click->action == 1)
    {
      mouse_down = true;

      start_x_pos = x_pos;
      start_y_pos = y_pos;

      // std::cout << "x_pos down: " << start_x_pos << std::endl;
      // std::cout << "y_pos down: " << start_y_pos << std::endl;
    }
    if (click->action == 0)
    {
      mouse_down = false;

      end_x_pos = x_pos;
      end_y_pos = y_pos;

      // std::cout << "x_pos up: " << end_x_pos << std::endl;
      // std::cout << "y_pos up: " << end_y_pos << std::endl;
    }

    bird_x_velocity = static_cast<float>(start_x_pos - end_x_pos) * 2;
    bird_y_velocity = static_cast<float>(end_y_pos - start_y_pos) * 2;

    for (int i = 0; i < bird_count; i++)
    {
      if (clickOn(birds[i].getSprite(), x_pos, y_pos) && birds[i].visibility)
      {
        selected_bird = i;
        // target_distance = game_width;
      }
    }
  }
}

bool AngryBirds::clickOn(
  const ASGE::Sprite* sprite, double mouse_x, double mouse_y)
{
  if (
    mouse_x > sprite->getGlobalBounds().v1.x &&
    mouse_x < sprite->getGlobalBounds().v3.x &&
    mouse_y > sprite->getGlobalBounds().v1.y &&
    mouse_y < sprite->getGlobalBounds().v3.y)
  {
    return true;
  }
  return false;
}

bool AngryBirds::isOverlapping(ASGE::Sprite* sprite1, ASGE::Sprite* sprite2)
{
  if (
    sprite2->xPos() < sprite1->xPos() + sprite1->width() &&
    sprite2->xPos() + sprite2->width() > sprite1->xPos() &&

    sprite2->yPos() < sprite1->yPos() + sprite1->height() &&
    sprite2->yPos() + sprite2->height() > sprite1->yPos())
  {
    return true;
  }
  return false;
}

void AngryBirds::birdMovement(const ASGE::GameTime& game_time)
{
  auto dt_sec = float(game_time.delta.count() / 1000.0);

  if (birds[selected_bird].fired && birds[selected_bird].visibility)
  {
    // std::cout << "birds moving " << std::endl;

    birds[selected_bird].getSprite()->xPos(
      birds[selected_bird].getSprite()->xPos() + (bird_x_velocity * dt_sec));

    birds[selected_bird].getSprite()->yPos(
      birds[selected_bird].getSprite()->yPos() - bird_y_velocity * dt_sec);

    bird_y_velocity -= powf(GRAVITY, 2) * dt_sec;

    // std::cout << "bird_x_velocity: " << bird_x_velocity << std::endl;
    // std::cout << "bird_y_velocity: " << bird_y_velocity << std::endl;
  }
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void AngryBirds::update(const ASGE::GameTime& game_time)
{
  // auto dt_sec = float(game_time.delta.count() / 1000.0);
  // make sure you use delta time in any movement calculations!

  if (in_game)
  {
    score_text.setString("score: " + std::to_string(score));

    camera.update(game_time);

    if (mouse_down)
    {
      birds[selected_bird].getSprite()->xPos(
        static_cast<float>(x_move) -
        birds[selected_bird].getSprite()->width() / 2);

      birds[selected_bird].getSprite()->yPos(
        static_cast<float>(y_move) -
        birds[selected_bird].getSprite()->height() / 2);

      birds[selected_bird].ready_to_fire = true;
    }

    else if (!mouse_down && birds[selected_bird].ready_to_fire)
    {
      birds[selected_bird].fired = true;
      birdMovement(game_time);
    }

    for (int i = 0; i < bird_count; i++)
    {
      if (
        (birds[i].getSprite()->yPos() > game_height ||
         birds[i].getSprite()->xPos() > game_width) &&
        birds[i].visibility)
      {
        number_of_birds_left--;
        birds[i].visibility = false;
        // std::cout << "birds off screen" << std::endl;
        // std::cout << "num of birds: " << number_of_birds_left << std::endl;
        bird_y_velocity = 200;
      }

      for (int j = 0; j < pig_count; j++)
      {
        if (
          isOverlapping(birds[i].getSprite(), pigs[j].getSprite()) &&
          birds[i].visibility && pigs[j].visibility)
        {
          // std::cout << "bird / pig collision" << std::endl;
          pigs[j].visibility  = false;
          birds[i].visibility = false;
          number_of_pigs_left--;
          number_of_birds_left--;
          // std::cout << "num of birds: " << number_of_birds_left << std::endl;
          // std::cout << "num of pigs: " << number_of_pigs_left << std::endl;
          score += 25;
          bird_y_velocity = 200;
        }
      }
    }

    if (number_of_pigs_left == 0)
    {
      in_game   = false;
      win_state = true;
    }

    if (number_of_birds_left == 0 && number_of_pigs_left > 0)
    {
      in_game    = false;
      lose_state = true;
    }
  }
  else if (lose_state)
  {
    pigs[0].getSprite()->width(216);
    pigs[0].getSprite()->height(192);

    pigs[0].getSprite()->xPos(
      game_width / 2 - pigs[0].getSprite()->width() / 2);
    pigs[0].getSprite()->yPos(
      game_height / 2 - pigs[0].getSprite()->height() / 2);
  }
  else if (win_state)
  {
    score_text.setColour(ASGE::COLOURS::YELLOW);
    score_text.setPosition(
      { game_width / 2 - 28, static_cast<float>(game_height * 0.85) });
  }
}

void AngryBirds::restartGame()
{
  initSprites();

  for (int i = 0; i < 3; ++i)
  {
    birds[i].visibility = true;
    pigs[i].visibility  = true;
  }

  bird_count           = 3;
  number_of_birds_left = 3;

  bird_x_velocity = 0;
  bird_y_velocity = 0;

  selected_bird = -1;

  pig_count           = 3;
  number_of_pigs_left = 3;

  score = 0;
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void AngryBirds::render()
{
  renderer->setFont(0);

  if (in_menu)
  {
    renderer->renderSprite(*backgrounds[0].getSprite());
    renderer->renderText(intro_text);
  }

  else if (in_game)
  {
    if (level_count == 1)
    {
      renderer->renderSprite(*backgrounds[1].getSprite());
    }

    else if (level_count == 2)
    {
      renderer->renderSprite(*backgrounds[2].getSprite());
    }
    else
    {
      renderer->renderSprite(*backgrounds[3].getSprite());
    }

    renderer->renderText(score_text);

    for (int i = 0; i < bird_count; i++)
    {
      if (birds[i].visibility)
      {
        renderer->renderSprite(*birds[i].getSprite());
      }
    }

    for (int i = 0; i < pig_count; i++)
    {
      if (pigs[i].visibility)
      {
        renderer->renderSprite(*pigs[i].getSprite());
      }
    }

    renderer->setProjectionMatrix(camera.getView());
  }
  else if (win_state)
  {
    renderer->renderSprite(*backgrounds[0].getSprite());
    renderer->renderText(win_text);
    renderer->renderText(score_text);
  }
  else if (lose_state)
  {
    renderer->renderSprite(*pigs[0].getSprite());
    renderer->renderText(lose_text);
    renderer->renderText(lose_text_2);
  }
}
