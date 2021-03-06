#include "Level.h"
#include "Game.h"

Level::Level()
{
	setType(GameObjectType::LEVEL);
	SetLevelTileWidth(64);
	SetLevelTileHeight(64);
	ui_pause_ptr_ = new UI("pause_text", "../Assets/textures/press P to pause.png", 0, 0, 321, 44, 800, 10, 321 / 2, 44 / 2);
	ui_quit_ptr_ = new UI("quit_text", "../Assets/textures/press X to quit.png", 0, 0, 296, 46, 800, 50, 296 / 2, 46 / 2);
}

Level::~Level()
{
}

void Level::update()
{
	int row_idx_start;
	int row_idx_end;
	int col_idx_start;
	int col_idx_end;

	row_idx_start = GetTileIndexFromPosY(cam_pos_y_);
	row_idx_end = row_idx_start + GetVisibleTilesNumOfRows();
	
	// TRY TO INCLUDE OUTSIDE TILES FOR OVERDRAWING
	if (row_idx_start > 0) {
		row_idx_start -= 1;
	}
	if (row_idx_end < GetLevelNumOfRows()-1) {
		row_idx_end += 1;
	}
	
	col_idx_start = GetTileIndexFromPosX(cam_pos_x_);
	col_idx_end = col_idx_start + GetVisibleTilesNumOfColumns();
	
	// TRY TO INCLUDE OUTSIDE TILES FOR OVERDRAWING
	if (col_idx_start > 0) {
		col_idx_start -= 1;
	}
	if (col_idx_end < GetVisibleTilesNumOfColumns()-1) {
		col_idx_end += 1;
	}

	// UPDATE SCREEN POS
	for (int row = 0; row < GetVisibleTilesNumOfRows()+2; row++) {
		int world_row = row + row_idx_start;
		for (int col = 0; col < GetVisibleTilesNumOfColumns()+2; col++) {
			int world_col = col + col_idx_start;
			if (world_row < GetLevelNumOfRows() && world_col < GetLevelNumOfColumns()) { //bound check
				auto world_tile = level_world_tile_list_[world_row][world_col];
				auto tile = visible_tile_list_[row][col];
				tile->setDstX(world_tile->GetWorldRect()->x - cam_pos_x_);
				tile->setDstY(world_tile->GetWorldRect()->y - cam_pos_y_);
				tile->SetTileTextureId(world_tile->GetTileTextureId());
			}
		}
	}
}

void Level::draw()
{
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), bkg_r_value_, bkg_g_value_, bkg_b_value_, bkg_a_value_);
	
	for (int row = 0; row < GetVisibleTilesNumOfRows()+2; row++) {
		for (int col = 0; col < GetVisibleTilesNumOfColumns()+2; col++) {	
			auto tile = visible_tile_list_[row][col];
			TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(),
				tile_texture_list_[tile->GetTileTextureId()]->getTextureId(),
				tile_texture_list_[tile->GetTileTextureId()]->getSrc(),
				visible_tile_list_[row][col]->getDst(), 0.0, 0, SDL_FLIP_NONE);
		}
	}

	ui_pause_ptr_->draw();
	ui_quit_ptr_->draw();
}

void Level::clean()
{
}


int Level::CheckWorldBounds(GameObject* obj_ptr)
{
	int result = -1;

	if (obj_ptr->getHitBoxX() < 0) {
		obj_ptr->setVelocityX(0.0); // Stop the player from moving horizontally.
		obj_ptr->SetHitBoxXAndWorld(0);
		result = LEFT;
	}
	else if (obj_ptr->getHitBoxRightmostX() > GetLevelMaxPosX()) {
		obj_ptr->setVelocityX(0.0); // Stop the player from moving horizontally.
		obj_ptr->SetHitBoxXAndWorld(GetLevelMaxPosX() - obj_ptr->getHitBoxW());
		result = RIGHT;
	}

	return result;
}

int Level::CheckLevelCollision(GameObject* obj_ptr)
{
	CheckWorldBounds(obj_ptr);
	
	int result = -1;

	int obj_col = GetTileIndexFromPosX(obj_ptr->getHitBoxX());
	int obj_row = GetTileIndexFromPosY(obj_ptr->getHitBoxY());

	for (int row = (obj_row < 0 ? 0 : obj_row); row < ((obj_row + 3) > GetLevelNumOfRows() ? GetLevelNumOfRows() : (obj_row + 3)); row++)
	{
		for (int col = (obj_col < 0 ? 0 : obj_col); col < ((obj_col + 2) > GetLevelNumOfColumns() ? GetLevelNumOfColumns() : (obj_col + 2)); col++)
		{
			auto tile = level_world_tile_list_[row][col];
			if (tile != nullptr && tile->IsCollidable() && SDL_HasIntersection(obj_ptr->getHitBox(), tile->GetWorldRect()))
			{
				if ((obj_ptr->getHitBoxLowermostY()) - obj_ptr->getVelocityY() <= tile->GetWorldRect()->y)
				{ // Collision from top.
					obj_ptr->SetGrounded(true);
					obj_ptr->setVelocityY(0.0); // Stop the player from moving vertically. We aren't modifying gravity.
					obj_ptr->SetHitBoxYAndWorld(tile->GetWorldRect()->y - obj_ptr->getHitBox()->h);
					if (obj_ptr->getAnimState() == FALL)
					{
						obj_ptr->setAnimState(IDLE);
					}
					result = DOWN;
				}
				else if (obj_ptr->getHitBox()->y - obj_ptr->getVelocityY() >= tile->GetWorldRect()->y + tile->GetWorldRect()->h)
				{ // Collision from bottom.
					obj_ptr->setVelocityY(0.0); // Stop the player from moving vertically. We aren't modifying gravity.
					obj_ptr->SetHitBoxYAndWorld(tile->GetWorldRect()->y + tile->GetWorldRect()->h);
					result = UP;
				}
				else if ((obj_ptr->getHitBoxRightmostX()) - obj_ptr->getVelocityX() <= tile->GetWorldRect()->x)
				{ // Collision from left.
					obj_ptr->setVelocityX(0.0); // Stop the player from moving horizontally.
					obj_ptr->SetHitBoxXAndWorld(tile->GetWorldRect()->x - obj_ptr->getHitBox()->w);
					result = RIGHT;
				}
				else if (obj_ptr->getHitBox()->x - obj_ptr->getVelocityX() >= tile->GetWorldRect()->x + tile->GetWorldRect()->w)
				{ // Collision from right.
					obj_ptr->setVelocityX(0.0); // Stop the player from moving horizontally.
					obj_ptr->SetHitBoxXAndWorld(tile->GetWorldRect()->x + tile->GetWorldRect()->w);
					result = LEFT;
				}
			}
		}
	}

	return result;
}

void Level::MapAllTileNeighbors() //set up, down, left, right tile neighbors for easy access
{
	// CONSTRUCTOR FOR NEIGHBORS
	for (int row = 0; row < GetLevelNumOfRows(); row++) {
		for (int col = 0; col < GetLevelNumOfColumns(); col++) {
			auto tile = level_world_tile_list_[row][col];
			//std::cout << "[" << row << "]" << "[" << col << "]" << std::endl;
			row > 0 ? tile->setUp(level_world_tile_list_[row - 1][col])   : tile->setUp(nullptr);
			row < (GetLevelNumOfRows() - 1)    ? tile->setDown(level_world_tile_list_[row + 1][col])  : tile->setDown(nullptr);
			col > 0 ? tile->setLeft(level_world_tile_list_[row][col - 1]) : tile->setLeft(nullptr);
			col < (GetLevelNumOfColumns() - 1) ? tile->setRight(level_world_tile_list_[row][col + 1]) : tile->setRight(nullptr);
		}
	}
}

void Level::BuildTileTextureDatabase()
{
	//Tileset uses src, tiles use dst
	tileset_texture_list_[CHURCH_TILESET_01] = new Tile();
	tileset_texture_list_[CHURCH_TILESET_01]->setTextureId("church_tileset_01");
	TheTextureManager::Instance()->load("../Assets/textures/church_tileset_01.png", tileset_texture_list_[CHURCH_TILESET_01]->getTextureId(), TheGame::Instance()->getRenderer());

	tile_texture_list_[CHURCH_BKG_00] = new Tile();
	tile_texture_list_[CHURCH_BKG_00]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BKG_00]->setSrc(16, 80, 15, 15);
	
	tile_texture_list_[CHURCH_BKG_01] = new Tile();
	tile_texture_list_[CHURCH_BKG_01]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BKG_01]->setSrc(128, 16, 15, 15);

	tile_texture_list_[CHURCH_CEILING_00] = new Tile();
	tile_texture_list_[CHURCH_CEILING_00]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_CEILING_00]->setSrc(48, 32, 31, 31);

	tile_texture_list_[CHURCH_GROUND_00] = new Tile();
	tile_texture_list_[CHURCH_GROUND_00]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_GROUND_00]->setSrc(0, 167, 47, 39);
	
	tile_texture_list_[CHURCH_GROUND_01] = new Tile();
	tile_texture_list_[CHURCH_GROUND_01]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_GROUND_01]->setSrc(64, 167, 47, 39);

	tile_texture_list_[CHURCH_GROUND_02] = new Tile();
	tile_texture_list_[CHURCH_GROUND_02]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_GROUND_02]->setSrc(128, 167, 47, 39);

	tile_texture_list_[CHURCH_BLOCK_00] = new Tile();
	tile_texture_list_[CHURCH_BLOCK_00]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BLOCK_00]->setSrc(16, 112, 31, 31);

	tile_texture_list_[CHURCH_BLOCK_01] = new Tile();
	tile_texture_list_[CHURCH_BLOCK_01]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BLOCK_01]->setSrc(64, 112, 31, 31);

	tile_texture_list_[CHURCH_BLOCK_02] = new Tile();
	tile_texture_list_[CHURCH_BLOCK_02]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BLOCK_02]->setSrc(288, 96, 31, 31);
}

void Level::MapAllTileTextureIdAndCollision() //determine which char is responsible for which texture_id
{
	for (int row = 0; row < GetLevelNumOfRows(); row++) {
		for (int col = 0; col < GetLevelNumOfColumns(); col++) {
			auto tile = level_world_tile_list_[row][col];
			switch (tile->GetTileChar())
			{
			case 'C':
				tile->SetTileTextureId(CHURCH_CEILING_00);
				tile->SetCollidable(true);
				break;
			case '=':
				tile->SetTileTextureId(CHURCH_GROUND_00);
				tile->SetCollidable(true);
				break;
			case 'G':
				tile->SetTileTextureId(CHURCH_GROUND_01);
				tile->SetCollidable(true);
				break;
			case '#':
				tile->SetTileTextureId(CHURCH_GROUND_02);
				tile->SetCollidable(true);
				break;
			case '@':
				tile->SetTileTextureId(CHURCH_BLOCK_00);
				tile->SetCollidable(true);
				break;
			case '&':
				tile->SetTileTextureId(CHURCH_BLOCK_01);
				tile->SetCollidable(true);
				break;
			case 'B':
				tile->SetTileTextureId(CHURCH_BLOCK_02);
				tile->SetCollidable(true);
				break;
			case '-':
				tile->SetTileTextureId(CHURCH_BKG_00);
				tile->SetCollidable(false);
				break;
			default:
				tile->SetTileTextureId(CHURCH_BKG_01);
				tile->SetCollidable(false);
				break;
			}
		}
	}
}

void Level::LoadLevel(LevelId level_id)
{
	// NUKE PREVIOUS LEVEL (IF ANY)
	std::string level_raw_str = "";
	if (!level_world_tile_list_.empty()) {
		level_world_tile_list_.clear();
		level_world_tile_list_.resize(0);
		level_world_tile_list_.shrink_to_fit();
	}
	if (!visible_tile_list_.empty()) {
		visible_tile_list_.clear();
		visible_tile_list_.resize(0);
		visible_tile_list_.shrink_to_fit();
	}

	// PROCESS RAW STRING
	switch (level_id) {
	case CHURCH:
		SetBkgColor(22, 22, 51, 255);

		level_raw_str += "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";
		level_raw_str += "BB................................B";
		level_raw_str += "BB................................B";
		level_raw_str += "BB................................B";
		level_raw_str += "BB................................B";
		level_raw_str += "BB.........E.................E....B";
		level_raw_str += "BB................................B";
		level_raw_str += "BB&..GGGGGGGGGGGGG......GGGGGGGGGG@";
		level_raw_str += "BB..G----&--------G.....@---------&";
		level_raw_str += "BB..@-----@-------G.....&---------@";
		level_raw_str += "BB@.CCCCCCCCCCCCCCC@@@..&CCCCCCCCCB";
		level_raw_str += "BB............B........&..........B";
		level_raw_str += "BB................................B";
		level_raw_str += "BB.@@...........@@&@&@............B";
		level_raw_str += "BB......E......@......B...........B";
		level_raw_str += "BBG...................B...........B";
		level_raw_str += "B--GGGGGGGGGG.........BGGG........B";
		level_raw_str += "B---&--------GGGGGGG..B--&........B";
		level_raw_str += "B-----&--BCCCCCCCCCC..CCCCCCCCCCCCB";
		level_raw_str += "BB-------&...........@B...........B";
		level_raw_str += "B--@---&-B........................B";
		level_raw_str += "B----@---@........................B";
		level_raw_str += "CCCCCCCCCC.........&&.............B";
		level_raw_str += "..........P....@@@@...............B";
		level_raw_str += ".............@....................B";
		level_raw_str += "GGGGGGGGGG@............E..........B";
		level_raw_str += "B---&-----B&.....................BB";
		level_raw_str += "B----@------GGGGGGGGGGGGGGGGGGGGG@@";

		SetLevelRawStr(level_raw_str);
		SetLevelNumOfRows(28);
		SetLevelNumOfColumns(level_raw_str.size() / GetLevelNumOfRows());
		break;

	default:
		SetBkgColor(22, 22, 51, 255);
		
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += "......................#########.B.B.B..................................................";
		level_raw_str += ".....................####..............................................................";
		level_raw_str += "...................###.........................................#.......................";
		level_raw_str += "...................###........................................####.....................";
		level_raw_str += "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.##########.###.....############################.B#";
		level_raw_str += "........................############.#...............###...............................";
		level_raw_str += "........................#............#............###..................................";
		level_raw_str += "........................#............#.........###.....................................";
		level_raw_str += "........................#.############......###........................................";
		level_raw_str += "........................#................###...........................................";
		level_raw_str += "........................#..............##..............................................";
		level_raw_str += "........................################...............................................";
		level_raw_str += ".......................................................................................";
		level_raw_str += ".......................................................................................";

		SetLevelRawStr(level_raw_str);
		SetLevelNumOfRows(23);
		SetLevelNumOfColumns(level_raw_str.size() / GetLevelNumOfRows());
		break;
	}

	std::vector<std::vector<char>> level_raw_str_list;
	level_raw_str_list.resize(GetLevelNumOfRows(), std::vector<char>(GetLevelNumOfColumns(), '.'));
	for (int row = 0; row < GetLevelNumOfRows(); row++) {
		for (int col = 0; col < GetLevelNumOfColumns(); col++) {
			//1D to 2D arr fomula: array2d[row][col] = array1d[(row * no_of_col) + col]; 
			level_raw_str_list[row][col] = level_raw_str[row * GetLevelNumOfColumns() + col];
		}
	}

	// BUILD LEVEL
	level_world_tile_list_.resize(GetLevelNumOfRows(), std::vector<Tile*>(GetLevelNumOfColumns(), nullptr));
	for (int row = 0; row < GetLevelNumOfRows(); row++) {
		for (int col = 0; col < GetLevelNumOfColumns(); col++) {
			level_world_tile_list_[row][col] = new Tile(col * GetLevelTileWidth(), row * GetLevelTileHeight(), GetLevelTileWidth(), GetLevelTileHeight(), level_raw_str_list[row][col], WORLD_RECT);
		}
	}
	MapAllTileNeighbors();

	// BUILD TILE TEXTURE DATABASE USING "FAKE" TILE OBJECTS TO HOLD DATA
	BuildTileTextureDatabase();

	// TRANSLATE CHAR TO TEXTURE ID
	MapAllTileTextureIdAndCollision();

	UpdateVisibleTileList();
}

void Level::UpdateVisibleTileList()
{
	visible_tile_list_.resize(GetVisibleTilesNumOfRows() + 2, std::vector<Tile*>(GetVisibleTilesNumOfColumns() + 2, nullptr)); //overdraw by +2 rows/columns

	for (int row = 0; row < GetVisibleTilesNumOfRows() + 2; row++) {
		// This resize method for columns is better
		visible_tile_list_[row].resize(GetVisibleTilesNumOfColumns() + 2);
		for (int col = 0; col < GetVisibleTilesNumOfColumns() + 2; col++) {
			/*SDL_Rect temp_rect = { col * GetLevelTileWidth(), row * GetLevelTileHeight(), GetLevelTileWidth(), GetLevelTileHeight() };
			visible_tile_list_[row][col] = &temp_rect;*/
			visible_tile_list_[row][col] = new Tile(col * GetLevelTileWidth(), row * GetLevelTileHeight(), GetLevelTileWidth(), GetLevelTileHeight(), DST_RECT);
		}
	}
}

int Level::GetLevelNumOfColumns()
{
	return level_num_of_columns_;
}

int Level::GetLevelNumOfRows()
{
	return level_num_of_rows_;
}

int Level::GetLevelTileWidth()
{
	return level_tile_width_;
}

int Level::GetLevelTileHeight()
{
	return level_tile_height_;
}

int Level::GetLevelMaxPosX()
{
	return GetLevelNumOfColumns() * GetLevelTileWidth();
}

int Level::GetLevelMaxPosY()
{
	return GetLevelNumOfRows() * GetLevelTileHeight();
}

int Level::GetTileIndexFromPosX(int coord) {
	return int(coord / GetLevelTileWidth());
}

int Level::GetTileIndexFromPosY(int coord) {
	return int(coord / GetLevelTileHeight());
}

int Level::GetVisibleTilesNumOfColumns()
{
	return Globals::sWindowWidth / GetLevelTileWidth();
}

int Level::GetVisibleTilesNumOfRows()
{
	return Globals::sWindowHeight / GetLevelTileHeight();
}

int Level::GetCamPosX()
{
	return cam_pos_x_;
}

int Level::GetCamPosY()
{
	return cam_pos_y_;
}

int Level::GetCamCenterX()
{
	return cam_pos_x_ + Globals::sWindowWidth / 2;
}

int Level::GetCamCenterY()
{
	return cam_pos_y_ + Globals::sWindowHeight / 2;
}

void Level::SetBkgColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bkg_r_value_ = r;
	bkg_g_value_ = g;
	bkg_b_value_ = b;
	bkg_a_value_ = a;
}

void Level::SetLevelRawStr(std::string raw_str)
{
	level_raw_str_ = raw_str;
}

void Level::SetLevelNumOfColumns(int total_columns)
{
	this->level_num_of_columns_ = total_columns;
}

void Level::SetLevelNumOfRows(int total_rows)
{
	this->level_num_of_rows_ = total_rows;
}

void Level::SetLevelTileWidth(int width)
{
	level_tile_width_ = width;
}

void Level::SetLevelTileHeight(int height)
{
	level_tile_height_ = height;
}

void Level::SetCamPosX(int x_coord)
{
	if (x_coord < 0) {
		cam_pos_x_ = 0;
	}
	else {
		cam_pos_x_ = x_coord;
	}
}

void Level::SetCamPosY(int y_coord)
{
	if (y_coord < 0) {
		cam_pos_y_ = 0;
	}
	else {
		cam_pos_y_ = y_coord;
	}
}