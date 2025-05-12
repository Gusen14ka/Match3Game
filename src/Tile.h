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

    // ������������� ��� ������
    void setType(TileType type);

    // ���������� ��� ������
    TileType getType() const;

    // ������������ ������
    void draw(sf::RenderWindow& window);
    
    // ���������� ��������� ���� �� TileType
    static TileType randomColor(std::mt19937& rng);

    // ������������� ������ � ���������� �������
    void setPxPosition(const sf::Vector2f& pixelPos);

    // ���������� ������� ������ � ����
    sf::Vector2f getPxPosition() const;

    // ������������� ������� ������ (����������� {1 , 1})
    void setScale(float x, float y);

    // ������������� ��������� �������� ������ �� �����
    void setLogicalPos(const Pos& pos);

    // ���������� ���������� ������� ������ �� �����
    Pos getLogicalPos() const;

    void setBonus(char c);
    void clearBonus();

    /*
    // ������������� ��������� �������� �������� �������� ������
    void startMoveAnimation();

    // ������������� ��������� ����-�������� ������
    void triggerFlash(float duration = 0.3f, float amplitude = 0.2f);

    // ��������� ��������� ������ - ������ ��������
    void update(float dt);

    // ��������� ����-�������� ������
    float flashTimer_ = 0.f; // ������� ��� ����-��������
    float flashDuration_ = 0.3f; // ����� ������������ ����-��������
    float flashAmplitudeScale_ = 0.2f; // ������� ����-���������

    // ���������� �������� ����-��������� isAnimating 
    bool getAnimationStatus();
    */
  private:
    Pos pos_; // ���������� ������� �� �����
    TileType type_;
    float size_;
    sf::RectangleShape shape_;

    bool hasBonus_ = false;
    char bonusChar_;

    void setPositionByPosAndSize(const Pos& pos, float tileSize);

    /*
    // ����-��������� �������� �������� ������
    bool isAnimating = false;

    float flash_timer_ = 0.f;

    void setTargetPos(const Pos& pos, float tileSize);
    */
};
