#pragma once
#include "Animation.h"
#include "../Pos.h"
#include "../Board.h"
#include <SFML/Graphics.hpp>

class BonusAnimation : public Animation {
public:
    // window – для отрисовки,
    // cellCenter – лямбда из Game: Pos→pixel,
    // font – загруженный sf::Font,
    // c – буква, duration – время эффекта
    BonusAnimation(
        sf::RenderWindow& window,
        std::function<sf::Vector2f(const Pos&)> cellCenter,
        const sf::Font& font,
        Pos pos, char c, float duration);

    void onUpdate(float t) override;
    void onComplete() override;

private:
    sf::RenderWindow& window_;
    std::function<sf::Vector2f(const Pos&)> cellCenter_;
    sf::Text text_;
};
