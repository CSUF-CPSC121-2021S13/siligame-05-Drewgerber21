#include "opponent.h"

#include <vector>

#include "cpputils/graphics/image.h"
#include "game.h"

void OpponentProjectile::Draw(graphics::Image &screen) {
  graphics::Color opponentColor(223, 3, 252);
  screen.DrawRectangle(GetX() + 0, GetY() + 0, 5, 5, opponentColor);
  screen.DrawRectangle(GetX() + 1, GetY() + 1, 3, 3, 255, 255, 255);
  screen.DrawRectangle(GetX() + 2, GetY() + 2, 1, 1, 0, 0, 0);
}

void Opponent::Draw(graphics::Image &screen) {
  graphics::Color opponentColor(223, 3, 252);
  screen.DrawRectangle(GetX() + 0, GetY() + 0, 30, 30, opponentColor);
  screen.DrawRectangle(GetX() + 2, GetY() + 2, 26, 26, 255, 255, 255);
  screen.DrawRectangle(GetX() + 3, GetY() + 3, 24, 24, 0, 0, 0);
}

void Opponent::Move(const graphics::Image &screen) {
  if (going_right_ && GetX() + GetWidth() >= screen.GetWidth() - 20) {
    going_right_ = false;
  } else if (!going_right_ && GetX() <= 20) {
    going_right_ = true;
  }
  int offset = going_right_ ? 1 : -1;

  if (!moving_down_) {
    SetX(GetX() + offset * 3);
  }
  SetY(GetY() + 1);

  if (IsOutOfBounds(screen)) {
    SetIsActive(false);
  }
}

void OpponentProjectile::Move(const graphics::Image &game_screen) {
  if (GetIsActive()) {
    SetY(GetY() + 3);
  }
  if (IsOutOfBounds(game_screen)) {
    SetIsActive(false);
  }
}

std::unique_ptr<OpponentProjectile> Opponent::LaunchProjectile() {
  std::unique_ptr<OpponentProjectile> opp_bullet;
  opp_bullet = std::unique_ptr<OpponentProjectile>(new OpponentProjectile (GetX(), GetY()));
  return std::move(opp_bullet);
}
