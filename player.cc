#include "player.h"

#include <vector>

#include "cpputils/graphics/image.h"
#include "game.h"

void PlayerProjectile::Draw(graphics::Image &screen) {
  graphics::Color playerColor(3, 252, 36);
  screen.DrawRectangle(GetX() + 0, GetY() + 0, 5, 5, playerColor);
  screen.DrawRectangle(GetX() + 1, GetY() + 1, 3, 3, 255, 255, 255);
  screen.DrawRectangle(GetX() + 2, GetY() + 2, 1, 1, 0, 0, 0);
}

void PlayerProjectile::Move(const graphics::Image &game_screen) {
  SetY(GetY() - 5);
  if (IsOutOfBounds(game_screen)) {
    SetIsActive(false);
  }
}

void Player::Draw(graphics::Image &screen) {
  graphics::Color playerColor(3, 252, 36);
  screen.DrawRectangle(GetX() + 0, GetY() + 0, 30, 30, playerColor);
  screen.DrawRectangle(GetX() + 2, GetY() + 2, 26, 26, 255, 255, 255);
  screen.DrawRectangle(GetX() + 3, GetY() + 3, 24, 24, 0, 0, 0);
}

void Player::Move(const graphics::Image &game_screen) {}
