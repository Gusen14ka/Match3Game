#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Pos.h"


enum class TileType { Red, Green, Blue, Yellow, Purple, EMPTY };

class Tile {

  public:
    Tile() = default;
    Tile(const Tile&) = default;
    Tile(const Pos& pos, TileType type, float tileSize);

    // Устанавливает тип плитки
    void setType(TileType type);

    // Возвращает тип плитки
    TileType getType() const;

    // Отрисовывает плитку
    void draw(sf::RenderWindow& window);
    
    // Возвращает рандомный цвет из TileType
    static TileType randomColor(std::mt19937& rng);

    // Устанавливает плитку в переданную позицию
    void setPxPosition(const sf::Vector2f& pixelPos);

    // Возвращает позицию плитки в окне
    sf::Vector2f getPxPosition() const;

    // Устанавливает масштаб плитки (изначальный {1 , 1})
    void setScale(float x, float y);

    // Устанавливает логическу позициию плитки на доске
    void setLogicalPos(const Pos& pos);

    // Возвращает логическую позицию плитки на доске
    Pos getLogicalPos() const;

    void setBonus(char c);
    void clearBonus();

    /*
    // Устанавливает состояние процесса анимации движения плитки
    void startMoveAnimation();

    // Устанавливает состояние флеш-анимации плитки
    void triggerFlash(float duration = 0.3f, float amplitude = 0.2f);

    // Обновляет состояние плитки - создаёт анимации
    void update(float dt);

    // Параметры флеш-анимации плитки
    float flashTimer_ = 0.f; // сколько ещё флеш-анимации
    float flashDuration_ = 0.3f; // общая длительность флеш-анимации
    float flashAmplitudeScale_ = 0.2f; // масштаб флеш-пульсации

    // Возвращает значение поля-состояния isAnimating 
    bool getAnimationStatus();
    */
  private:
    Pos pos_; // Логическая позиция на доске
    TileType type_;
    float size_;
    sf::RectangleShape shape_;

    bool hasBonus_ = false;
    char bonusChar_;

    void setPositionByPosAndSize(const Pos& pos, float tileSize);

    /*
    // Поле-состояние процесса анимации плитки
    bool isAnimating = false;

    float flash_timer_ = 0.f;

    void setTargetPos(const Pos& pos, float tileSize);
    */
};
