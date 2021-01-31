//
// Created by a3-hillman on 23/02/2020.
//

#ifndef ANGRYBIRDS_GAMEOBJECT_H
#define ANGRYBIRDS_GAMEOBJECT_H

#include "Vector.h"
#include <Engine/Renderer.h>
#include <Engine/Sprite.h>

class GameObject
{
 public:
  GameObject();
  ~GameObject();

  bool initialiseSprite(ASGE::Renderer* renderer, std::string filename);
  ASGE::Sprite* getSprite();
  bool visibility    = true;
  bool ready_to_fire = false;
  bool fired         = false;

 private:
  ASGE::Sprite* sprite = nullptr;
};

#endif // ANGRYBIRDS_GAMEOBJECT_H
