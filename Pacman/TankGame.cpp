#include "TankGame.h"
#include <sstream>
#include <iostream>

//Constructor
TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(100), _cAmmoFrameTime(500)
{
	//Seed random
	srand(time(NULL));
	
	//Initialise important Game aspects
	_player = new Player();
	_player->isPlayerDead = false;

	//Test stuff
	Vector2* testVec1 = new Vector2(0,0);
	Vector2* testVec2 = new Vector2(3, 4);
	
	AmmoPickup::_texture.Load("Textures/Ammo.png", false);

	for (int i = 0; i < _initalAmmoCount; i++)
	{
		AmmoVector.push_back(*new AmmoPickup());
	}

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_drones[i] = new MovingEnemy();
<<<<<<< Updated upstream
		_drones[i]->direction = 0;
		_drones[i]->speed = 0.2f;
	
=======
		_drones[i]->_direction = 0;
		_drones[i]->_speed = 0.2f;
		_drones[i]->_droneFrameCount = rand() % 1;
		_drones[i]->_droneCurrentFrame = 0;
		_drones[i]->_droneFrame = rand() % 500 + 50;
	}
	
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		_explosions[i] = new Explosion();
		_explosions[i]->_boomFrameCount = 0;
		_explosions[i]->_boomCurrentFrame = 0;
		_explosions[i]->_boomFrame = rand() % 500 + 50;
		_explosions[i]->_inUse = false;
>>>>>>> Stashed changes
	}
	
	std::cout << GetRadians(testVec1,testVec2) << endl;
	
	
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

<<<<<<< Updated upstream
	delete _ammoTexture;
	delete[] *_ammoPickup;
=======
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		delete _explosions[i]->_position;
		delete _explosions[i]->_sourceRect;
		delete _explosions[i];
	}

	delete _ammoTexture;
	delete _boomTexture;
>>>>>>> Stashed changes
	delete _menuBackground;
	delete _player;
	delete[] *_drones;
	delete _droneTexture;

	delete _stringPosition;
	delete _stringPosition2;
	
}

void TankGame::LoadContent() 
{
	//Initilize Player
<<<<<<< Updated upstream
	_player->_playerTexture = new Texture2D();
	_player->_playerTurretTexture = new Texture2D();
	_player->_playerTexture->Load("Textures/TankBaseSheet.png", false);
	_player->_playerTurretTexture->Load("Textures/TankTurretSheet.png", false);
	
	_player->_playerPosition = new Vector2(350.0f, 350.0f);
	_player->_playerLastPosition = new Vector2(350.0f, 350.0f);
=======
	_player->_texture = new Texture2D();
	_player->_turretTexture = new Texture2D();
	_player->_texture->Load("Textures/TankBaseSheet.png", false);
	_player->_turretTexture->Load("Textures/TankTurretSheet.png", false);
	_player->_position = new Vector2(350.0f, 350.0f);
	_player->_lastPosition = new Vector2(350.0f, 350.0f);
>>>>>>> Stashed changes
	_player->_mousePosition = new Vector2(0.0f, 0.0f);
	_player->_turretPosition = new Vector2(0.0f, 0.0f);
	
<<<<<<< Updated upstream
	_player->_playerTurretSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	
	_player->_isPlayerMoving = false;
	_player->_playerDirection = 0;
=======
	_player->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_turretSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	_player->_direction = 0;
>>>>>>> Stashed changes
	_player->_playerCurrentFrameTime = 0;
	_player->_playerFrame = 0;
	_player->_turretRotation = 0.0f;

<<<<<<< Updated upstream
	_player->_playerAmmoCount = 3;

	//Initilize Ammo
	_ammoTexture->Load("Textures/Ammo.png", false);
	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{	
		_ammoPickup[i]->_ammoTexture = _ammoTexture;
		_ammoPickup[i]->_ammoRect = new Rect(0.0f, 0.0f, 16, 16);
		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
		_ammoPickup[i]->_ammoFrameCount = 0;
		_ammoPickup[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ammoPickup[i]->_isFollowingMouse = false;
	}

=======
>>>>>>> Stashed changes
	//Initilize Drones

	_droneTexture->Load("Textures/BlueGhost.png", false);
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
<<<<<<< Updated upstream
		_drones[i]->texture = _droneTexture;
		_drones[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_drones[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
=======
		_drones[i]->_texture = _droneTexture;
		_drones[i]->_position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_drones[i]->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	}

	//Initilize Explosions
	_boomTexture->Load("Textures/Explosion.png", false);
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		_explosions[i]->_texture = _boomTexture;
		_explosions[i]->_position = new Vector2(-200,-200);
		_explosions[i]->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
>>>>>>> Stashed changes

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

bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
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

<<<<<<< Updated upstream
float GetRadians(Vector2* p1, Vector2* p2)
{
	float length1, length2;
	length1 = p2->X - p1->X;
	length2 = p2->Y - p1->Y;

	return (atan(length1 / length2));
}

float GetDegrees(float radians)
{
	float result = radians / 3.14159f * 180;
	if (result < 0)
		result = -result;
	return result;
	//return(((radians / 3.14159f * 180)+360)%360);
=======
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
>>>>>>> Stashed changes
}

void TankGame::Input(int elapsedTime, Input::KeyboardState* state,Input::MouseState* mouseState)
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
	
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
	_player->_mousePosition->X = mouseState->X;
	_player->_mousePosition->Y = mouseState->Y;
	
}

<<<<<<< Updated upstream
void TankGame::UpdateDrone(MovingEnemy* drone, int elapsedTime)
=======
void TankGame::ShowExplosion(Vector2* position)
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
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
	
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
	
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	_player->_playerSourceRect->Y = _player->_playerSourceRect->Height * _player->_playerDirection;

	_player->_playerTurretPosition = _player->_playerPosition;
	
=======
	_player->_sourceRect->Y = _player->_sourceRect->Height * _player->_direction;
>>>>>>> Stashed changes
}

void TankGame::UpdateAmmoPickups(int i,int elapsedTime)
{
	//Tracking time since last ammo animation frame change.
	AmmoVector[i]._ammoCurrentFrameTime += elapsedTime;

	//Updating the ammo frame.
	if (AmmoVector[i]._ammoCurrentFrameTime > _cAmmoFrameTime)
	{
<<<<<<< Updated upstream
		_ammoPickup[i]->_ammoFrameCount++;
		_ammoPickup[i]->_ammoFrame = 0;
=======
		AmmoVector[i]._sourceRect->X = AmmoVector[i]._sourceRect->Width * AmmoVector[i]._ammoFrameCount;
		AmmoVector[i]._ammoFrameCount++;
>>>>>>> Stashed changes

		if (AmmoVector[i]._ammoFrameCount >= 2)
		{
<<<<<<< Updated upstream
			_ammoPickup[i]->_ammoFrameCount = 0;
			_ammoPickup[i]->_ammoFrame = 1;
		}

		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
		_ammoPickup[i]->_ammoRect->X = _ammoPickup[i]->_ammoRect->Width * _ammoPickup[i]->_ammoFrame;
=======
			AmmoVector[i]._ammoFrameCount = 0;
		}

		AmmoVector[i]._ammoCurrentFrameTime = 0;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
		Input(elapsedTime, keyboardState, mouseState);
		UpdatePlayer(elapsedTime);
		
		//Determine turret rotation.
		_player->_turretRotation = GetDegrees(GetRadians(_player->_playerTurretPosition, _player->_mousePosition));

		for (int i = 0; i < AMMOPICKUPCOUNT; i++)
=======
		CheckPaused(keyboardState);

		//For things that need to stop when paused.
		if (!_paused)
>>>>>>> Stashed changes
		{
			//Determine players last position for isMoving.
			_player->_lastPosition->X = _player->_position->X;
			_player->_lastPosition->Y = _player->_position->Y;

<<<<<<< Updated upstream
		for (int i = 0; i < ENEMYCOUNT; i++)
		{
			UpdateDrone(_drones[i], elapsedTime);
			if (CollisionCheck(_player->_playerPosition->X, _player->_playerPosition->Y, _player->_playerSourceRect->Width, _player->_playerSourceRect->Height, _drones[i]->position->X,
				_drones[i]->position->Y, _drones[i]->sourceRect->Width, _drones[i]->sourceRect->Height))
			{
				_player->isPlayerDead = true;
				i = ENEMYCOUNT;
=======
			//Determine turret rotation.
			_player->_turretRotation = getDegrees(_player->_mousePosition->X - _player->_turretPosition->X, _player->_mousePosition->Y - _player->_turretPosition->Y);

			Input(elapsedTime, keyboardState, mouseState);
			UpdatePlayer(elapsedTime);

			for (int i = 0; i < AmmoVector.size(); i++)
			{
				UpdateAmmoPickups(i, elapsedTime);
>>>>>>> Stashed changes
			}

<<<<<<< Updated upstream
		for (int i = 0; i < AMMOPICKUPCOUNT; i++)
		{
			UpdateDrone(_drones[i], elapsedTime);
			if (CollisionCheck(_player->_playerPosition->X, _player->_playerPosition->Y, _player->_playerSourceRect->Width, _player->_playerSourceRect->Height, _ammoPickup[i]->position->X,
				_ammoPickup[i]->position->Y, _ammoPickup[i]->_ammoRect->Width, _ammoPickup[i]->_ammoRect->Height))
			{
				_ammoPickup[i]->position->Y = -150.0f;
				_ammoPickup[i]->position->X = -150.0f;
				_player->_playerAmmoCount += 3;
				i = AMMOPICKUPCOUNT;
=======
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
>>>>>>> Stashed changes
			}

<<<<<<< Updated upstream

		CheckViewportCollision();
=======
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
>>>>>>> Stashed changes

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
	Score << "Turret radians: " << _player->_turretRotation;

	std::stringstream Ammo;
	Ammo << "Turret pos y: " << _player->_playerTurretPosition->Y;
	
	SpriteBatch::BeginDraw(); // Starts Drawing
	
	if (!_player->isPlayerDead)
	{
<<<<<<< Updated upstream
		Vector2* turretCenter = new Vector2(16 + _player->_playerTurretPosition->X, 16 + _player->_playerTurretPosition->Y);
		SpriteBatch::Draw(_player->_playerTexture, _player->_playerPosition, _player->_playerSourceRect); //Draws player.
		//for rotation and origin look into the other draw.
		SpriteBatch::Draw(_player->_playerTurretTexture,_player->_playerTurretPosition , _player->_playerTurretSourceRect, new Vector2(0, 0), 1, _player->_turretRotation, Color::White, SpriteEffect::NONE); //Draws player turret.
=======
		Vector2* testVecPointer = new Vector2(16, 24);
		SpriteBatch::Draw(_player->_texture, _player->_position, _player->_sourceRect); //Draws player.
		SpriteBatch::Draw(_player->_turretTexture, _player->_turretPosition, _player->_turretSourceRect, testVecPointer, 1, _player->_turretRotation, Color::White, SpriteEffect::NONE); //Draws player turret.
>>>>>>> Stashed changes
	}

	//draw ammo
	for (int i = 0; i < AmmoVector.size(); i++)
	{
		SpriteBatch::Draw(AmmoPickup::_texture, AmmoVector[i]._position, AmmoVector[i]._sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	//Draw drones
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		SpriteBatch::Draw(_drones[i]->_texture, _drones[i]->_position, _drones[i]->_sourceRect);
	}

<<<<<<< Updated upstream
	// Draws String
	SpriteBatch::DrawString(Score.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(Ammo.str().c_str(), _stringPosition2, Color::Green);
=======
	//Draw explosions
	for (int i = 0; i < EXPLOSIONS; i++)
	{
		SpriteBatch::Draw(_explosions[i]->_texture, _explosions[i]->_position, _explosions[i]->_sourceRect);
	}

	// Draws Strings
	SpriteBatch::DrawString(score.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(ammo.str().c_str(), _stringPosition2, Color::Green);
>>>>>>> Stashed changes
	
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
		SpriteBatch::DrawString(menuStream.str().c_str(),_startStringPosition, Color::Green);
	}
	
	SpriteBatch::EndDraw(); // Ends Drawing
}

