#pragma once
#include "BaseGame.h"

#include "Player.h"
#include <vector>
#include "utils.h"

#include "Level.h"

#include "Camera.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

#include "HUD.h"
class Game : public BaseGame 
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	//void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	//void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	//void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	//void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void LoadLevel1();
	void ResetLevel();
	void StartNewRun();
	void StartNextLife();

	enum class GameState
	{
		MainMenu,
		Playing,
		DeathMenu
	};

	EntityManager m_EntityManager;

	Camera m_Camera;
	Player m_Player;
	Level* m_pLevel;

	HUD m_pHud;

	GameState m_MyState;
	const int m_PlayerLivesMax;
	int m_PlayerLivesCurrent;

	const Texture* m_pMainMenu;
	const Texture* m_pDeathMenu;

	bool m_DebugShowColliders;
	int m_CameraScale;
};