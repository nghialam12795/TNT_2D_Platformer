#include "Engine.h"

// Main function.
int main(int argc, char* args[]) {
	//Engine engine_obj;
	//return engine_obj.Run();

	return Engine::Instance().Run();
}