#pragma once
#include <exception>
#include <string>

class GameException : public std::exception 
{
protected:
    std::string mesaj;
public:
    explicit GameException(const std::string& msg) : mesaj(msg) {}


    const char* what() const noexcept override 
    {
        return mesaj.c_str();
    }
};


class FileLoadException : public GameException 
{
public:
    explicit FileLoadException(const std::string& numeFisier) 
        : GameException("Eroare critica: Nu s-a putut incarca fisierul [" + numeFisier + "]") {}
};


class InvalidStatsException : public GameException 
{
public:
    explicit InvalidStatsException(const std::string& entitate, const std::string& detaliu)
        : GameException("Valori invalide pentru " + entitate + ": " + detaliu) {}
};


class GameLogicException : public GameException 
{
public:
    explicit GameLogicException(const std::string& motiv)
        : GameException("Eroare logica joc: " + motiv) {}
};