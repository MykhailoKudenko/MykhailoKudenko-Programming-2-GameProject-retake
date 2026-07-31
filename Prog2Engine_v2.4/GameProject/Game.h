#pragma once
#include "BaseGame.h"

#include "Player.h"
#include <vector>
#include "utils.h"

#include "Level.h"

#include "Camera.h"
#include "EntityManager.h"
#include "SoundManager.h"

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
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

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

	Camera* m_pCamera;
	Player* m_P1;
	Level* m_level1;
	EntityManager* m_pEntityManager;
	SoundManager* m_SoundManager;

	HUD* m_hud;

	GameState m_MyState{ GameState::DeathMenu };
	const int m_PlayerLivesMax = 3;
	int m_PLayerLivesCurrent = 3;

	Texture* m_MainMenu;
	Texture* m_DeathMenu;

	bool m_DebugShowColliders = false;
	int m_CameraScale = 4;
};