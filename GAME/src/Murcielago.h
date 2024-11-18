#ifndef MURCIELAGO_H
#define MURCIELAGO_H

#include "EnemyInClass.h"
#include "Pathfinding.h"
#include "Player.h"

class Murcielago : public Enemy {
private:
    Player* player; // Referencia al jugador
    Pathfinding pathfinding; // Sistema de pathfinding
    std::vector<Vec2> currentPath; // Camino actual hacia el jugador
    int pathIndex; // Índice del nodo actual en el camino

public:
    Murcielago(Player* target);
    ~Murcielago();

    void Update(float deltaTime) override; // Actualización en cada frame
    void Draw() override; // Dibuja el murciélago en pantalla
};

#endif // MURCIELAGO_H
