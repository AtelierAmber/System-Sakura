#include "Player.h"
#include <Sakura/include/TexParams.h>

#define player_texture_path "Assets/Textures/Player/Fox.png"
#define player_texture_dimensions 8,2 // Width , Height

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(const glm::vec2& position, const glm::vec2& collisionDims, const glm::vec2& drawDims, Sakura::ResourceManager& resourceManager, Sakura::ColorRGBA8 color) {
	Sakura::GLTexture texture = resourceManager.getTexture(player_texture_path, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_color = color;
	m_drawDims = drawDims;
	m_texture.init(texture, glm::ivec2(player_texture_dimensions));
	m_collisionBox.initialize(position.x, position.y, collisionDims.x, collisionDims.y, false);
}

void Player::draw(Sakura::SpriteBatch& spriteBatch, float deltaTime){

	int tileIndex;
	int animLength = 8;

	float animSpeed = 0.2f;

	//calculate animation
	if (!(m_collisionBox.velocity.x == 0 && m_collisionBox.velocity.y == 0)){
		//Running
		tileIndex = 8;
		animLength = 5;
		animSpeed = 0.15f;
		if (m_moveState != PlayerMoveState::WALKING){
			m_moveState = PlayerMoveState::WALKING;
			m_animTime = 0.0f;
		}
	}
	else {
		//Standing
		if (m_attack){
			/* Attacking */
			tileIndex = 0;
			animLength = 4;
			animSpeed = 0.25f;
			if (m_moveState != PlayerMoveState::ATTACKING){
				m_moveState = PlayerMoveState::ATTACKING;
				m_animTime = 0.0f;
			}
		}
		else{
			/* Idle */
			tileIndex = 0;
			animLength = 8;
			animSpeed = 0.1f;
			if (m_moveState != PlayerMoveState::STANDING){
				m_moveState = PlayerMoveState::STANDING;
				m_animTime = 0.0f;
			}
		}
	}
	//Increment animation time
	if (deltaTime > animSpeed){
		m_animTime += animSpeed * deltaTime;
	}
	else m_animTime += animSpeed;

	/* Check for static animation ending */
	if (m_animTime > animLength){
		m_attack = false;
	}

	/* Apply animation */
	tileIndex = tileIndex + (int)m_animTime % animLength;

	glm::vec4 uvRect = m_texture.getUVs(tileIndex);

	/* Flip the animation if the direction is left */
	if (m_direction & dir_left){
		uvRect = m_texture.getUVs(tileIndex);

		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	glm::vec4 destRect(m_collisionBox.x1 - m_drawnPosOffset.x, m_collisionBox.y2 - m_drawnPosOffset.y, m_drawDims);
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.5f, m_color);
}

void Player::drawDebug(Sakura::DebugRenderer& debugRenderer){
	debugRenderer.drawBox(glm::vec4(m_collisionBox.x1, m_collisionBox.y2, m_collisionBox.width, m_collisionBox.height), Sakura::ColorRGBA8(255, 255, 255, 255), 0.0f);
	debugRenderer.drawBox(glm::vec4(m_collisionBox.x1 - m_drawnPosOffset.x, m_collisionBox.y2 - m_drawnPosOffset.y, m_drawDims), Sakura::ColorRGBA8(255, 255, 255, 255), 0.0f);
}

void Player::update(Sakura::InputManager& inputManager, float deltaTime){
	m_collisionBox.velocity = glm::vec2(0.0f);
	if (inputManager.isKeyDown(KeyID::KeyMod::LSHIFT) || inputManager.isKeyDown(KeyID::KeyMod::RSHIFT)){
		m_playerMoveSpeed = 0.5f * deltaTime;
	}
	else m_playerMoveSpeed = 1.5f * deltaTime;
	if (inputManager.isKeyDown(KeyID::d) && ! m_attack){
		if (m_direction & dir_left){
			m_direction ^= dir_left;
		}
		m_direction |= dir_right;
		if (m_direction & dir_up || m_direction & dir_down){
			m_collisionBox.velocity.x = m_playerMoveSpeed / 2.0f;
			m_collisionBox.velocity.y /= 2.0f;
		}
		else m_collisionBox.velocity.x = m_playerMoveSpeed;
	}
	else if (inputManager.isKeyDown(KeyID::a) && !m_attack){
		if (m_direction & dir_right){
			m_direction ^= dir_right;
		}
		m_direction |= dir_left;
		if (m_direction & dir_up || m_direction & dir_down){
			m_collisionBox.velocity.x = -(m_playerMoveSpeed / 2.0f);
			m_collisionBox.velocity.y /= 2.0f;
		}
		else m_collisionBox.velocity.x = -m_playerMoveSpeed;
	}
	if (inputManager.isKeyDown(KeyID::w) && !m_attack){
		if (m_direction & dir_down){
			m_direction ^= dir_down;
		}
		m_direction |= dir_up;
		m_collisionBox.velocity.y = m_playerMoveSpeed;
	}
	else if (inputManager.isKeyDown(KeyID::s) && !m_attack){
		if (m_direction & dir_up){
			m_direction ^= dir_up;
		}
		m_direction |= dir_down;
		m_collisionBox.velocity.y = -m_playerMoveSpeed;
	}
	else{
		if (m_direction & dir_up){
			m_direction ^= dir_up;
		}
		if (m_direction & dir_down){
			m_direction ^= dir_down;
		}
	}
	if (inputManager.isKeyPressed(KeyID::SPACE)){
		if (!m_attack){
			m_attack = true;
		}
	}
	// TODO : DELTA TIME INTERVAL STEP
	m_collisionBox.update(deltaTime);
}
