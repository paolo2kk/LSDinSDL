#include "Trigger.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Trigger::Trigger() : Entity(EntityType::ITEM)
{
	name = "item";
}

Trigger::~Trigger() {}

bool Trigger::Awake() {
	return true;
}

bool Trigger::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Trigger.png");
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW, texH / 2, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::TRIGGER;

	return true;
}

bool Trigger::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());

	return true;
}

bool Trigger::CleanUp()
{
	return true;
}