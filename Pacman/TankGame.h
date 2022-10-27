#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class TankGame : public Game
{
private:
	// Data to represent Player
	Vector2* _playerPosition;
	Vector2* _playerLastPosition;
	Rect* _playerSourceRect;
	Rect* _playerTurretSourceRect;
	Texture2D* _playerTexture;
	Texture2D* _playerTurretTexture;

	int _playerDirection;
	int _playerFrame;
	int _playerCurrentFrameTime;
	bool _playerIsMoving;


	//Constant data for Game Variables.
	const float _cPlayerSpeed;
	const int _cPlayerFrameTime;
	const int _cAmmoFrameTime;

	// Data to represent Ammo
	Rect* _ammoRect;
	Texture2D* _ammoBlueTexture;
	Texture2D* _ammoInvertedTexture;
	int _ammoFrame;
	int _ammoCurrentFrameTime;
	int _ammoFrameCount;


	// Position for String
	Vector2* _stringPosition;

	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Vector2* _startStringPosition;
	bool _paused;
	bool _escKeyDown;
	bool _startGameMenu;

	
	


public:
	/// <summary> Constructs the Pacman class. </summary>
	TankGame(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Player class. </summary>
	virtual ~TankGame();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};