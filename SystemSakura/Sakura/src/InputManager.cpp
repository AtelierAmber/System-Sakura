#include "InputManager.h"

namespace Sakura {

	InputManager::InputManager() : m_mouseCoords(0.0f){}
	InputManager::~InputManager(){}

	void InputManager::update(){
		for (auto& it : m_keyMap){
			if (m_previousKeyMap[it.first].pressedTimer <= 0){
				m_previousKeyMap[it.first].pressed = it.second;
			}
			else --m_previousKeyMap[it.first].pressedTimer;
		}
	}

	void InputManager::pressKey(KeyCode keyID) {
		// Here we are treating m_keyMap as an associative array.
		// if keyID doesn't already exist in m_keyMap, it will get added
		m_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(KeyCode keyID) {
		m_keyMap[keyID] = false;
		m_previousKeyMap[keyID] = true;
	}

	void InputManager::setMouseCoords(float x, float y) {
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(KeyCode keyID) {
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end()) {
			// Found the key
			return it->second;
		} else {
			// Didn't find the key
			return false;
		}
	}

	bool InputManager::isKeyPressed(KeyCode keyID){
		if (isKeyDown(keyID) && !wasKeyDown(keyID)){
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyDown(KeyCode keyID){
		// We dont want to use the associative array approach here
		// because we don't want to create a key if it doesnt exist.
		// So we do it manually
		auto it = m_previousKeyMap.find(keyID);
		if (it != m_previousKeyMap.end()) {
			// Found the key
			return it->second.pressed;
		}
		else {
			// Didn't find the key
			return false;
		}
	}

}