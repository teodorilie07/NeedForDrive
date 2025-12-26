#pragma once
#include <string>
#include "vector.h"
#include <SFML/Graphics.hpp>

class car
{
private:
    std::string nume;
    vector pozitie;
    vector viteza;
    double fuel;
    const int consum;
    int damage;
    const int damageMax;
    float performanta;
    float unghi;
    float latime;
    float lungime;
    sf::Sprite m_sprite;

    float immunityTimer;

public:
    car(const std::string& nume, const vector& poz, int fuelInit, int consumMediu, sf::Texture& texturaMasinii);
    car(const car& other);
    car& operator=(const car& other);
    ~car();

    void uptState(float dtime);
    void acceleratie(float factor);
    void roteste(float grade);
    void brake();
    void aplicaFrictiune(float factor);

    [[nodiscard]] float getUnghi() const;
    [[nodiscard]] const std::string& getNume() const;
    [[nodiscard]] const vector& getPozitie() const;
    [[nodiscard]] const vector& getViteza() const;
    float getLatime() const;
    float getLungime() const;
    [[nodiscard]] bool eliminata() const;

    void setPozitie(const vector& pos);
    void setViteza(const vector& vel);
    void setFuel(double valoare);

    void aplicaDamage(int valoare);
    void adaugaCombustibil(int cantitate);
    void penalizareMotor(float penalizare);
    void onCollision();

    void activeazaImunitate(float secunde);
    bool esteImuna() const;

    sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderWindow& window);
    friend std::ostream& operator<<(std::ostream& os, const car& car);
};