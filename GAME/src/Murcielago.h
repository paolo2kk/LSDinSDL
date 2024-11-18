#pragma once
#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "EnemyInClass.h"
#include "Player.h"

class Murcielago : public Entity {
private:
    Player* player; // Referencia al jugador
    Pathfinding* pathfinding; // Sistema de pathfinding
    std::vector<Vector2D> currentPath; // Camino actual hacia el jugador
    int pathIndex; // Índice del nodo actual en el camino
	int health; // Vida del murciélago
	int speed; // Velocidad del murciélago
	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	PhysBody* pbody;

public:
    Murcielago(Player* target);
    ~Murcielago();

	bool Awake(); // Inicialización de variables
	bool Start(); // Inicialización de variables
    bool Update(float deltaTime); // Actualización en cada frame
    void Draw(); // Dibuja el murciélago en pantalla
    void SetPosition(Vector2D pos);
	Vector2D GetPosition();
};


