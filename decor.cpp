#include "decor.h"
#include <iostream>

Decor::Decor(const std::vector<vector>& puncte) {
    shape.setPointCount(puncte.size());
    for (size_t i = 0; i < puncte.size(); ++i) {
        shape.setPoint(i, toSfmlVector(puncte[i]));
    }
}

 
bool isPointInPolygon(const sf::Vector2f& point, const sf::ConvexShape& polygon) {
    bool inside = false;
    size_t count = polygon.getPointCount();
    for (size_t i = 0, j = count - 1; i < count; j = i++) {
        sf::Vector2f pi = polygon.getPoint(i);
        sf::Vector2f pj = polygon.getPoint(j);
        
         
         
        
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

     
    if (!shape.getGlobalBounds().contains(center)) return;

     
    if (isPointInPolygon(center, shape)) {
         
         
         
         
        masina.aplicaFrictiune(0.90f); 
    }
}
