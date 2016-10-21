#pragma once
#include <Sakura/include/InputManager.h>
#include <Sakura/include/SpriteBatch.h>
#include <Sakura/include/DebugRenderer.h>
#include <Sakura/include/ResourceManager.h>
#include <Sakura/include/Bounds.h>

#define dir_left 0x1
#define dir_down 0x2
#define dir_right 0x4
#define dir_up 0x8

enum class PlayerMoveState{
	STANDING,
	WALKING,
	ATTACKING
};

class Player{
public:
	Player();
	~Player();

	void init(const glm::vec2& position, const glm::vec2& collisionDims, const glm::vec2& drawDims, Sakura::ResourceManager& resourceManager, Sakura::ColorRGBA8 color);

	void draw(Sakura::SpriteBatch& spriteBatch, float deltaTime);
	void drawDebug(Sakura::DebugRenderer& debugRenderer);

	void update(Sakura::InputManager& inputManager, float deltaTime);

	Sakura::Rect* collisionRectangle(){ return &m_collisionBox; }
	glm::vec2 getPosition(){ return glm::vec2(m_collisionBox.x1, m_collisionBox.y2); }

	int Health() const { return health; }
	void SetHealth(int val) { health = val; }
private:
	float m_animTime = 0.0f;
	unsigned int m_direction = 0x4;

	float m_playerMoveSpeed = 1.5f;
	bool m_attack = false;

	int health = 3;

	const glm::vec2 m_drawnPosOffset = glm::vec2(10.0f, 0.0f);

	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
	Sakura::Rect m_collisionBox;

	glm::vec2 m_drawDims;
	Sakura::ColorRGBA8 m_color;
	Sakura::TileSheet m_texture;
};

