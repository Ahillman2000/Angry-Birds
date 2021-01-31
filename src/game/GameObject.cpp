//
// Created by a3-hillman on 23/02/2020.
//

#include "GameObject.h"
GameObject::GameObject() {}

GameObject::~GameObject()
{
  delete (sprite);
}

bool GameObject::initialiseSprite(ASGE::Renderer* renderer, std::string filename)
{
  sprite = renderer->createRawSprite();

  if (!sprite->loadTexture("/data/images/" + filename))
  {
    return false;
  }
  return true;
}

ASGE::Sprite* GameObject::getSprite()
{
  return sprite;
}