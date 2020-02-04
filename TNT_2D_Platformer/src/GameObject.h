#pragma once
#ifndef __GameObject__
#define __GameObject__

#include <glm\vec2.hpp>
#include <string>
#include <vector>
#include <SDL.h>

#include "GameObjectType.h"
#include "SteeringState.h"

class GameObject {
private:
	// RENDERING
	SDL_Rect src_, dst_;
	std::string texture_id_;

	// SOUND
	std::vector<std::string> sfx_id_list_;

	// PHYSICS
	int velocity_x_;
	int velocity_y_;

	// COLLISION
	SDL_Rect hit_box_;
	int hit_box_manual_offset_x_ = 0;
	int hit_box_manual_offset_y_ = 0;
	bool is_hit_box_visible = true;
	bool m_isColliding;
	GameObjectType m_type;
	SteeringState m_state;

	// TRANSFORM
	int custom_pivot_x_;
	int custom_pivot_y_;

	// LEGACY
	// transform variables
	glm::vec2 m_position;
	glm::vec2 m_rotation;
	glm::vec2 m_scale;

	// movement variables
	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;

	// size variables
	int m_width;
	int m_height;

	// animation variables
	int m_currentRow;
	int m_currentFrame;
	int m_numFrames;

public:
	GameObject();
	~GameObject();

	// Update the object
	virtual void update() = 0;

	// Draw the object
	virtual void draw() = 0;

	// remove anything that needs to be deleted
	virtual void clean() = 0;

	// getters for common variables
	SDL_Rect* getSrc();
	SDL_Rect* getDst();
	int getDstX();
	int getDstRightmostX();
	int getDstY();
	int getDstLowermostY();
	int getDstW();
	int getDstH();
	int getDstCenterX();
	int getDstCenterY();
	SDL_Rect* getHitBox();
	int getHitBoxX();
	int getHitBoxRightmostX();
	int getHitBoxY();
	int getHitBoxLowermostY();
	int getHitBoxW();
	int getHitBoxH();
	int getHitBoxCenterX();
	int getHitBoxCenterY();
	int getHitBoxOffsetX();
	int getHitBoxOffsetY();
	bool isHitBoxVisible();
	bool getIsColliding();
	std::string getTextureId();
	int getCustomPivotX();
	int getCustomPivotY();
	int getVelocityX();
	int getVelocityY();
	GameObjectType getType();
	SteeringState getSteeringState();
	glm::vec2 getPosition();
	glm::vec2 getRotation();
	glm::vec2 getScale();
	glm::vec2 getVelocityVect();
	glm::vec2 getAcceleration();
	int getWidth();
	int getHeight();

	// setters for common variables
	void setSrc(SDL_Rect src_ptr);
	void setSrc(int x, int y, int w, int h);
	void setDst(int x, int y, int w, int h);
	void setDstX(int x);
	void setDstY(int y);
	void setDstXAndHitBox(int x);
	void setDstYAndHitBox(int y);
	void setHitBox(int x, int y, int w, int h);
	void setHitBoxX(int coord);
	void setHitBoxY(int coord);	
	void setHitBoxOffsetX(int coord);
	void setHitBoxOffsetY(int coord);
	void setHitBoxVisibility(bool toggle);
	void setIsColliding(bool collision);
	void setTextureId(std::string id); //cleanup is done by manager
	void addSfxId(std::string id); //cleanup is done by manager
	void setCustomPivotX(int coord);
	void setCustomPivotY(int coord);
	void setVelocityX(int velocity);
	void setVelocityY(int velocity);
	void setType(GameObjectType newType);
	void setSteeringState(SteeringState newState);
	void setAcceleration(glm::vec2 newAcceleration);
	void setPosition(glm::vec2 newPosition);
	void setWidth(int newWidth);
	void setHeight(int newHeight);
	void setVelocityVect(glm::vec2 newVelocity);
};

#endif