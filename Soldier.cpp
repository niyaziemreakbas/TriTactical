#include "Soldier.h"
#include "Owner.h"
#include <iostream>

Soldier::Soldier(Owner* p_owner, Type p_type, sf::Vector2i p_gridPosition, const sf::Texture& bodyTex, const sf::Texture& outlineTex)
    : owner(p_owner), type(p_type), gridPosition(p_gridPosition), bodySprite(bodyTex), outlineSprite(outlineTex)
{
    resetMovementPoints();

    // Body (Gövde) Merkezi
    sf::Vector2u bodySize = bodyTex.getSize();
    bodySprite.setOrigin(sf::Vector2f(bodySize.x / 2.f, bodySize.y / 2.f));

    // Outline (Çerçeve) Merkezi
    sf::Vector2u outSize = outlineTex.getSize();
    outlineSprite.setOrigin(sf::Vector2f(outSize.x / 2.f, outSize.y / 2.f));
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

void Soldier::draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY)
{
    float scaleRatio = 0.02f; // Görselin boyutuna göre bunu ayarlayabilirsin

    // 1. Pozisyonu Hesapla (Animasyonlu veya Sabit)
    sf::Vector2f drawPos;
    if (isAnimating)
    {
        drawPos = pixelPosition;
    }
    else
    {
        float pixelX = offsetX + (gridPosition.x * tileSize) + (tileSize / 2);
        float pixelY = offsetY + (gridPosition.y * tileSize) + (tileSize / 2);
        drawPos = sf::Vector2f(pixelX, pixelY);
    }

    outlineSprite.setPosition(drawPos);
    outlineSprite.setScale(sf::Vector2f(scaleRatio, scaleRatio));
    if (owner) {
        outlineSprite.setColor(owner->color);
    }
    window.draw(outlineSprite);

    bodySprite.setPosition(drawPos);
    bodySprite.setScale(sf::Vector2f(scaleRatio, scaleRatio));
    window.draw(bodySprite);
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
