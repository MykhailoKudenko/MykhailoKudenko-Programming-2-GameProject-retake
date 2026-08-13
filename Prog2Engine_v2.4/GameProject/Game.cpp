#include "pch.h"
#include "Game.h"

#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window },
m_Camera{ utils::g_WindowSize },
m_pPlayer{ Vector2f{ 0,0 } },
m_pLevel{ nullptr },
m_pEntityManager{ },

m_pHud{},
m_MyState{ GameState::DeathMenu },
m_PlayerLivesMax{ 3 },
m_PLayerLivesCurrent{ 3 },

m_pMainMenu{ nullptr },
m_pDeathMenu{ nullptr },

m_DebugShowColliders{false},
m_CameraScale{4}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	m_pMainMenu = TextureManager::GetInstance().GetTexture("StartScreen.png");
	m_pDeathMenu = TextureManager::GetInstance().GetTexture("DeathMenu.png");
	m_pLevel = nullptr;

	m_PLayerLivesCurrent = m_PlayerLivesMax;
	m_MyState = GameState::MainMenu;

	SoundManager::GetInstance().SetMusicVolume(20);
	SoundManager::GetInstance().SetEffectVolume(20);
}

void Game::Cleanup()
{
	delete m_pLevel;
	m_pLevel = nullptr;

	SoundManager::GetInstance().StopMusic();
}

void Game::Update(float elapsedSec)
{
	switch (m_MyState)
	{
	case GameState::MainMenu:
		break;

	case GameState::Playing:
		// Always update player, so death animation/physics can continue
		m_pPlayer.Update(
			elapsedSec,
			m_pLevel->GetVertices(),
			m_pLevel->GetPlayerOnlyVertices(),
			m_pLevel->GetLadders(),
			m_pLevel->GetPlatformTopEdges()
		);

		
		m_pLevel->Update(elapsedSec);
		m_pEntityManager.Update(elapsedSec);
		m_pHud.Update(elapsedSec);
		
		if (m_pHud.DidTimerFinish())
		{
			SoundManager::GetInstance().StopMusic();
			m_MyState = GameState::MainMenu;
			m_pHud.ResetTimer();
		}

		if (m_pPlayer.IsDeathAnimationFinished() || m_pPlayer.GetCenterPosition().y < 0)
		{
			m_PLayerLivesCurrent -= 1;

			SoundManager::GetInstance().StopMusic();

			if (m_PLayerLivesCurrent > 0)
			{
				m_MyState = GameState::DeathMenu;
				SoundManager::GetInstance().PlayEffect(SoundManager::SFX::Death);

			}
			else
			{
				m_MyState = GameState::MainMenu;
			}
		}
		break;

	case GameState::DeathMenu:
		break;
	}
}
void Game::Draw() const
{
	ClearBackground();

	switch (m_MyState)
	{
	case GameState::MainMenu:
		if (m_pMainMenu != nullptr)
		{
			glPushMatrix();
			glScalef(m_CameraScale, m_CameraScale, 1.0f);
			m_pMainMenu->Draw(Vector2f{ 0.f, 0.f });
			glPopMatrix();

		}
		break;

	case GameState::Playing:
		m_Camera.Aim(
			m_pLevel->GetWidth(), m_pLevel->GetHeight(), 0, 20,
			Vector2f{ m_pPlayer.GetCenterPosition().x, 0 }, static_cast<float>(m_CameraScale)
		);

		m_pLevel->Draw(m_DebugShowColliders);
		m_pEntityManager.Draw(m_DebugShowColliders);
		m_pPlayer.Draw();

		m_Camera.Reset();
		glPushMatrix();
		glScalef(m_CameraScale, m_CameraScale, 1.0f);
		m_pHud.Draw(m_pPlayer.GetPlayerScore(), m_pPlayer.GetPlayerWeapon());
		glPopMatrix();

		break;

	case GameState::DeathMenu:
		if (m_pDeathMenu != nullptr)
		{
			glPushMatrix();
			glScalef(m_CameraScale, m_CameraScale, 1.0f);
			m_pDeathMenu->Draw(Vector2f{ 0.f, 0.f });
			glPopMatrix();
		}
		break;
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER)
	{
		switch (m_MyState)
		{
		case GameState::MainMenu:
			StartNewRun();
			SoundManager::GetInstance().PlayMusic();
			break;

		case GameState::Playing:
			break;

		case GameState::DeathMenu:
			SoundManager::GetInstance().PlayMusic();
			StartNextLife();
			break;

		}
	}
	//debug
	switch (e.keysym.sym)
	{
	case SDLK_F1:
		m_DebugShowColliders = !m_DebugShowColliders;
		if (m_DebugShowColliders)
		{
			std::cout << "DEBUG: COLLIDERS SHOW ON" << std::endl;

		}
		else
		{
			std::cout << "DEBUG: COLLIDERS SHOW OFF" << std::endl;

		}
		break;
	case SDLK_F2:
		
		m_pPlayer.SetImmortal(!m_pPlayer.IsImmortal());
		if (m_pPlayer.IsImmortal())
		{
			std::cout << "DEBUG: IMMORTALITY ON" << std::endl;
		}
		else
		{
			std::cout << "DEBUG: IMMORTALITY OFF" << std::endl;
		}
		break;
	case SDLK_F3:

		m_pPlayer.SetFlying(!m_pPlayer.IsFlying());
		if (m_pPlayer.IsFlying())
		{
			std::cout << "DEBUG: FLYING ON" << std::endl;

		}
		else
		{
			std::cout << "DEBUG: FLYING OFF" << std::endl;

		}
		break;
	case SDLK_F4:
		std::cout << m_pEntityManager << std::endl;
		break;
	case SDLK_F5:
		if (m_CameraScale < 10)
		{
			m_CameraScale += 1;
		}
		std::cout << "DEBUG: CAMERA SCALE = " << m_CameraScale << " (NORMAL SCALE IS 4)" << std::endl;
		break;

	case SDLK_F6:
		if (m_CameraScale > 1)
		{
			m_CameraScale -= 1;
		}
		std::cout << "DEBUG: CAMERA SCALE = " << m_CameraScale << " (NORMAL SCALE IS 4)" << std::endl;
		break;
	}
	
	
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//	break;

}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::LoadLevel1()
{
	if (m_pLevel != nullptr)
	{
		delete m_pLevel;
		m_pLevel = nullptr;
	}
	// make it readabl for a file svg (game technique)
	m_pLevel = new Level("Level1Data.txt");
}

void Game::ResetLevel()
{
	int savedScore = m_pPlayer.GetPlayerScore();

	m_pEntityManager.Reset();

	LoadLevel1();

	m_pPlayer.Respawn(m_pLevel->GetPlayerPosition());

	m_pPlayer.SetPlayerScore(savedScore);

	m_pHud.ResetTimer();

	m_pEntityManager.SetLevel(m_pLevel);
	m_pEntityManager.SetPlayer(&m_pPlayer);
	m_pEntityManager.SpawnPointEnemies();
}
void Game::StartNewRun()
{
	m_PLayerLivesCurrent = m_PlayerLivesMax;
	m_pPlayer.SetPlayerScore(0);
	ResetLevel();
	m_MyState = GameState::Playing;
	m_pHud.ResetTimer();
}

void Game::StartNextLife()
{
	ResetLevel();
	m_MyState = GameState::Playing;
}