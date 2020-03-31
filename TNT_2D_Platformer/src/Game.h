#pragma once
#ifndef __Game__
#define __Game__

// Core Libraries
#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

// Game Managers
#include "TextureManager.h"
#include "CollisionManager.h"
#include "FSM.h"

// Game Objects
#include "UI.h"
#include "Level.h"
#include "Camera.h"
#include "Player.h"

class Game
{
private:
	Game();
	~Game();

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	FSM* fsm_;

	static Game* s_pInstance;
	Uint32 m_frames;
	bool m_bRunning;
	const Uint8* key_states_ = nullptr;

	// GameObjects
	UI* title_screen_ptr_;
	UI* pause_screen_ptr_;
	Level* level_ptr_;
	Camera* camera_ptr_;
	Player* player_ptr_;
	FSM* player_anim_fsm_;
	
	// Movement offset
	bool can_reset_anim_frame = true;
	bool is_jump_key_pressable_ = true;

	// REDUNDANT
	glm::vec2 m_mousePosition;

public:
	static Game* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}

	// simply set the running variable to true
	void init() { m_bRunning = true; }

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	// public functions
	bool isKeyDown(SDL_Scancode keyboard_code);
	void handleEvents();
	void update();
	void render();
	void clean();

	// a function to access the private running variable
	bool running() { return m_bRunning; }

	void createGameObjects();
	void CheckCollision();
	void UpdateGameObjects();
	void ResetCurrAnimFrame();
	void RenderGameObjects();

	// getters
	Uint32 getFrames() const;
	SDL_Renderer* getRenderer();
	FSM& GetFSM();
	FSM& GetPlayerFSM();
	Player& GetPlayer();
	Player* GetPlayerPtr();
	Camera& GetCamera();
	bool IsJumpKeyPressable();
	UI& GetTitleScreen();
	UI& GetPauseScreen();

	// setters
	void setFrames(Uint32 frames);
	void SetJumpKeyPressable(bool toggle);

	// REDUNDANT
	glm::vec2 getMousePosition();
	glm::vec2 getTargetPosition();
};

typedef Game TheGame;

#endif /* defined (__Game__) */

