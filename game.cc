#include "game.h"
#include <iostream>
#include "cpputils/graphics/image_event.h"
#include <memory>
#include <string>

void Game::Init() {
  my_player_.SetX(375);
  my_player_.SetY(500);
  game_screen_.AddMouseEventListener(*this);
  game_screen_.AddAnimationEventListener(*this);
}

void Game::CreateOpponents() {
  int numXOpps = 5;
  int Xoffset = (game_screen_.GetWidth() - 40 - 30) / numXOpps;

  for (int i = 0; i < numXOpps; i++) {
    std::unique_ptr<Opponent> opp;
    opp = std::unique_ptr<Opponent>(new Opponent (20 + (i * Xoffset), 20));
    opponents_.push_back(std::move(opp));
  }
}

void Game::MoveGameElements() {
  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i]->GetIsActive()) {
      opponents_[i]->Move(game_screen_);
    }
  }

  for (int i = 0; i < opponent_projectiles_.size(); i++) {
    if (opponent_projectiles_[i]->GetIsActive()) {
      opponent_projectiles_[i]->Move(game_screen_);
    }
  }

  for (int i = 0; i < player_projectiles_.size(); i++) {
    if (player_projectiles_[i]->GetIsActive()) {
      player_projectiles_[i]->Move(game_screen_);
    }
  }
}

void Game::FilterIntersections() {
  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i]->GetIsActive() && my_player_.GetIsActive() &&
        my_player_.IntersectsWith(opponents_[i].get())) {
      // Checks opponent and player intersection
      opponents_[i]->SetIsActive(false);
      my_player_.SetIsActive(false);
      has_lost_ = true;
    } else {
      for (int j = 0; j < player_projectiles_.size(); j++) {
        if (opponents_[i]->GetIsActive() &&
          player_projectiles_[j]->GetIsActive() &&
          player_projectiles_[j]->IntersectsWith(opponents_[i].get())) {
          // Checks Opponent and PlayerProjectile intersection
          opponents_[i]->SetIsActive(false);
          player_projectiles_[j]->SetIsActive(false);
          if(my_player_.GetIsActive()) {
            score_ += 1;
          }
        }
      }
    }
  }

  // Checks OpponentProjectile and Player intersection
  for (int i = 0; i < opponent_projectiles_.size(); i++) {
    if (opponent_projectiles_[i]->GetIsActive() && my_player_.GetIsActive() &&
        my_player_.IntersectsWith(opponent_projectiles_[i].get())) {
      opponent_projectiles_[i]->SetIsActive(false);
      my_player_.SetIsActive(false);
      has_lost_ = true;
    }
  }
}


void Game::RemoveInactive() {
  for (int i = opponents_.size() - 1; i >= 0; i--) {
    if (!opponents_[i]->GetIsActive()) {
      opponents_.erase(opponents_.begin() + i);
    }
  }
  for (int i = opponent_projectiles_.size() - 1; i >= 0; i--) {
    if (!opponent_projectiles_[i]->GetIsActive()) {
      opponent_projectiles_.erase(opponent_projectiles_.begin() + i);
    }
  }
  for (int i = player_projectiles_.size() - 1; i >= 0; i--) {
    if (!player_projectiles_[i]->GetIsActive()) {
      player_projectiles_.erase(player_projectiles_.begin() + i);
    }
  }
}


void Game::UpdateScreen() {
  if (!has_lost_) {
    game_screen_.DrawRectangle(0, 0, game_screen_.GetWidth(),
                              game_screen_.GetHeight(),
                              graphics::Color(0, 0, 0));
    std::string score_text = "Score: ";
    score_text += std::to_string(score_);
    game_screen_.DrawText(10, 10, score_text, 20, 255, 255, 255);

    if (my_player_.GetIsActive()) {
      my_player_.Draw(game_screen_);
    }

    for (int i = 0; i < opponents_.size(); i++) {
      if (opponents_[i]->GetIsActive()) {
        opponents_[i]->Draw(game_screen_);
      }
    }

    for (int i = 0; i < opponent_projectiles_.size(); i++) {
      if (opponent_projectiles_[i]->GetIsActive()) {
        opponent_projectiles_[i]->Draw(game_screen_);
      }
    }

    for (int i = 0; i < player_projectiles_.size(); i++) {
      if (player_projectiles_[i]->GetIsActive()) {
        player_projectiles_[i]->Draw(game_screen_);
      }
    }
  } else {
    game_screen_.DrawText(game_screen_.GetWidth() / 2 - 50,
                       game_screen_.GetHeight() / 2 - 10, "GAME OVER", 30,
                        255, 255, 255);
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
    if (event.GetMouseAction() == graphics::MouseAction::kDragged ||
      event.GetMouseAction() == graphics::MouseAction::kPressed) {
    std::unique_ptr<PlayerProjectile> bullet_ptr;
    bullet_ptr = std::unique_ptr<PlayerProjectile>(new PlayerProjectile (my_player_.GetX() + 13, my_player_.GetY()));
    player_projectiles_.push_back(std::move(bullet_ptr));
    }
  }
}

void Game::LaunchProjectiles() {
  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i]->LaunchProjectile() != nullptr && my_player_.GetIsActive()
        && projectile_counter_ % 50 == 0) {
      opponent_projectiles_.push_back(std::move(opponents_[i]->LaunchProjectile()));
    }
  }
  projectile_counter_++;
}

void Game::OnAnimationStep() {
  bool enemys_present_ = false;
  for (int i = 0; i < opponents_.size(); i++) {
    if (opponents_[i]->GetIsActive()) {
      enemys_present_ = true;
    }
  }
  if (!enemys_present_) {
    CreateOpponents();
  }
  MoveGameElements();
  LaunchProjectiles();
  FilterIntersections();
  RemoveInactive();
  UpdateScreen();
  game_screen_.Flush();
}
