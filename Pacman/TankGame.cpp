#include "TankGame.h"
#include <sstream>

//Constructor
TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(100), _cAmmoFrameTime(500)
{
	//Seed random
	srand(time(NULL));
	
	//Initialise important Game aspects
	_player = new Player();
	
	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{
		_ammoPickup[i] = new AmmoPickup();
		_ammoPickup[i]->_ammoFrameCount = rand() % 1;
		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
		_ammoPickup[i]->_ammoFrame = rand() % 500 + 50;
	}
	
	
	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Tank Game", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

//Destructor
TankGame::~TankGame() 
{
	delete _player->_playerTexture;
	delete _player->_playerTurretTexture;
	delete _player->_playerSourceRect;
	delete _player->_playerTurretSourceRect;
	
	

	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{
		delete _ammoPickup[i]->_ammoRect;
		delete _ammoPickup[i]->position;
		delete _ammoPickup[i];
	}
	
	delete _ammoTexture;
	delete _ammoInvertTexture;
	delete[] _ammoPickup;


	delete _menuBackground;
	delete _player;
}

void TankGame::LoadContent() 
{
	
	//Initilize Player
	_player->_playerTexture = new Texture2D();
	_player->_playerTurretTexture = new Texture2D();
	_player->_playerTexture->Load("Textures/TankBaseSheet.png", false);
	_player->_playerTurretTexture->Load("Textures/TankTurretSheet.png", false);
	_player->_playerPosition = new Vector2(350.0f, 350.0f);
	_player->_playerLastPosition = new Vector2(350.0f, 350.0f);
	_player->_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_playerTurretSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player->_playerDirection = 0;
	_player->_playerCurrentFrameTime = 0;
	_player->_playerFrame = 0;
	_player->_playerIsMoving = false;

	//Initilize Ammo
	
	_ammoTexture->Load("Textures/Munchie.tga", false);
	_ammoInvertTexture->Load("Textures/MunchieInverted.tga", true);
	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{	
		_ammoPickup[i]->_ammoBlueTexture = _ammoTexture;
		_ammoPickup[i]->_ammoInvertedTexture = _ammoInvertTexture;
		_ammoPickup[i]->_ammoRect = new Rect(0.0f, 0.0f, 12, 12);
		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
		_ammoPickup[i]->_ammoFrameCount = 0;
		_ammoPickup[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	//Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Set menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_startStringPosition = new Vector2(550.0f, Graphics::GetViewportHeight() / 2.0f);
	_paused = true;
	_startGameMenu = true;
	_escKeyDown = false;
}

void TankGame::Update(int elapsedTime)
{
	//Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	
	CheckPaused(keyboardState);

	//For things that need to stop when paused.
	if (!_paused)
	{
		//Determine players last position for isMoving.
		_player->_playerLastPosition->X = _player->_playerPosition->X;
		_player->_playerLastPosition->Y = _player->_playerPosition->Y;

		Input(elapsedTime, keyboardState);
		UpdatePlayer(elapsedTime);
		
		for (int i = 0; i < AMMOPICKUPCOUNT; i++)
		{
			UpdateAmmoPickups(i, elapsedTime);
		}
		
		CheckViewportCollision();

	}
}

void TankGame::Input(int elapsedTime, Input::KeyboardState* state)
{
	//WASD player input and movement.
	if (state->IsKeyDown(Input::Keys::D))
	{
		_player->_playerPosition->X += _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 0;
	}
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_player->_playerPosition->X -= _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 2;
	}
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_player->_playerPosition->Y += _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 1;
	}
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_player->_playerPosition->Y -= _cPlayerSpeed * elapsedTime;
		_player->_playerDirection = 3;
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
	if (_player->_playerPosition->X + _player->_playerSourceRect->Width > Graphics::GetViewportWidth())
	{
		_player->_playerPosition->X = Graphics::GetViewportWidth() - _player->_playerSourceRect->Width;
	}

	if (_player->_playerPosition->X < 0)
	{
		_player->_playerPosition->X = 0;
	}

	if (_player->_playerPosition->Y + _player->_playerSourceRect->Height > Graphics::GetViewportHeight())
	{
		_player->_playerPosition->Y = Graphics::GetViewportHeight() - _player->_playerSourceRect->Height;
	}

	if (_player->_playerPosition->Y < 0)
	{
		_player->_playerPosition->Y = 0;
	}
}

void TankGame::UpdatePlayer(int elapsedTime)
{
	//Checking if the player is moving.
	if (_player->_playerDirection == 0 || _player->_playerDirection == 2)
	{
		if (_player->_playerLastPosition->X > _player->_playerPosition->X || _player->_playerLastPosition->X < _player->_playerPosition->X)
		{
			_player->_playerIsMoving = true;
		}
		else
		{
			_player->_playerIsMoving = false;
		}
	}
	else
	{
		if (_player->_playerLastPosition->Y > _player->_playerPosition->Y || _player->_playerLastPosition->Y < _player->_playerPosition->Y)
		{
			_player->_playerIsMoving = true;
		}
		else
		{
			_player->_playerIsMoving = false;
		}
	}

	if (_player->_playerIsMoving)
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

			_player->_playerSourceRect->X = _player->_playerSourceRect->Width * _player->_playerFrame;
		}
	}

	//Changing player direction
	_player->_playerSourceRect->Y = _player->_playerSourceRect->Height * _player->_playerDirection;
}

void TankGame::UpdateAmmoPickups(int i,int elapsedTime)
{
	//Tracking time since last ammo animation frame change.
	_ammoPickup[i]->_ammoCurrentFrameTime += elapsedTime;

	//Updating the ammo frame.
	if (_ammoPickup[i]->_ammoCurrentFrameTime > _cAmmoFrameTime)
	{
		_ammoPickup[i]->_ammoFrameCount++;

		if (_ammoPickup[i]->_ammoFrameCount >= 2)
		{
			_ammoPickup[i]->_ammoFrameCount = 0;
		}

		_ammoPickup[i]->_ammoCurrentFrameTime = 0;
	}
	
}

void TankGame::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _player->_playerPosition->X << " Y: " << _player->_playerPosition->Y;
	
	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_player->_playerTexture, _player->_playerPosition, _player->_playerSourceRect); //Draws player.
	//for rotation and origin look into the other draw.
	SpriteBatch::Draw(_player->_playerTurretTexture, _player->_playerPosition, _player->_playerTurretSourceRect); //Draws player turret.

	for (int i = 0; i < AMMOPICKUPCOUNT; i++)
	{
		if (_ammoPickup[i]->_ammoFrameCount == 0)
		{
			// Draws Red Munchie
			SpriteBatch::Draw(_ammoPickup[i]->_ammoInvertedTexture, _ammoPickup[i]->position, _ammoPickup[i]->_ammoRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		}
		else
		{
			// Draw Blue Munchie
			SpriteBatch::Draw(_ammoPickup[i]->_ammoBlueTexture, _ammoPickup[i]->position, _ammoPickup[i]->_ammoRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			if (_ammoPickup[i]->_ammoFrameCount >= 60)
				_ammoPickup[i]->_ammoFrameCount = 0;
		}
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	
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