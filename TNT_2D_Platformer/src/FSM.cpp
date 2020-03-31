#include "FSM.h"
#include "Game.h"

GameState::GameState() 
{
}

void GameState::Render()
{
	SDL_RenderPresent(TheGame::Instance()->getRenderer());
}

void GameState::Resume()
{
}

// PAUSE 
PauseState::PauseState()
{
}

void PauseState::Enter()
{
	std::cout << "Entering Pause..." << std::endl;
}

void PauseState::Update()
{
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_R)) {
		TheGame::Instance()->GetFSM().PopState();
	}
}

void PauseState::Render()
{
	std::cout << "Rendering Pause..." << std::endl;
	TheGame::Instance()->GetFSM().GetStates().front()->Render();
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 255, 255, 255, 50);
	SDL_Rect temp_overlay = { 0,0,Globals::sWindowWidth,Globals::sWindowHeight };
	SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &temp_overlay);
	TheGame::Instance()->GetPauseScreen().draw();
	GameState::Render();
}

void PauseState::Exit()
{
	std::cout << "Exiting Pause..." << std::endl;
}

// RUNNING
RunningState::RunningState()
{
	TheGame::Instance()->createGameObjects();
}

void RunningState::Enter()
{
	std::cout << "Entering Game..." << std::endl;
}

void RunningState::Update()
{
	TheGame::Instance()->UpdateGameObjects();
	
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_P)) {
		TheGame::Instance()->GetFSM().PushState(new PauseState());
	}
	else if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_X)) {
		TheGame::Instance()->GetFSM().ChangeState(new TitleState());
	}
}

void RunningState::Render()
{
	//std::cout << "Rendering Game..." << std::endl;

	SDL_RenderClear(TheGame::Instance()->getRenderer()); // clear the renderer to the draw colour
	TheGame::Instance()->RenderGameObjects();

	if (dynamic_cast<RunningState*>(TheGame::Instance()->GetFSM().GetStates().back())) {
		GameState::Render();
	}
}

void RunningState::Exit()
{
	std::cout << "Exiting Game..." << std::endl;
}

void RunningState::Resume()
{
}

// TITLE
TitleState::TitleState()
{
}

void TitleState::Enter()
{
	std::cout << "Entering Title..." << std::endl;
}

void TitleState::Update()
{
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_N)) {
		TheGame::Instance()->GetFSM().ChangeState(new RunningState());
	}
}

void TitleState::Render()
{
	std::cout << "Rendering Title..." << std::endl;
	SDL_RenderClear(TheGame::Instance()->getRenderer()); // clear the renderer to the draw colour
	TheGame::Instance()->GetTitleScreen().draw();
	GameState::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting Title..." << std::endl;
}

FSM::FSM()
{
}

FSM::~FSM()
{
}

void FSM::Update()
{
	if (!states_.empty()) {
		states_.back()->Update();
	}
}

void FSM::Render()
{
	if (!states_.empty()) {
		states_.back()->Render();
	}
}

void FSM::ChangeState(GameState* state_ptr)
{
	if (!states_.empty()) {
		states_.back()->Exit(); //invoke Exit() of curr state
		delete states_.back(); //deallocate curr state
		states_.back() = nullptr;
		states_.pop_back();
	}
	PushState(state_ptr);

	//state_ptr->Enter();
	//states_.push_back(state_ptr);

	//states_.push_back(state_ptr);
	//states_.back()->Enter();
}

void FSM::PushState(GameState* state_ptr)
{
	state_ptr->Enter();
	states_.push_back(state_ptr);
}

void FSM::PopState()
{
	if (!states_.empty()) {
		states_.back()->Exit(); //invoke Exit() of curr state
		delete states_.back(); //deallocate curr state
		states_.back() = nullptr;
		states_.pop_back();
	}
	states_.back()->Resume();
}

void FSM::Clean()
{
	while (!states_.empty()) {
		states_.back()->Exit(); //invoke Exit() of curr state
		delete states_.back(); //deallocate curr state
		states_.back() = nullptr;
		states_.pop_back();
	}
}

std::vector<GameState*>& FSM::GetStates()
{
	// TODO: insert return statement here
	return states_;
}

PlayerIdleState::PlayerIdleState()
{
}

void PlayerIdleState::Enter()
{
	TheGame::Instance()->GetPlayer().setCurrFrame(0);
	TheGame::Instance()->GetPlayer().setAnimState(AnimState::IDLE);
}

void PlayerIdleState::Update()
{
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_A) || TheGame::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) {
		if (TheGame::Instance()->GetPlayer().IsGrounded()) {
			TheGame::Instance()->GetPlayerFSM().ChangeState(new PlayerRunState());
		}
	}
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_D) || TheGame::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) {
		if (TheGame::Instance()->GetPlayer().IsGrounded()) {
			TheGame::Instance()->GetPlayerFSM().ChangeState(new PlayerRunState());
		}
	}
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_SPACE) && TheGame::Instance()->IsJumpKeyPressable() && TheGame::Instance()->GetPlayer().IsGrounded()) {
		TheGame::Instance()->SetJumpKeyPressable(false);
		TheGame::Instance()->GetPlayer().setAccelerationY(-Globals::sJumpForce);
		TheGame::Instance()->GetPlayer().SetGrounded(false);
		TheGame::Instance()->GetPlayer().setAnimState(AnimState::JUMP);
	}
	TheGame::Instance()->GetPlayer().setAccelerationY(0);
}

void PlayerIdleState::Render()
{
	TheGame::Instance()->GetCamera().draw(TheGame::Instance()->GetPlayerPtr());
}

void PlayerIdleState::Exit()
{
}

PlayerRunState::PlayerRunState()
{
}

void PlayerRunState::Enter()
{
	TheGame::Instance()->GetPlayer().setCurrFrame(0);
	TheGame::Instance()->GetPlayer().setAnimState(AnimState::RUN);
}

void PlayerRunState::Update()
{
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_A) || TheGame::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) {
		TheGame::Instance()->GetPlayer().setMoveDirection(-1);
	}
	if (TheGame::Instance()->isKeyDown(SDL_SCANCODE_D) || TheGame::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) {
		TheGame::Instance()->GetPlayer().setMoveDirection(1);
	}
	TheGame::Instance()->GetPlayer().MoveX();
	
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
				TheGame::Instance()->GetPlayer().setAccelerationX(0);
				TheGame::Instance()->GetPlayerFSM().ChangeState(new PlayerIdleState());
			}
			break;
		}
	}
}

void PlayerRunState::Render()
{
	TheGame::Instance()->GetCamera().draw(TheGame::Instance()->GetPlayerPtr());
}

void PlayerRunState::Exit()
{
}
