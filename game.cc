#include "game.h"
#include <iostream>
#include "cpputils/graphics/image_event.h"

void Game::Init() {
  my_player_.SetX(375);
  my_player_.SetY(500);
  CreateOpponents();
  CreateOpponentProjectiles();
  CreatePlayerProjectiles();
  game_screen_.AddMouseEventListener(*this);
  game_screen_.AddAnimationEventListener(*this);
}

void Game::CreateOpponents() {
  int numOpps = 5;
  int offset = (game_screen_.GetWidth() - 40 - 30) / (numOpps - 1);
  for (int i = 0; i < numOpps; i++) {
    Opponent my_opponent(20 + (i * offset), 20);
    opponents_.push_back(my_opponent);
  }
}

void Game::CreateOpponentProjectiles() {
  for (int i = 0; i < 5; i++) {
    OpponentProjectile my_opponent_projectile(100 * i, 100);
    opponent_projectiles_.push_back(my_opponent_projectile);
  }
}

void Game::CreatePlayerProjectiles() {
  for (int i = 0; i < 5; i++) {
    PlayerProjectile my_projectile(100 * i, 450);
    player_projectiles_.push_back(my_projectile);
  }
}

void Game::MoveGameElements() {
  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i].GetIsActive()) {
      opponents_[i].Move(game_screen_);
    }
  }

  for (int i = 0; i < opponent_projectiles_.size(); i++) {
    if (opponent_projectiles_[i].GetIsActive()) {
      opponent_projectiles_[i].Move(game_screen_);
    }
  }

  for (int i = 0; i < player_projectiles_.size(); i++) {
    if (player_projectiles_[i].GetIsActive()) {
      player_projectiles_[i].Move(game_screen_);
    }
  }
}

void Game::FilterIntersections() {
  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i].GetIsActive() && my_player_.GetIsActive() &&
        my_player_.IntersectsWith(&opponents_[i])) {
      // opponents_.erase(opponents_.begin() + i);
      opponents_[i].SetIsActive(false);
      my_player_.SetIsActive(false);
    } else {
      for (int j = 0; j < player_projectiles_.size(); j++) {
        if (opponents_[i].GetIsActive() &&
            player_projectiles_[j].GetIsActive() &&
            player_projectiles_[j].IntersectsWith(&opponents_[i])) {
          opponents_[i].SetIsActive(false);
          player_projectiles_[j].SetIsActive(false);
        }
      }
    }
  }

  for (int i = 0; i < opponent_projectiles_.size(); i++) {
    if (opponent_projectiles_[i].GetIsActive() && my_player_.GetIsActive() &&
        my_player_.IntersectsWith(&opponent_projectiles_[i])) {
      opponent_projectiles_[i].SetIsActive(false);
      my_player_.SetIsActive(false);
    }
  }
}

void Game::UpdateScreen() {
  game_screen_.DrawRectangle(0, 0, game_screen_.GetWidth(),
                             game_screen_.GetHeight(),
                             graphics::Color(255, 255, 255));
  if (my_player_.GetIsActive()) {
    my_player_.Draw(game_screen_);
  }

  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i].GetIsActive()) {
      opponents_[i].Draw(game_screen_);
    }
  }

  for (int i = 0; i < opponent_projectiles_.size(); i++) {
    if (opponent_projectiles_[i].GetIsActive()) {
      opponent_projectiles_[i].Draw(game_screen_);
    }
  }

  for (int i = 0; i < player_projectiles_.size(); i++) {
    if (player_projectiles_[i].GetIsActive()) {
      player_projectiles_[i].Draw(game_screen_);
    }
  }
}

void Game::Start() {
  game_screen_.ShowUntilClosed();
}

void Game::OnMouseEvent(const graphics::MouseEvent& event) {
  if (event.GetMouseAction() == graphics::MouseAction::kMoved ||
      event.GetMouseAction() == graphics::MouseAction::kDragged) {
    int new_x = event.GetX() - my_player_.GetWidth() / 2;
    int new_y = event.GetY() - my_player_.GetHeight() / 2;
    int old_x = my_player_.GetX();
    int old_y = my_player_.GetY();
    my_player_.SetX(new_x);
    my_player_.SetY(new_y);
    if (my_player_.IsOutOfBounds(game_screen_)) {
      my_player_.SetX(old_x);
      my_player_.SetY(old_y);
    }
  }
}

void Game::OnAnimationStep() {
  MoveGameElements();
  FilterIntersections();
  UpdateScreen();
  game_screen_.Flush();
}
