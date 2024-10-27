#include "Door.h"
#include "Engine.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Physics.h"

Door::Door() : Entity(EntityType::ITEM)
{
	name = "item";
}

Door::~Door() {}

bool Door::Awake() {
	return true;
}

bool Door::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Door.png");

	// L08 TODO 4: Add a physics to an item - initialize the physics body
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX() + texW / 2, (int)position.getY() + texH / 2, texW / 2, texW / 2, bodyType::STATIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::DOOR;

	return true;
}

bool Door::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	auto input = Engine::GetInstance().input.get();

	if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		imDed = true;
	}
	if (!imDed)
	{
		b2Transform pbodyPos = pbody->body->GetTransform();
		position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
		position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	}
	
	
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());

	return true;
}

bool Door::CleanUp()
{
	return true;
}