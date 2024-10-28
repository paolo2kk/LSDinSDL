#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Player::Player() : Entity(EntityType::PLAYER), canChangeDirection(true), isCornerCollision(false), godMode(false)
{
	name = "Player";

}

Player::~Player() {

}

bool Player::Awake() {
	position = Vector2D(160, 832);
	return true;
}

bool Player::Start() {
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;
	right.LoadAnimations(parameters.child("animations").child("right"));
	left.LoadAnimations(parameters.child("animations").child("left"));
	up.LoadAnimations(parameters.child("animations").child("up"));
	down.LoadAnimations(parameters.child("animations").child("down"));

	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW / 2, texH / 2, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetFixedRotation(true);
	pbody->body->SetGravityScale(0.0f);
	pbody->body->SetBullet(true);

	pickCoinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	velocity.Set(0, 0);

	return true;
}

void Player::GravityChange(b2Vec2& velocity)
{
	auto input = Engine::GetInstance().input.get();

	// Cambia la dirección sin restricciones complejas
	if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity = b2Vec2(0, GRAVITY_Y);
		currentAnimation = &up;
		lastDirection = UP;
	}
	else if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity = b2Vec2(0, -GRAVITY_Y);
		currentAnimation = &down;
		lastDirection = DOWN;
	}
	else if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity = b2Vec2(GRAVITY_Y, 0);
		currentAnimation = &left;
		lastDirection = LEFT;
	}
	else if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity = b2Vec2(-GRAVITY_Y, 0);
		currentAnimation = &right;
		lastDirection = RIGHT;
	}
}

bool Player::Update(float dt)
{
	GravityChange(velocity);
	pbody->body->SetLinearVelocity(velocity);

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (godMode == false) {
			godMode = true;
		}
		else if (godMode == true) {
			godMode = false;
		}
	}
	if (godMode == true) {
		
		//dibujar texto más adelante
		
	}
	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

void Player::SetAnimation(Direction dir)
{
	switch (dir)
	{
	case UP:
		currentAnimation = &up;
		break;
	case DOWN:
		currentAnimation = &down;
		break;
	case LEFT:
		currentAnimation = &left;
		break;
	case RIGHT:
		currentAnimation = &right;
		break;
	default:
		break;
	}
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		canChangeDirection = true;
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		Engine::GetInstance().audio.get()->PlayFx(pickCoinFxId);
		Engine::GetInstance().physics.get()->DeletePhysBody(physB);
		canChangeDirection = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		canChangeDirection = true;
		break;
	case ColliderType::UP:
	case ColliderType::DOWN:
	case ColliderType::LEFT:
	case ColliderType::RIGHT:
		LOG("Collision with wall");
		canChangeDirection = true;
		currentCollider = physB->ctype;
		break;
		

	case ColliderType::ENEMY:
		LOG("Collided an enemy");
		if (godMode == false) {
			isDead = true;
		}
		else if (godMode == true)  isDead = false;

		break;
	case ColliderType::BOX:
		velocity.Set(0, 0);
		canChangeDirection = true;
		lastDirection = NONE;
		break;
	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
	case ColliderType::ITEM:
	case ColliderType::UNKNOWN:
	case ColliderType::UP:
	case ColliderType::DOWN:
	case ColliderType::LEFT:
	case ColliderType::RIGHT:
		LOG("End Collision");
		break;
	default:
		break;
	}
}
