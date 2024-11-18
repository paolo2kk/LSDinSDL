#include "Murcielago.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"

// Constructor
Murcielago::Murcielago(Player* target) : Entity(EntityType::ENEMYBFS), player(target), pathIndex(0) {
    this->health = 50; // Configuración específica del murciélago
    this->speed = 10; // Velocidad específica del murciélago
    this->currentPath = {};
}

// Destructor
Murcielago::~Murcielago() {

    delete pathfinding;
}

bool Murcielago::Awake()
{
    return true;
}

bool Murcielago::Start()
{
	// Inicializar texturas
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/murcielago.png");
	texW = 64;
	texH = 64;

	// Inicializar animaciones
	idle.LoadAnimations(parameters.child("animations").child("left"));
	currentAnimation = &idle;

	// Crear cuerpo físico
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMYBFS;
	pbody->body->SetGravityScale(0);

	// Inicializar pathfinding
	pathfinding = new Pathfinding();
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);

	return true;
}

// Actualización del murciélago en cada frame
bool Murcielago::Update(float deltaTime) {
    if (!player) {
        std::cerr << "Murcielago: No hay jugador asignado para perseguir." << std::endl;
        return;
    }

    // Recalcular el camino si es necesario (por ejemplo, si llegamos al final o el jugador se movió)
    if (currentPath.empty() || pathIndex >= currentPath.size()) {
        Vector2D startPos = this->GetPosition();
        Vector2D targetPos = player->GetPosition();
        pathIndex = 0; // Reinicia el índice del camino
    }

    // Mover al siguiente nodo en el camino
    if (!currentPath.empty() && pathIndex < currentPath.size()) {
        Vector2D nextNode = currentPath[pathIndex];
        Vector2D direction = (nextNode - this->GetPosition());

        // Actualizar posición del murciélago
        Vector2D newPosition = this->GetPosition() + direction * speed * deltaTime;
        this->SetPosition(newPosition);

        // Revisar si hemos alcanzado el nodo actual
        if ((nextNode - newPosition).getY() < 1.0f) { // Tolerancia de 1 unidad
            pathIndex++;
        }
    }
}

void Murcielago::SetPosition(Vector2D pos)
{
    pos.setX(pos.getX() + texW / 2);
    pos.setY(pos.getY() + texH / 2);
    b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
    pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Murcielago::GetPosition()
{
    b2Vec2 bodyPos = pbody->body->GetTransform().p;
    Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
    return pos;
}
