#include "Soldier.h"
#include "Owner.h"
#include <iostream>

Soldier::Soldier(Owner* p_owner, Type p_type, sf::Vector2i p_gridPosition, const sf::Texture& texture)
    : owner(p_owner), type(p_type), gridPosition(p_gridPosition), sprite(texture)
{
    resetMovementPoints();
    
    //sprite.setTexture(texture);
    sf::Vector2u texSize = texture.getSize();
    sprite.setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
}

//Anim Funcs
void Soldier::startMoveAnimation(const sf::Vector2f& startPixel, const sf::Vector2f& targetPixel)
{
    isAnimating = true;
    startPixelPos = startPixel;
    targetPixelPos = targetPixel;
    pixelPosition = startPixel;
    animationTime = 0.f;
}

void Soldier::update(float dt)
{
    if (isAnimating)
    {
        // Animasyon süresini artýr.
        animationTime += dt;

        // Animasyonun yüzde kaçýnýn tamamlandýðýný hesapla (0.0 ile 1.0 arasý).
        float progress = animationTime / animationDuration;

        // Eðer animasyon bittiyse...
        if (progress >= 1.0f)
        {
            progress = 1.0f; // Ýlerlemenin 1.0'ý geçmediðinden emin ol.
            isAnimating = false; // Animasyonu bitir.
        }
        pixelPosition = startPixelPos + (targetPixelPos - startPixelPos) * progress;
    }
}

void Soldier::draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY, sf::Shader* shader)
{
    float scaleRatio = 0.02f;
    float outlineFactor = 1.2f;
    sf::Vector2u texSize = sprite.getTexture().getSize();

    if (shader)
    {
        // 1. Outline Rengi (Takým Rengi)
        shader->setUniform("u_borderColor", sf::Glsl::Vec4(owner->color));

        // 2. Resim Boyutu (Shader'ýn piksel hesabýný yapabilmesi için þart)
        shader->setUniform("u_textureSize", sf::Vector2f(float(texSize.x), float(texSize.y)));

        shader->setUniform("texture", sf::Shader::CurrentTexture);

        window.draw(sprite, shader);
    }

    // 2. KATMAN: Asýl Görsel (Ön Plan)
    sprite.setScale({ scaleRatio, scaleRatio });
    sprite.setColor(sf::Color::White); // Beyaz = Orijinal resim renkleri
    window.draw(sprite);

    if (isAnimating)
    {
        sprite.setPosition(pixelPosition);
    }
    else
    {
        float pixelX = offsetX + (gridPosition.x * tileSize) + (tileSize / 2);
        float pixelY = offsetY + (gridPosition.y * tileSize) + (tileSize / 2);
        sprite.setPosition(sf::Vector2f(pixelX, pixelY));
    }

    window.draw(sprite);
}

bool Soldier::moveTo(const sf::Vector2i& newPosition)
{
    // 1. Gidilecek mesafeyi hesapla.
    int distance = abs(newPosition.x - gridPosition.x) + abs(newPosition.y - gridPosition.y);

    // 2. Hareket puanýnýn yetip yetmediðini kontrol et.
    if (currentMovementPoints >= distance)
    {
        // 3. Puaný düþür.
        setCurrentPoints(-distance);

        // 4. Pozisyonu güncelle.
        gridPosition = newPosition;

        std::cout << "Soldier moved to (" << gridPosition.x << ", " << gridPosition.y
            << "). Points remaining: " << currentMovementPoints << "\n";
        return true; // Hareket baþarýlý.
    }

    std::cout << "Move failed! Not enough points.\n";
    return false; // Hareket baþarýsýz.
}

void Soldier::resetMovementPoints()
{
    currentMovementPoints = movementPoints; // Deðeri baþlangýç deðerine döndür.
}

void Soldier::setOwner(Owner* newOwner)
{
    if (owner == newOwner)
    {
        return;
    }
    owner = newOwner;
}

void Soldier::toggleSelection()
{
	std::cout << "Toggling selection for soldier at (" << gridPosition.x << ", " << gridPosition.y << ")\n";
    isSelected = !isSelected;
}

bool Soldier::getSelectState()
{
    if (isSelected)
        return true;
    
    return false;
}
