#include "decor.h"
#include <iostream>

Decor::Decor(const std::vector<vector>& puncte) {
    shape.setPointCount(puncte.size());
    for (size_t i = 0; i < puncte.size(); ++i) {
        shape.setPoint(i, toSfmlVector(puncte[i]));
    }
}

// Algoritm Ray-Casting pentru Point in Polygon
bool isPointInPolygon(const sf::Vector2f& point, const sf::ConvexShape& polygon) {
    bool inside = false;
    size_t count = polygon.getPointCount();
    for (size_t i = 0, j = count - 1; i < count; j = i++) {
        sf::Vector2f pi = polygon.getPoint(i);
        sf::Vector2f pj = polygon.getPoint(j);
        
        // Transformam punctele in coordonate globale daca shape-ul are transformari
        // Aici shape e la (0,0) fara rotatie, deci punctele sunt deja globale.
        
        if (((pi.y > point.y) != (pj.y > point.y)) &&
            (point.x < (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)) {
            inside = !inside;
        }
    }
    return inside;
}

void Decor::verificaSiAplica(car& masina) const {
    vector carPos = masina.getPozitie();
    sf::Vector2f center = toSfmlVector(carPos);

    // Optimizare: Verificam intai AABB
    if (!shape.getGlobalBounds().contains(center)) return;

    // Verificare detaliata
    if (isPointInPolygon(center, shape)) {
        // Aplica efect de incetinire (frictiune marita)
        // 0.95 este frictiune normala (exemplu), 0.90 sau 0.85 este incetinire.
        // Daca in car::acceleratie se aplica frictiune 0.95 cand nu acceleram,
        // aici aplicam extra.
        masina.aplicaFrictiune(0.90f); 
    }
}
