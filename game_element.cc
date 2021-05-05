#include "game_element.h"
#include <vector>
#include <memory>

bool GameElement::IntersectsWith(GameElement *game_element) {
  return !(GetX() > game_element->GetX() + game_element->GetWidth() ||
           game_element->GetX() > GetX() + GetWidth() ||
           GetY() > game_element->GetY() + game_element->GetHeight() ||
           game_element->GetY() > GetY() + GetHeight());
}

bool GameElement::IsOutOfBounds(const graphics::Image& game_screen) {
  if (GetX() < 0 || (GetX() + GetWidth()) > game_screen.GetWidth() ||
      GetY() < 0 || (GetY() + GetHeight()) > game_screen.GetHeight()) {
    return true;
  } else {
    return false;
  }
}
