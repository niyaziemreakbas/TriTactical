#include "AnimationController.h"

AnimationController::AnimationController(sf::Transformable* target)
    : m_target(target)
{
}

void AnimationController::startMove(const sf::Vector2f& targetPosition, float duration)
{
    if (m_target == nullptr) return;

    m_isAnimating = true;
    m_startPos = m_target->getPosition(); // Mevcut pozisyondan ba�la.
    m_targetPos = targetPosition;
    m_duration = duration;
    m_progress = 0.f;
}

void AnimationController::update(float dt)
{
    if (!m_isAnimating || m_target == nullptr) return;

    // �lerlemeyi g�ncelle.
    m_progress += dt / m_duration;

    // Animasyon bitti mi?
    if (m_progress >= 1.0f)
    {
        m_progress = 1.0f;
        m_isAnimating = false;
    }

    // Lineer �nterpolasyon (Lerp) ile yeni pozisyonu hesapla.
    sf::Vector2f newPosition = m_startPos + (m_targetPos - m_startPos) * m_progress;

    // Hedef nesnenin pozisyonunu do�rudan g�ncelle.
    m_target->setPosition(newPosition);
}