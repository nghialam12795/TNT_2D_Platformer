#pragma once

#include "AnimState.h"

class AnimSprite
{
private:
	AnimState anim_id_;
	int start_row_; 
	int start_col_;
	int num_frames_;
	float anim_speed_;
	int max_sheet_row_;
	int max_sheet_col_;
public:
	AnimSprite();
	AnimSprite(AnimState anim_id,
		int start_row,
		int start_col,
		int num_frames,
		float anim_speed,
		int max_sheet_row,
		int max_sheet_col);
	~AnimSprite();

	AnimState GetAnimId();
	int GetStartRow(); 
	int GetStartCol();
	int GetNumFrames();
	float GetAnimSpeed();
	int GetMaxSheetRow();
	int GetMaxSheetCol();

	void SetAnimId(AnimState value);
	void SetStartRow(int value);
	void SetStartCol(int value);
	void SetNumFrames(int value);
	void SetAnimSpeed(float value);
	void SetMaxSheetRow(int value);
	void SetMaxSheetCol(int value);
};
