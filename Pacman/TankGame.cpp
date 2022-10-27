#include "TankGame.h"

#include <sstream>

TankGame::TankGame(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.1f), _cPlayerFrameTime(100), _cAmmoFrameTime(500)
{
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Tank Game", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

TankGame::~TankGame() 
{
	delete _playerTexture;
	delete _playerTurretTexture;
	delete _playerSourceRect;
	delete _playerTurretSourceRect;
	delete _ammoBlueTexture;
	delete _ammoInvertedTexture;
	delete _ammoRect;
	delete _menuBackground;
}

void TankGame::LoadContent() 
{
	//Load Player
	_playerTexture = new Texture2D();
	_playerTexture->Load("Textures/TankBaseSheet.png", false);
	//_playerTurretTexture->Load("Textures/TankTurretSheet.png", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerLastPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_playerTurretSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_playerDirection = 0;
	_playerCurrentFrameTime = 0;
	_playerFrame = 0;
	_playerIsMoving = false;

	//Load Ammo
	_ammoBlueTexture = new Texture2D();
	_ammoBlueTexture->Load("Textures/Munchie.tga", true);
	_ammoInvertedTexture = new Texture2D();
	_ammoInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_ammoRect = new Rect(100.0f, 450.0f, 12, 12);
	_ammoCurrentFrameTime = 0;
	_ammoFrameCount = 0;

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
	
	//Checks for pause input.
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_escKeyDown && !_startGameMenu)
	{
		_escKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE))
		_escKeyDown = false;

	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && _startGameMenu)
	{
		_startGameMenu = false;
		_paused = !_paused;
	}
	
	//For things that need to stop when paused.
	if (!_paused)
	{
		_playerLastPosition->X = _playerPosition->X;
		_playerLastPosition->Y = _playerPosition->Y;
		
		//Checks for WASD and moves player.
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_playerPosition->X += _cPlayerSpeed * elapsedTime;
			_playerDirection = 0;
		}	
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_playerPosition->X -= _cPlayerSpeed * elapsedTime;
			_playerDirection = 2;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_playerPosition->Y += _cPlayerSpeed * elapsedTime;
			_playerDirection = 1;
		}	
		else if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_playerPosition->Y -= _cPlayerSpeed * elapsedTime;
			_playerDirection = 3;
		}

		//Checking if the player is moving.
		if (_playerDirection == 0 || _playerDirection == 2)
		{
			if (_playerLastPosition->X > _playerPosition->X || _playerLastPosition->X < _playerPosition->X)
			{
				_playerIsMoving = true;
			}
			else
			{
				_playerIsMoving = false;
			}
		}
		else
		{
			if (_playerLastPosition->Y > _playerPosition->Y || _playerLastPosition->Y < _playerPosition->Y)
			{
				_playerIsMoving = true;
			}
			else
			{
				_playerIsMoving = false;
			}
		}

		if (_playerIsMoving)
		{
			//Tracking time since last player animation frame change.
			_playerCurrentFrameTime += elapsedTime;

			//Updating the x column of the animation.
			if (_playerCurrentFrameTime > _cPlayerFrameTime)
			{
				_playerFrame++;

				if (_playerFrame >= 4)
				{
					_playerFrame = 0;
				}

				_playerCurrentFrameTime = 0;

				_playerSourceRect->X = _playerSourceRect->Width * _playerFrame;
			}
		}

		//Changing player direction
		_playerSourceRect->Y = _playerSourceRect->Height * _playerDirection;
		
		//Tracking time since last ammo animation frame change.
		_ammoCurrentFrameTime += elapsedTime;

		//Updating the ammo frame.
		if (_ammoCurrentFrameTime > _cAmmoFrameTime)
		{
			_ammoFrameCount++;

			if (_ammoFrameCount >= 2)
			{
				_ammoFrameCount = 0;
			}

			_ammoCurrentFrameTime = 0;
		}
			

		//Colliding with the walls.
		if (_playerPosition->X + _playerSourceRect->Width > Graphics::GetViewportWidth())
		{
			_playerPosition->X = Graphics::GetViewportWidth() - _playerSourceRect->Width;
		}

		if (_playerPosition->X < 0)
		{
			_playerPosition->X = 0;
		}

		if (_playerPosition->Y + _playerSourceRect->Height > Graphics::GetViewportHeight())
		{
			_playerPosition->Y = Graphics::GetViewportHeight() - _playerSourceRect->Height;
		}

		if (_playerPosition->Y < 0)
		{
			_playerPosition->Y = 0;
		}
	}
}

void TankGame::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _playerPosition->X << " Y: " << _playerPosition->Y;
	
	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); //Draws player.
	//SpriteBatch::Draw(_playerTurretTexture, _playerPosition, _playerTurretSourceRect); //Draws player turret.

	if (_ammoFrameCount == 0)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_ammoInvertedTexture, _ammoRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_ammoBlueTexture, _ammoRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		if (_ammoFrameCount >= 60)
			_ammoFrameCount = 0;
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