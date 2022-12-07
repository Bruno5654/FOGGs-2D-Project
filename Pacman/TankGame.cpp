#include "TankGame.h"
#include <sstream>
#include <iostream>

//Constructor
TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(100), _cAmmoFrameTime(500), _cExplosionFrameTime(50), _cDroneFrameTime(100)
{
	//Seed random
	srand(time(NULL));

	//Initialise important Game aspects
	_player = new Player();
	_player->isPlayerDead = false;

	//AmmoPickup::_texture.Load("Textures/Ammo.png", false);

	for (int i = 0; i < _initalAmmoCount; i++)
	{
		AmmoVector.push_back(*new AmmoPickup());
	}

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_drones[i] = new MovingEnemy();
		_drones[i]->_direction = 0;
		_drones[i]->_speed = 0.2f;
		_drones[i]->_droneFrameCount = rand() % 1;
		_drones[i]->_droneCurrentFrame = 0;
		_drones[i]->_droneFrame = rand() % 500 + 50;
	}

	for (int i = 0; i < BUILDINGS; i++)
	{
		_buildings[i] = new Building();
		_buildings[i]->_stage = 1;
		_buildings[i]->_isPassable = false;
	}

	for (int i = 0; i < EXPLOSIONS; i++)
	{
		_explosions[i] = new Explosion();
		_explosions[i]->_boomFrameCount = 0;
		_explosions[i]->_boomCurrentFrame = 0;
		_explosions[i]->_boomFrame = rand() % 500 + 50;
		_explosions[i]->_inUse = false;
	}


	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Tank Game", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

//Destructor
TankGame::~TankGame()
{
	delete _player->_texture;
	delete _player->_turretTexture;
	delete _player->_sourceRect;
	delete _player->_turretSourceRect;
	delete _player->_turretPosition;
	delete _player->_mousePosition;

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		delete _drones[i]->_position;
		delete _drones[i]->_sourceRect;
		delete _drones[i];
	}

	for (int i = 0; i < EXPLOSIONS; i++)
	{
		delete _explosions[i]->_position;
		delete _explosions[i]->_sourceRect;
		delete _explosions[i];
	}

	for (int i = 0; i < BUILDINGS; i++)
	{
		delete _buildings[i]->_position;
		delete _buildings[i]->_sourceRect;
		delete _buildings[i];
	}

	delete _ammoTexture;
	delete _boomTexture;
	delete _menuBackground;
	delete _player;
	delete[] * _drones;
	delete _droneTexture;
	delete[] * _buildings;
	delete _buildingTexture;

	delete _stringPosition;
	delete _stringPosition2;

}

void TankGame::LoadContent()
{
	//Initilize Player
	_player->_texture = new Texture2D();
	_player->_turretTexture = new Texture2D();
	_player->_texture->Load("Textures/TankBaseSheet.png", false);
	_player->_turretTexture->Load("Textures/TankTurretSheet.png", false);
	_player->_position = new Vector2(350.0f, 350.0f);
	_player->_lastPosition = new Vector2(350.0f, 350.0f);
	_player->_mousePosition = new Vector2(0.0f, 0.0f);
	_player->_turretPosition = new Vector2(0.0f, 0.0f);

	_player->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_turretSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	_player->_direction = 0;
	_player->_playerCurrentFrameTime = 0;
	_player->_playerFrame = 0;
	_player->_turretRotation = 0.0f;

	//Initilize Drones

	_droneTexture->Load("Textures/Drone.png", false);
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_drones[i]->_texture = _droneTexture;
		_drones[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_drones[i]->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	//Initilize Explosions
	_boomTexture->Load("Textures/Explosion.png", false);
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		_explosions[i]->_texture = _boomTexture;
		_explosions[i]->_position = new Vector2(-200, -200);
		_explosions[i]->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}
	
	//Initilize Buildings
	
	Vector2* randPos = new Vector2(0, 0);
	_buildingTexture->Load("Textures/Building.png", false);
	for (int i = 0; i < BUILDINGS; i++)
	{
		bool isOk = false;
		while (!isOk)
		{
			isOk = true;
			randPos = new Vector2((rand() % (Graphics::GetViewportWidth()-50)), (rand() % (Graphics::GetViewportHeight()-50)));
			for (int j = 0; j < i; j++)
			{
				isOk = !CollisionCheck(randPos->X, randPos->Y, 32, 32, _buildings[j]->_position->X, _buildings[j]->_position->Y, 32, 32);
				if (!isOk)
					break;
				
			}
		}
		_buildings[i]->_position = randPos;
		_buildings[i]->_texture = _buildingTexture;
		_buildings[i]->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	//Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_stringPosition2 = new Vector2(10.0f, 50.0f);

	//Set menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_startStringPosition = new Vector2(550.0f, Graphics::GetViewportHeight() / 2.0f);
	_paused = true;
	_startGameMenu = true;
	_escKeyDown = false;

	//Set gameState
	gameState = 1;
}

bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) //Returning true = collision
{
	int left1 = x1, left2 = x2, right1 = x1 + width1, right2 = x2 + width2, top1 = y1, top2 = y2, bottom1 = y1 + height1, bottom2 = y2 + height2;
	if (bottom1 < top2)
		return false;
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;

	return true;
}

float getDegrees(float x, float y) {
	float angle = 0;

	if (x < 0) {
		angle = (float)(270 - (atan(y / -x) * 180 / 3.14159));
	}
	else {
		angle = (float)(90 + (atan(y / x) * 180 / 3.14159));
	}

	angle = (int(180 - angle) % 360) + 180;

	return int(angle);
}

void TankGame::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	if (gameState == 1)
	{
		//WASD player input and movement.
		if (state->IsKeyDown(Input::Keys::D))
		{
			_player->_position->X += _cPlayerSpeed * elapsedTime;
			_player->_direction = 0;
		}
		else if (state->IsKeyDown(Input::Keys::A))
		{
			_player->_position->X -= _cPlayerSpeed * elapsedTime;
			_player->_direction = 2;
		}
		else if (state->IsKeyDown(Input::Keys::S))
		{
			_player->_position->Y += _cPlayerSpeed * elapsedTime;
			_player->_direction = 1;
		}
		else if (state->IsKeyDown(Input::Keys::W))
		{
			_player->_position->Y -= _cPlayerSpeed * elapsedTime;
			_player->_direction = 3;
		}
	}


	_player->_mousePosition->X = mouseState->X;
	_player->_mousePosition->Y = mouseState->Y;

}

void TankGame::ShowExplosion(Vector2 * position)
{
	bool foundBoom = false;

	for (int i = 0; i < EXPLOSIONS; i++)
	{
		if (_explosions[i]->_inUse == false && foundBoom == false)
		{
			foundBoom = true;
			_explosions[i]->_position->X = position->X;
			_explosions[i]->_position->Y = position->Y;
			_explosions[i]->_inUse = true;
			break;
		}
	}
}

void TankGame::UpdateDrone(MovingEnemy* drone, int elapsedTime, int i)
{
	if (drone->_direction == 0)//Move Right
	{
		drone->_position->X += drone->_speed * elapsedTime;
	}
	else if (drone->_direction == 1)//Move Left
	{
		drone->_position->X -= drone->_speed * elapsedTime;
	}

	if (drone->_position->X + drone->_sourceRect->Width >= Graphics::GetViewportWidth())//Change direction on right edge.
	{
		drone->_direction = 1;
	}
	else if (drone->_position->X <= 0)
	{
		drone->_direction = 0;
	}

	//Tracking time since last drone animation frame change.
	_drones[i]->_droneCurrentFrame += elapsedTime;
	//Updating the drone frame.
	if (_drones[i]->_droneCurrentFrame > _cDroneFrameTime)
	{
		_drones[i]->_sourceRect->X = _drones[i]->_sourceRect->Width * _drones[i]->_droneFrameCount;
		_drones[i]->_droneFrameCount++;

		if (_drones[i]->_droneFrameCount >= 2)
		{
			_drones[i]->_droneFrameCount = 0;
		}
		_drones[i]->_droneCurrentFrame = 0;
	}
}

void TankGame::UpdateBoom(int elapsedTime, int i)
{
	if (_explosions[i]->_inUse == true)
	{
		//Tracking time since last drone animation frame change.
		_explosions[i]->_boomCurrentFrame += elapsedTime;
		//Updating the explosion frame.
		if (_explosions[i]->_boomCurrentFrame > _cExplosionFrameTime)
		{
			_explosions[i]->_sourceRect->X = _explosions[i]->_sourceRect->Width * _explosions[i]->_boomFrameCount;
			_explosions[i]->_boomFrameCount++;

			if (_explosions[i]->_boomFrameCount >= 5)
			{
				_explosions[i]->_boomFrameCount = 0;
				_explosions[i]->_position->X = -200;
				_explosions[i]->_position->Y = -200;
				_explosions[i]->_inUse = false;
				if (_player->isPlayerDead = true)
				{
					gameState = 2;
				}
			}
			_explosions[i]->_boomCurrentFrame = 0;
		}
	}

}

void TankGame::CheckPaused(Input::KeyboardState* state)
{
	//Checks for pause input.
	if (state->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown && !_startGameMenu)
	{
		_escKeyDown = true;
		_paused = !_paused;
	}

	if (state->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	if ((state->IsKeyDown(Input::Keys::SPACE) && _startGameMenu))
	{
		_startGameMenu = false;
		_paused = !_paused;
	}
}

void TankGame::CheckViewportCollision()
{
	//Colliding with the walls.
	if (_player->_position->X + _player->_sourceRect->Width > Graphics::GetViewportWidth())
	{
		_player->_position->X = Graphics::GetViewportWidth() - _player->_sourceRect->Width;
	}

	if (_player->_position->X < 0)
	{
		_player->_position->X = 0;
	}

	if (_player->_position->Y + _player->_sourceRect->Height > Graphics::GetViewportHeight())
	{
		_player->_position->Y = Graphics::GetViewportHeight() - _player->_sourceRect->Height;
	}

	if (_player->_position->Y < 0)
	{
		_player->_position->Y = 0;

	}
}

void TankGame::UpdatePlayer(int elapsedTime)
{
	//Checking if the player is moving.
	if (_player->_direction == 0 || _player->_direction == 2)
	{
		if (_player->_lastPosition->X > _player->_position->X || _player->_lastPosition->X < _player->_position->X)
		{
			_player->_isPlayerMoving = true;
		}
		else
		{
			_player->_isPlayerMoving = false;
		}
	}
	else
	{
		if (_player->_lastPosition->Y > _player->_position->Y || _player->_lastPosition->Y < _player->_position->Y)
		{
			_player->_isPlayerMoving = true;
		}
		else
		{
			_player->_isPlayerMoving = false;
		}
	}

	if (_player->_isPlayerMoving)
	{
		//Tracking time since last player animation frame change.
		_player->_playerCurrentFrameTime += elapsedTime;

		//Updating the x column of the animation.
		if (_player->_playerCurrentFrameTime > _cPlayerFrameTime)
		{
			_player->_playerFrame++;

			if (_player->_playerFrame >= 4)
			{
				_player->_playerFrame = 0;
			}

			_player->_playerCurrentFrameTime = 0;

			_player->_sourceRect->X = _player->_sourceRect->Width * _player->_playerFrame;
		}
	}

	//Changing player direction
	_player->_sourceRect->Y = _player->_sourceRect->Height * _player->_direction;
}

void TankGame::UpdateAmmoPickups(int i, int elapsedTime)
{
	//Tracking time since last ammo animation frame change.
	AmmoVector[i]._ammoCurrentFrameTime += elapsedTime;

	//Updating the ammo frame.
	if (AmmoVector[i]._ammoCurrentFrameTime > _cAmmoFrameTime)
	{
		AmmoVector[i]._sourceRect->X = AmmoVector[i]._sourceRect->Width * AmmoVector[i]._ammoFrameCount;
		AmmoVector[i]._ammoFrameCount++;

		if (AmmoVector[i]._ammoFrameCount >= 2)
		{
			AmmoVector[i]._ammoFrameCount = 0;
		}

		AmmoVector[i]._ammoCurrentFrameTime = 0;
	}

}

void TankGame::KillPlayer()
{
	_player->isPlayerDead = true;
	ShowExplosion(_player->_position);
}

void TankGame::Update(int elapsedTime)
{
	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	switch (gameState)
	{
	case 1:

		CheckPaused(keyboardState);

		//For things that need to stop when paused.
		if (!_paused)
		{
			//Determine players last position for isMoving.
			_player->_lastPosition->X = _player->_position->X;
			_player->_lastPosition->Y = _player->_position->Y;

			//Determine turret rotation.
			_player->_turretRotation = getDegrees(_player->_mousePosition->X - _player->_turretPosition->X, _player->_mousePosition->Y - _player->_turretPosition->Y);

			Input(elapsedTime, keyboardState, mouseState);
			UpdatePlayer(elapsedTime);

			for (int i = 0; i < AmmoVector.size(); i++)
			{
				UpdateAmmoPickups(i, elapsedTime);
			}

			for (int i = 0; i < EXPLOSIONS; i++)
			{
				UpdateBoom(elapsedTime, i);
			}

			//Drone Collision
			for (int i = 0; i < ENEMYCOUNT; i++)
			{
				UpdateDrone(_drones[i], elapsedTime, i);
				if (CollisionCheck(_player->_position->X, _player->_position->Y, _player->_sourceRect->Width, _player->_sourceRect->Height, _drones[i]->_position->X,
					_drones[i]->_position->Y, _drones[i]->_sourceRect->Width, _drones[i]->_sourceRect->Height))
				{
					KillPlayer();
					i = ENEMYCOUNT;

				}
			}

			//Ammo Collision
			for (int i = 0; i < AmmoVector.size(); i++)
			{
				if (CollisionCheck(_player->_position->X, _player->_position->Y, _player->_sourceRect->Width, _player->_sourceRect->Height, AmmoVector[i]._position->X,
					AmmoVector[i]._position->Y, AmmoVector[i]._sourceRect->Width, AmmoVector[i]._sourceRect->Height))
				{
					_player->_ammo += 3;
					AmmoVector[i]._position->Y = -150;
					AmmoVector[i]._position->X = -150;
				}
			}

			CheckViewportCollision();

			//Set turret position.
			_player->_turretPosition->X = _player->_position->X + 16;
			_player->_turretPosition->Y = _player->_position->Y + 16;

		}
		break;
	}

}

void TankGame::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream Score;
	//this will be the players current score eventually.
	Score << "Score: " << _player->_score;

	std::stringstream Ammo;
	Ammo << "Ammo: " << _player->_ammo;

	SpriteBatch::BeginDraw(); // Starts Drawing

	if (!_player->isPlayerDead)
	{
		Vector2* _turretOffset = new Vector2(16, 24);
		SpriteBatch::Draw(_player->_texture, _player->_position, _player->_sourceRect); //Draws player.
		SpriteBatch::Draw(_player->_turretTexture, _player->_turretPosition, _player->_turretSourceRect, _turretOffset, 1, _player->_turretRotation, Color::White, SpriteEffect::NONE); //Draws player turret.
	}

	//Draw Buildings
	for (int i = 0; i < BUILDINGS; i++)
	{
		SpriteBatch::Draw(_buildings[i]->_texture, _buildings[i]->_position, _buildings[i]->_sourceRect);
	}

	//draw ammo
	for (int i = 0; i < AmmoVector.size(); i++)
	{
		//SpriteBatch::Draw(AmmoPickup::_texture, AmmoVector[i]._position, AmmoVector[i]._sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	//Draw drones
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		SpriteBatch::Draw(_drones[i]->_texture, _drones[i]->_position, _drones[i]->_sourceRect);
	}
	
	// Draws String
	SpriteBatch::DrawString(Score.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(Ammo.str().c_str(), _stringPosition2, Color::Green);

	//Draw explosions
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		SpriteBatch::Draw(_explosions[i]->_texture, _explosions[i]->_position, _explosions[i]->_sourceRect);
	}

	// Draws Strings
	SpriteBatch::DrawString(Score.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(Ammo.str().c_str(), _stringPosition2, Color::Green);

		if (_paused && !_startGameMenu)
		{
			std::stringstream menuStream;
			menuStream << "Paused";
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::White);
		}
		else if (_paused && _startGameMenu)
		{
			std::stringstream menuStream;
			menuStream << "Press Space to Start";
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _startStringPosition, Color::Green);
		}

	

	SpriteBatch::EndDraw(); // Ends Drawing
}
