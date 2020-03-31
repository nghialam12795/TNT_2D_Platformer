#include "Player.h"
#include "Game.h"

Player::Player()
{
	setTextureId("player");
	TheTextureManager::Instance()->load("../Assets/textures/adventurer-v1.5-Sheet.png", getTextureId(), TheGame::Instance()->getRenderer());
	setSrc(0, 0, 50, 37);
	SetWorldRect(0, 0, 50 * 3, 37 * 3);
	setDst(0, 0, 50 * 3, 37 * 3);
	setHitBox(0, 0, 45, 85);
	setHitBoxOffsetX(-4);
	setHitBoxOffsetY(10);
	//SetHitBoxVisibility(true); //set this in GameObject.h to toggle collide boxes for ALL objects
	SetCollidable(true);
	SetGrounded(false);
	setAccelerationX(0);
	setAccelerationY(0);
	setVelocityX(0);
	setVelocityY(0);
	setGravity(Globals::sGravity);
	setMaxAccelerationX(2.0);
	setMaxAccelerationY(2.0);
	setMaxVelocityX(6.0);
	setMaxVelocityY(getGravity());
	setDrag(0.9);
	setMoveDirection(1);
	setType(GameObjectType::PLAYER);
	setAnimState(AnimState::IDLE);

	// ANIM INIT
	InitAnimList();
	GetAnimList()[IDLE]->SetAnimId(IDLE);
	GetAnimList()[IDLE]->SetStartRow(0);
	GetAnimList()[IDLE]->SetStartCol(0);
	GetAnimList()[IDLE]->SetNumFrames(4);
	GetAnimList()[IDLE]->SetAnimSpeed(0.12f);
	GetAnimList()[IDLE]->SetMaxSheetRow(16); //same for all states since there's only 1 sheet
	GetAnimList()[IDLE]->SetMaxSheetCol(7); //same for all states since there's only 1 sheet

	GetAnimList()[RUN]->SetAnimId(RUN);
	GetAnimList()[RUN]->SetStartRow(1);
	GetAnimList()[RUN]->SetStartCol(1);
	GetAnimList()[RUN]->SetNumFrames(6);
	GetAnimList()[RUN]->SetAnimSpeed(0.25f);
	GetAnimList()[RUN]->SetMaxSheetRow(16); //same for all states since there's only 1 sheet
	GetAnimList()[RUN]->SetMaxSheetCol(7); //same for all states since there's only 1 sheet
}

Player::~Player()
{
}

void Player::MoveX() 
{
	setAccelerationX(getAccelerationX() + 0.1 * getMoveDirection());
}

void Player::update()
{
	// UPDATE POSITION
	setAccelerationX(std::min(std::max(getAccelerationX(), -getMaxAccelerationX()), getMaxAccelerationX()));
	setVelocityX((getVelocityX() + getAccelerationX()) * getDrag());
	setVelocityX(std::min(std::max(getVelocityX(), -getMaxVelocityX()), getMaxVelocityX()));
	SetWorldXAndHitBox(GetWorldRect()->x + (int)getVelocityX());
	//std::cout << "getVelocityX = " << getVelocityX() << std::endl;

	setVelocityY(getVelocityY() + getAccelerationY() + (getGravity() / 3));
	setVelocityY(std::min(std::max(getVelocityY(), -getMaxVelocityY() * 3), getMaxVelocityY()));
	SetWorldYAndHitBox(GetWorldRect()->y + (int)getVelocityY());
	//std::cout << "getVelocityY = " << getVelocityY() << std::endl;
}

void Player::draw()
{
	Animate();
	switch (getMoveDirection()) {
	case 1:
		TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(), getTextureId(), getSrc(), getDst(), 0.0, 0, SDL_FLIP_NONE);
		break;
	case -1:
		TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(), getTextureId(), getSrc(), getDst(), 0.0, 0, SDL_FLIP_HORIZONTAL);
		break;
	}
}

void Player::clean()
{
	
}
