#pragma once
#include <vector>

class GameState
{
protected:
	GameState();
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
};

// ENGINE STATES
class PauseState :public GameState {
public:
	PauseState();
	void Enter();
	void Update();
	void Render();
	void Exit();
};

class RunningState :public GameState {
public:
	RunningState();
	void Enter();
	void Update();
	void Render();
	void Exit();
	void Resume();
};

class TitleState :public GameState {
public:
	TitleState();
	void Enter();
	void Update();
	void Render();
	void Exit();
};

// ANIM STATES
class PlayerIdleState :public GameState {
public:
	PlayerIdleState();
	void Enter();
	void Update();
	void Render();
	void Exit();
};

class PlayerRunState :public GameState {
public:
	PlayerRunState();
	void Enter();
	void Update();
	void Render();
	void Exit();
};

class PlayerJumpState :public GameState {
public:
	PlayerJumpState();
	void Enter();
	void Update();
	void Render();
	void Exit();
};

// FSM
class FSM {
private:
	std::vector<GameState*> states_;
public:
	FSM();
	~FSM();

	void Update();
	void Render();
	void ChangeState(GameState* state_ptr); //normal state change
	void PushState(GameState* state_ptr); //RunningState to PauseState
	void PopState(); //PauseState to RunningState
	void Clean();
	std::vector<GameState*>& GetStates();
};