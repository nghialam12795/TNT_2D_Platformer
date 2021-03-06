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
	for (int x = 0; x < GetVisibleTilesNumOfColumns(); x++) {
		for (int y = 0; y < GetVisibleTilesNumOfRows(); y++) {
			visible_tile_list_[x][y].x = x * GetLevelTileWidth() - GetTileOffsetX();
			visible_tile_list_[x][y].y = y * GetLevelTileHeight() - GetTileOffsetY();
		}
	}
}

void Level::draw()
{
	char tile_char = ' ';
	for (int x = 0; x < GetVisibleTilesNumOfColumns(); x++) {
		for (int y = 0; y < GetVisibleTilesNumOfRows(); y++) {
			tile_char = GetTileChar(x + GetTileIndexFromPosX(GetCamPosX()), y + GetTileIndexFromPosY(GetCamPosY()));
			//if (x == -1 && y == -1) {
			//	std::cout << "tile_char =" << tile_char << std::endl;
			//}
			switch (tile_char)
			{
			case 'G':
				TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(), tile_texture_list_[CHURCH_GROUND_01]->getTextureId(), tile_texture_list_[CHURCH_GROUND_01]->getSrc(), &visible_tile_list_[x][y], 0.0, 0, SDL_FLIP_NONE);
				break;
			case '#':
				TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(), tile_texture_list_[CHURCH_GROUND_02]->getTextureId(), tile_texture_list_[CHURCH_GROUND_02]->getSrc(), &visible_tile_list_[x][y], 0.0, 0, SDL_FLIP_NONE);
				break;
			case '@':
				TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(), tile_texture_list_[CHURCH_BLOCK_01]->getTextureId(), tile_texture_list_[CHURCH_BLOCK_01]->getSrc(), &visible_tile_list_[x][y], 0.0, 0, SDL_FLIP_NONE);
				break;
			default:
				TheTextureManager::Instance()->draw(TheGame::Instance()->getRenderer(), tile_texture_list_[CHURCH_BKG_01]->getTextureId(), tile_texture_list_[CHURCH_BKG_01]->getSrc(), &visible_tile_list_[x][y], 0.0, 0, SDL_FLIP_NONE);
				break;
			}
		}
	}

	ui_pause_ptr_->draw();
	ui_quit_ptr_->draw();

	/*char tile_char = ' ';
	for (int i = 0; i < GetLevelWidth(); i++) {
		for (int j = 0; j < GetLevelWidth(); j++) {
			SDL_Rect temp_rect = { -GetLevelTileWidth(), -GetLevelTileHeight(), GetLevelTileWidth(), GetLevelTileHeight() };
			temp_rect.x = i * GetLevelTileWidth();
			temp_rect.y = j * GetLevelTileHeight();
			tile_char = GetTileChar(i, j);
			switch (tile_char)
			{
			case 'G':
				TheTextureManager::Instance()->draw(renderer, tile_texture_list_[CHURCH_GROUND_02]->getTextureId(), tile_texture_list_[CHURCH_GROUND_01]->getSrc(), &temp_rect, 0.0, 0, SDL_FLIP_NONE);
				break;
			default:
				break;
			}
		}
	}*/

}

void Level::clean()
{
}

void Level::LoadLevel(std::string level_id)
{
	std::string level_raw_str = "";
	if (!visible_tile_list_.empty()) {
		visible_tile_list_.clear();
	}

	if (level_id == "church") {
		level_raw_str += "G.G.............................................................";
		level_raw_str += "................................................................";
		level_raw_str += "G.G.............................................................";
		level_raw_str += "................................................................";
		level_raw_str += "G......................########.................................";
		level_raw_str += "......................###..............#.#......................";
		level_raw_str += "G...................###................#.#......................";
		level_raw_str += "...................####.........................................";
		level_raw_str += "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.##########.........########";
		level_raw_str += "........................############.#...............###........";
		level_raw_str += "........................#............#............###...........";
		level_raw_str += "........................#............#.........###..............";
		level_raw_str += "........................#.############......###.................";
		level_raw_str += "........................#................###....................";
		level_raw_str += "........................#..............##.......................";
		level_raw_str += "........................################........................";

		SetLevelRawStr(level_raw_str);
		SetLevelNumOfColumns(64);
		SetLevelNumOfRows(16);
	}

	SDL_Rect temp_rect = { -GetLevelTileWidth(), -GetLevelTileHeight(), GetLevelTileWidth(), GetLevelTileHeight() };
	for (int i = 0; i < GetVisibleTilesNumOfColumns(); i++) {
		std::vector<SDL_Rect> temp;
		temp.clear();
		for (int j = 0; j < GetVisibleTilesNumOfRows(); j++) {
			temp.push_back(temp_rect);
		}
		visible_tile_list_.push_back(temp);
	}

	// Tileset uses src, tiles use dst
	tileset_texture_list_[CHURCH_TILESET_01] = new Tile();
	tileset_texture_list_[CHURCH_TILESET_01]->setTextureId("church_tileset_01");
	TheTextureManager::Instance()->load("../Assets/textures/church_tileset_01.png", tileset_texture_list_[CHURCH_TILESET_01]->getTextureId(), TheGame::Instance()->getRenderer());
	//tileset_texture_list_[CHURCH_TILESET_01]->setSrc(0, 0, 336, 224);
	//tileset_texture_list_[CHURCH_TILESET_01]->SetCollidable(false);

	tile_texture_list_[CHURCH_BKG_01] = new Tile();
	tile_texture_list_[CHURCH_BKG_01]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BKG_01]->setSrc(128, 16, 15, 15);
	tile_texture_list_[CHURCH_BKG_01]->SetCollidable(false);

	tile_texture_list_[CHURCH_GROUND_01] = new Tile();
	tile_texture_list_[CHURCH_GROUND_01]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_GROUND_01]->setSrc(64, 168, 47, 39);
	tile_texture_list_[CHURCH_GROUND_01]->SetCollidable(true);

	tile_texture_list_[CHURCH_GROUND_02] = new Tile();
	tile_texture_list_[CHURCH_GROUND_02]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_GROUND_02]->setSrc(128, 168, 47, 39);
	tile_texture_list_[CHURCH_GROUND_02]->SetCollidable(true);

	tile_texture_list_[CHURCH_BLOCK_01] = new Tile();
	tile_texture_list_[CHURCH_BLOCK_01]->setTextureId(tileset_texture_list_[CHURCH_TILESET_01]->getTextureId());
	tile_texture_list_[CHURCH_BLOCK_01]->setSrc(16, 112, 31, 31);
	tile_texture_list_[CHURCH_BLOCK_01]->SetCollidable(true);
}

char Level::GetTileChar(int x_index, int y_index)
{
	if (x_index > -1 && x_index < GetLevelNumOfColumns() && y_index > -1 && y_index < GetLevelNumOfRows()) { // check bounds just in case
		return level_raw_str_[y_index * GetLevelNumOfColumns() + x_index]; //magic algorithm
	}
	else {
		return ' ';
	}
}

SDL_Rect* Level::GetVisibleTileObj(int x_index, int y_index)
{
	if (x_index > -1 && x_index < GetVisibleTilesNumOfColumns() && y_index > -1 && y_index < GetVisibleTilesNumOfRows()) { // check bounds just in case
		return &visible_tile_list_[x_index][y_index];
	}
	else {
		/*SDL_Rect temp = { 0,0,0,0 };
		return &temp;*/
		return nullptr;
	}
}

int Level::IsTileCharCollidable(char tile_char) {
	//0=solid block, 1=air/background, 2=consummables
	if (tile_char == 'G' || tile_char == '#' || tile_char == '@') {
		return 0;
	}
	else {
		return 1;
	}
}

void Level::CollisionDebug(SDL_Rect* game_obj)
{
	std::vector<int> result;
	for (int x = 0; x < GetVisibleTilesNumOfColumns(); x++) {
		for (int y = 0; y < GetVisibleTilesNumOfRows(); y++) {
			//std::cout << "x = " << x << std::endl;
			//std::cout << "y = " << y << std::endl;
			//std::cout << "index = " << index << std::endl;
			//std::cout << "X = " << visible_tile_list_[index].x << std::endl;
			//std::cout << "Y = " << visible_tile_list_[index].y << std::endl;
			if (CollisionManager::HaveCollidedAABB(game_obj, &(visible_tile_list_[x][y]))) {
				SetTileChar(x + GetTileIndexFromPosX(GetCamPosX()), y + GetTileIndexFromPosY(GetCamPosY()), '@');
				result.push_back(x);
			}
			//else {
			//	std::cout << "X1 = " << game_obj->x << std::endl;
			//	std::cout << "X2 = " << temp_ptr->x << std::endl;
			//}
			//if (x == -1 && y == -1) {
			//	std::cout << "x = " << x << std::endl;
			//	std::cout << "y = " << y << std::endl;
			//	std::cout << "index = " << index << std::endl;
			//	std::cout << "X1 = " << game_obj->x << std::endl;
			//	std::cout << "X2 = " << temp_ptr->x << std::endl;
			//}
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

int Level::GetLevelMaxPosX()
{
	return GetLevelNumOfColumns() * GetLevelTileWidth();
}

int Level::GetLevelMaxPosY()
{
	return GetLevelNumOfRows() * GetLevelTileHeight();
}

int Level::GetLevelTileWidth()
{
	return level_tile_width_;
}

int Level::GetLevelTileHeight()
{
	return level_tile_height_;
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

int Level::GetVisibleTilesNumOfColumns()
{
	return Globals::sWindowWidth / GetLevelTileWidth() + 1;
}

int Level::GetVisibleTilesNumOfRows()
{
	return Globals::sWindowHeight / GetLevelTileHeight() + 1;
}

int Level::GetTileOffsetX()
{
	// FIND STARTING COORD OF NEAREST TILE
	int nearest_tile_starting_coord = GetTileIndexFromPosX(GetCamPosX()) * GetLevelTileWidth();

	// RETURN OFFSET
	return (GetCamPosX() - nearest_tile_starting_coord);
}

int Level::GetTileOffsetY()
{
	// FIND STARTING COORD OF NEAREST TILE
	int nearest_tile_starting_coord = GetTileIndexFromPosY(GetCamPosY()) * GetLevelTileHeight();

	// RETURN OFFSET
	return (GetCamPosY() - nearest_tile_starting_coord);
}

int Level::GetTileIndexFromPosX(int coord) {
	return int(coord / GetLevelTileWidth());
}

int Level::GetTileIndexFromPosY(int coord) {
	return int(coord / GetLevelTileHeight());
}

void Level::SetLevelRawStr(std::string raw_str)
{
	level_raw_str_ = raw_str;
}

void Level::SetTileChar(int x, int y, char in_char)
{
	if (x > -1 && x < GetLevelNumOfColumns() && y > -1 && y < GetLevelNumOfRows()) {
		level_raw_str_[y * GetLevelNumOfColumns() + x] = in_char;
	}
}

void Level::SetLevelNumOfColumns(int width)
{
	this->level_num_of_columns_ = width;
}

void Level::SetLevelNumOfRows(int height)
{
	this->level_num_of_rows_ = height;
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
	else if (x_coord + Globals::sWindowWidth > GetLevelNumOfColumns() * GetLevelTileWidth()) {
		cam_pos_x_ = GetLevelNumOfColumns() * GetLevelTileWidth() - Globals::sWindowWidth;
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
	else if (y_coord + Globals::sWindowHeight > GetLevelNumOfRows() * GetLevelTileHeight()) {
		cam_pos_y_ = GetLevelNumOfRows() * GetLevelTileHeight() - Globals::sWindowHeight;
	}
	else {
		cam_pos_y_ = y_coord;
	}
}