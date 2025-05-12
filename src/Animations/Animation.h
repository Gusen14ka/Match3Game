#pragma once
#include <algorithm>
#include <functional>

// Базовый класс для всех анимаций.
// Хранит elapsed/duration и вызывает чисто виртуальный onUpdate(t).
class Animation {
protected:
	float duration_; // длительность (в секундах)
	float elapsed_ = 0.f; // сколько прошло (в секундах)
	bool finished_ = false; // поле-состояние - анимация завершена
	std::function<void()> onCompleteCallback_; // колбек для плавности

public:

	Animation(float duration) : duration_(duration) {}
	virtual ~Animation() = default;

	// Обновляет анимацию на шаг dt.
	// Вызывает конкретный onUpdate и onComplete
	void update(float dt);

	// t ∈ [0,1] — нормализованное время анимации
	virtual void onUpdate(float t) = 0;

	// Явно приводит к состонию завершенной анимации
	virtual void onComplete();

	// Проверяет, завершена ли анимация
	bool isFinished() const;

	// Устанавливает колбек для плавности
	void setOnComplete(std::function<void()>);
};