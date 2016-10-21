#pragma once

#include <Sakura/include/GUIInterface.h>

class Player;

class GameGUI : 
	public Sakura::GUIInterface
{
public:
	GameGUI();
	~GameGUI();

	void initSpecialData(Player* player);

	void update(Sakura::InputManager& inputManager) override;
private:
	void IDraw(float fps) override;

	void initComponents() override;

	virtual void destroyComponents() override;

	Player* m_player = nullptr;

	Sakura::GUIButton m_backButton;
};

