#include "Soldier.h"
#include "Owner.h"
#include <iostream>

Soldier::Soldier(Owner* p_owner, Type p_type, sf::Vector2i p_gridPosition)
    : owner(p_owner), type(p_type), gridPosition(p_gridPosition)
{
    resetMovementPoints();

    // Configure the shape based on the soldier type.
    shape.setRadius(20.f); 
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius())); 
    shape.setFillColor(p_owner->color); 
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.f);

	// Configure selection highlight
    selectionHighlight.setRadius(20.f);
    selectionHighlight.setFillColor(sf::Color::Transparent);
    selectionHighlight.setOutlineColor(sf::Color::Green);
    selectionHighlight.setOutlineThickness(3.f);
    selectionHighlight.setOrigin(
        sf::Vector2f(selectionHighlight.getRadius(), selectionHighlight.getRadius())
    );

    switch (type)
    {
    case Type::Triangle:
        shape.setPointCount(3);
		selectionHighlight.setPointCount(3);
        break;
    case Type::Circle:
        shape.setPointCount(100);
		selectionHighlight.setPointCount(100);
        break;
    case Type::Square:
        shape.setPointCount(4);
		selectionHighlight.setPointCount(4);
        break;
    }
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

        // Lineer Ýnterpolasyon (Lerp): Baþlangýç ve bitiþ noktasý arasýnda 'progress' yüzdesi kadar ilerle.
        // Bu, akýcý hareketin formülüdür.
        pixelPosition = startPixelPos + (targetPixelPos - startPixelPos) * progress;
    }
}

void Soldier::draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY)
{
    // EÐER ANÝMASYON OYNATILIYORSA, 'm_pixelPosition' kullanýlýr.
    if (isAnimating)
    {
        shape.setPosition(pixelPosition);
    }
    // EÐER ANÝMASYON YOKSA (normal durma hali), pozisyonu her zamanki gibi HESAPLA.
    else
    {
        float pixelX = offsetX + (gridPosition.x * tileSize) + (tileSize / 2);
        float pixelY = offsetY + (gridPosition.y * tileSize) + (tileSize / 2);
        shape.setPosition(sf::Vector2f(pixelX, pixelY));
    }

    // Çizim ve seçim halkasý mantýðý ayný.
    window.draw(shape);

    if (isSelected)
    {
        // Seçim halkasýnýn pozisyonu da ana þeklin pozisyonuyla ayný olmalý.
        selectionHighlight.setPosition(shape.getPosition());
        window.draw(selectionHighlight);
    }
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

    // Hareket puaný yetersiz.
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
    shape.setFillColor(newOwner->color);
}

void Soldier::toggleSelection()
{
	std::cout << "Toggling selection for soldier at (" << gridPosition.x << ", " << gridPosition.y << ")\n";
    isSelected = !isSelected;
}

//void Soldier::draw(sf::RenderWindow& window, float tileSize, float offsetX, float offsetY)
//{
//    // Calculate the pixel position on the screen based on grid position.
//    float pixelX = offsetX + (gridPosition.x * tileSize) + (tileSize / 2);
//    float pixelY = offsetY + (gridPosition.y * tileSize) + (tileSize / 2);
//
//    shape.setPosition(sf::Vector2f(pixelX, pixelY));
//    window.draw(shape);
//
//    if (isSelected)
//    {
//        selectionHighlight.setPosition(sf::Vector2f(pixelX, pixelY));
//        window.draw(selectionHighlight);
//    }
//}

bool Soldier::getSelectState()
{
    if (isSelected)
        return true;
    
    return false;
}
