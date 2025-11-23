#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <queue>
#include <map>

// GameManager sýnýfýný önceden bildiriyoruz (içeriðini bilmesine gerek yok)
class GameManager;

class Pathfinder
{
public:
    // Ana fonksiyonumuz: Baþlangýçtan Hedefe giden yolu liste olarak döndürür.
    // Eðer yol yoksa boþ liste döner.
    static std::vector<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& target, GameManager& gameManager);

private:
    // Yardýmcý fonksiyon: Koordinat harita sýnýrlarý içinde mi?
    static bool isValid(const sf::Vector2i& pos, int width, int height);
};