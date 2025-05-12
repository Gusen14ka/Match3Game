#pragma once
#include "Animation.h"
#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp>


class AnimationManager {
private:
	std::vector<std::unique_ptr<Animation>> animations_;
public:
	// Добавляет новую анимацию
	void add(std::unique_ptr<Animation> animation);

	// Обновляет состояние всех анимаций, удаляет завершённые
	void updateAll(float dt);

	// Проверяет есть ли активные анимации
	bool empty() const;
};