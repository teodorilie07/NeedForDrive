#include "car.h"
#include <iostream>
#include <cmath>
#include "vector.h"
#include <SFML/Graphics.hpp>
#include "erori.h"


car::car(const std::string& nume, const vector& poz, int combInit, int consum, sf::Texture& texturaMasinii):
    nume(nume),
    pozitie(poz),
    viteza(0, 0),
    fuel(combInit),
    consum(consum),
    damage(0),
    damageMax(5),
    performanta(1.0),
    unghi(0),
    latime(40.f),
    lungime(70.f),
    m_sprite(texturaMasinii),
    immunityTimer(0.0f)
{
    if (consum < 0) {
        throw InvalidStatsException(nume, "Consumul nu poate fi negativ!");
    }
    if (damageMax <= 0) {
        throw InvalidStatsException(nume, "DamageMax trebuie sa fie pozitiv!");
    }

    std::cout << "Init construct: masina condusa de " << nume << " a intrat pe circuit.\n";
    auto bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.size / 2.f);
    if (bounds.size.x > 0 && bounds.size.y > 0) {
        m_sprite.setScale({ latime / bounds.size.x, lungime / bounds.size.y });
    }
}


car::car(const car& other):
    nume(other.nume),
    pozitie(other.pozitie),
    viteza(other.viteza),
    fuel(other.fuel),
    consum(other.consum),
    damage(other.damage),
    damageMax(other.damageMax),
    performanta(other.performanta),
    unghi(other.unghi),
    latime(other.latime),
    lungime(other.lungime),
    m_sprite(other.m_sprite),    
    immunityTimer(other.immunityTimer)
{
    std::cout << "Cpy construct: o copie a "<< nume << " a fost creata.\n";
}

 
car& car::operator=(const car& other)
{
    if (this != &other)
    {
        std::cout << "Assignment operator: Masina lui " << this->nume << " a devenit o copie a masinii lui " << other.nume <<"\n";
        this->nume = other.nume;
        this->pozitie = other.pozitie;
        this->viteza = other.viteza;
        this->fuel = other.fuel;
        this->damage = other.damage;
        this->performanta = other.performanta;
        this->unghi = other.unghi;
        this->latime = other.latime;
        this->lungime = other.lungime;
        this->m_sprite = other.m_sprite;
        this->immunityTimer = other.immunityTimer;

         
         
    }
    return *this;
}

car::~car()
{
    std::cout << "Destructor: masina lui " << nume << " a fost scoasa din cursa.\n";
}

void car::uptState(float dTime)
{
    pozitie = vector(
            pozitie.getx() + viteza.getx() * dTime,
            pozitie.gety() + viteza.gety() * dTime
        );

    // Invisible wall logic (no penalty)
    float x = pozitie.getx();
    float y = pozitie.gety();
    float vx = viteza.getx();
    float vy = viteza.gety();
    
    if (x < 0.0f) { x = 0.0f; vx = 0.0f; }
    else if (x > 1024.0f) { x = 1024.0f; vx = 0.0f; }

    if (y < 0.0f) { y = 0.0f; vy = 0.0f; }
    else if (y > 640.0f) { y = 640.0f; vy = 0.0f; }

    pozitie = vector(x, y);
    viteza = vector(vx, vy);

     
    if (immunityTimer > 0.0f) {
        immunityTimer -= dTime;
        if (immunityTimer < 0.0f) immunityTimer = 0.0f;
    }
}

void car::acceleratie(float forta)
{
    if (fuel > 0)
    {
        float unghiRad = unghi * (3.14159f / 180.0f);
        vector directie(std::sin(unghiRad), -std::cos(unghiRad));

        float fortaAplicata = forta * performanta;

        viteza = vector(
            viteza.getx() + directie.getx() * fortaAplicata,
            viteza.gety() + directie.gety() * fortaAplicata
        );

        fuel = fuel - consum * 0.2;
        if (fuel < 0) fuel = 0;
    }
    else
    {
        aplicaFrictiune(0.95f);
    }
}

void car::brake()
{
    vector v = getViteza();
    setViteza(vector(v.getx() * 0.9f, v.gety() * 0.9f));
}

const std::string& car::getNume() const { return nume; }
const vector& car::getPozitie() const { return pozitie; }
const vector& car::getViteza() const { return viteza; }

void car::setPozitie(const vector& pos) { pozitie = pos; }
void car::setViteza(const vector& vel) { viteza = vel; }
void car::setFuel(double valoare) { this->fuel = valoare; }

std::ostream& operator<<(std::ostream& os, const car& car)
{
    os << "Car: " << car.nume << " | HP: " << (car.damageMax - car.damage);
    return os;
}

void car::aplicaDamage(int valoare)
{
    this->damage += valoare;
    if(this->damage > this->damageMax) this->damage = this->damageMax;
    std::cout << "[INFO] " << this->nume << " a lovit ceva! Damage: " << this->damage << "/" << this->damageMax << "\n";
}

bool car::eliminata() const { return this->damage >= this->damageMax; }

void car::adaugaCombustibil(int cantitate)
{
    fuel += cantitate;
    if (fuel > 100) fuel = 100;
}

void car::penalizareMotor(float penalizare)
{
    performanta -= penalizare;
    if (performanta < 0.2f) performanta = 0.2f;
}

void car::roteste(float grade) { this->unghi += grade; }
float car::getUnghi() const { return this->unghi; }

void car::aplicaFrictiune(float factor)
{
    viteza = vector(viteza.getx() * factor, viteza.gety() * factor);
}

float car::getLatime() const { return latime; }
float car::getLungime() const { return lungime; }

 
void car::activeazaImunitate(float secunde) {
    immunityTimer = secunde;
}

bool car::esteImuna() const {
    return immunityTimer > 0.0f;
}

sf::FloatRect car::getGlobalBounds() const {
    return m_sprite.getGlobalBounds();
}

void car::onCollision()
{
    if (!esteImuna()) {
        aplicaDamage(1);
    }
}

void car::draw(sf::RenderWindow& window) {
    m_sprite.setPosition(toSfmlVector(this->pozitie));
    m_sprite.setRotation(sf::degrees(this->unghi));

    if (esteImuna()) {
        m_sprite.setColor(sf::Color(255, 255, 255, 128));
    } else {
        m_sprite.setColor(sf::Color(255, 255, 255, 255));
    }
    window.draw(m_sprite);
}
