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

Player::Player() : Entity(EntityType::PLAYER), canChangeDirection(true), isCornerCollision(false)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(160, 832);
	return true;
}

bool Player::Start() {
	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;
	right.LoadAnimations(parameters.child("animations").child("right"));
	left.LoadAnimations(parameters.child("animations").child("left"));
	up.LoadAnimations(parameters.child("animations").child("up"));
	down.LoadAnimations(parameters.child("animations").child("down"));

	// L08 TODO 5: Add physics to the player - initialize physics body
	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW / 2, texH / 2, bodyType::DYNAMIC);
	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetFixedRotation(true);
	pbody->body->SetGravityScale(0.0f);
	pbody->body->SetBullet(true);
	//initialize audio effect
	pickCoinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	velocity.Set(0, 0);

	return true;
}

void Player::GravityChange(b2Vec2& velocity)
{
	auto input = Engine::GetInstance().input.get();

	if (!canChangeDirection) {
	
		return;
	
	}

	// Permitir cambio de dirección después de cada frame
	

	if (canChangeDirection) {
		if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && lastDirection != UP && currentCollider != ColliderType::UP) {
			velocity = b2Vec2(0, GRAVITY_Y);
			currentAnimation = &up;
			lastDirection = UP;
			canChangeDirection = false;
		}
		else if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && lastDirection != DOWN && currentCollider != ColliderType::DOWN) {
			velocity = b2Vec2(0, -GRAVITY_Y);
			currentAnimation = &down;
			lastDirection = DOWN;
			canChangeDirection = false;
		}
		else if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && lastDirection != LEFT && currentCollider != ColliderType::LEFT) {
			velocity = b2Vec2(GRAVITY_Y, 0);
			currentAnimation = &left;
			lastDirection = LEFT;
			canChangeDirection = false;
		}
		else if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && lastDirection != RIGHT && currentCollider != ColliderType::RIGHT) {
			velocity = b2Vec2(-GRAVITY_Y, 0);
			currentAnimation = &right;
			lastDirection = RIGHT;
			canChangeDirection = false;
		}
	}
}

bool Player::Update(float dt)
{
	// L08 TODO 5: Add physics to the player - updated player position using physics
	//b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	GravityChange(velocity);

	//// Move left
	//if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
	//	velocity.x = -0.2 * dt;
	//}

	//// Move right
	//if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
	//	velocity.x = 0.2 * dt;
	//}
	
	////Jump
	//if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {
	//	// Apply an initial upward force
	//	pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
	//	isJumping = true;
	//}

	// If the player is jumpling, we don't want to apply gravity, we use the current velocity produced by the jump
	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		//reset the jump flag when touching the ground
		canExecuteGrav = true;
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
	case ColliderType::TRIGGER:
		LOG("Collision TRIGGER");
		break;
	case ColliderType::UP:
		LOG("Collision UP");
		canChangeDirection = true;
		currentCollider = ColliderType::UP;
		break;
	case ColliderType::DOWN:
		LOG("Collision DOWN");
		canChangeDirection = true;
		currentCollider = ColliderType::DOWN;
		break;
	case ColliderType::LEFT:
		LOG("Collision LEFT");
		canChangeDirection = true;
		currentCollider = ColliderType::LEFT;
		break;
	case ColliderType::RIGHT:
		LOG("Collision RIGHT");
		canChangeDirection = true;
		currentCollider = ColliderType::RIGHT;
		break;
	case ColliderType::ENEMY:
		LOG("Collided an enemy");
		isDead = true;
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
	static int collisionCount = 0;
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("End Collision PLATFORM");
		
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		velocity.Set(0,0);
		canChangeDirection = true;
		lastDirection = NONE;
		break;
	
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	case ColliderType::UP:
		LOG("End Collision UP");
		break;
	case ColliderType::DOWN:
		LOG("End Collision DOWN");
		break;
	case ColliderType::LEFT:
		LOG("End Collision LEFT");
		break;
	case ColliderType::RIGHT:
		LOG("End Collision RIGHT");
		break;
	default:
		break;
	}
}