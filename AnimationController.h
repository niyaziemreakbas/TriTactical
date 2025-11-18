#pragma once
#include <SFML/Graphics.hpp>

class AnimationController
{
public:
    // Kurucu, canland�raca�� nesnenin adresini al�r.
    explicit AnimationController(sf::Transformable* target);

    // Animasyonu her karede ilerletir.
    void update(float dt);

    // Belirli bir hedefe do�ru hareket animasyonu ba�lat�r.
    void startMove(const sf::Vector2f& targetPosition, float duration);

    // Animasyonun devam edip etmedi�ini bildirir.
    bool IsAnimating() const { return m_isAnimating; }

private:
    sf::Transformable* m_target = nullptr; // Canland�rd���m�z nesne (shape, sprite, etc.)

    bool m_isAnimating = false;
    sf::Vector2f m_startPos;
    sf::Vector2f m_targetPos;
    float m_duration = 0.f;
    float m_progress = 0.f; // Animasyonun ilerlemesi (0.0 to 1.0)
};