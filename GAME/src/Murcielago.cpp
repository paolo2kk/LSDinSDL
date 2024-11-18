#include "Murcielago.h"
#include <iostream>

// Constructor
Murcielago::Murcielago(Player* target) : player(target), pathIndex(0) {
    this->health = 50; // Configuración específica del murciélago
    this->speed = 100; // Velocidad específica del murciélago
    this->currentPath = {};
}

// Destructor
Murcielago::~Murcielago() {
    // Limpieza si es necesaria
}

// Actualización del murciélago en cada frame
void Murcielago::Update(float deltaTime) {
    if (!player) {
        std::cerr << "Murcielago: No hay jugador asignado para perseguir." << std::endl;
        return;
    }

    // Recalcular el camino si es necesario (por ejemplo, si llegamos al final o el jugador se movió)
    if (currentPath.empty() || pathIndex >= currentPath.size()) {
        Vec2 startPos = this->GetPosition();
        Vec2 targetPos = player->GetPosition();
        currentPath = pathfinding.CalculatePath(startPos, targetPos);
        pathIndex = 0; // Reinicia el índice del camino
    }

    // Mover al siguiente nodo en el camino
    if (!currentPath.empty() && pathIndex < currentPath.size()) {
        Vec2 nextNode = currentPath[pathIndex];
        Vec2 direction = (nextNode - this->GetPosition()).Normalize();

        // Actualizar posición del murciélago
        Vec2 newPosition = this->GetPosition() + direction * speed * deltaTime;
        this->SetPosition(newPosition);

        // Revisar si hemos alcanzado el nodo actual
        if ((nextNode - newPosition).Length() < 1.0f) { // Tolerancia de 1 unidad
            pathIndex++;
        }
    }
}

// Dibujar el murciélago
void Murcielago::Draw() {
    // Aquí iría el código específico para dibujar el murciélago, por ejemplo:
    std::cout << "Dibujando Murcielago en posición: " << this->GetPosition().ToString() << std::endl;
}
