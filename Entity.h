#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Entity {
private:
    string nombre;
    int vida;
    float posX, posY;

public:
    Entity(string n, int v) : nombre(n), vida(v), posX(0.0f), posY(0.0f) {}

    void modificarVida(int cantidad) { vida += cantidad; }
    void teletransportar(float x, float y) { posX = x; posY = y; }
    
    string obtenerEstado() const {
        stringstream ss;
        ss << "[Entidad: " << nombre << " | HP: " << vida << " | Pos: (" << posX << ", " << posY << ")]";
        return ss.str();
    }
};

#endif  