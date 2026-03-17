#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }, m_P1(Rectf{50, 150, 20, 20})
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	std::vector<Vector2f> m_Ground
	{
		Vector2f{ 0 , 94 },
		Vector2f{ 94 , 94 },
		Vector2f{ 94 , 38 },
		Vector2f{ 188 , 38 },
		Vector2f{ 188 , 65 },
		Vector2f{ 282 , 65 },
		Vector2f{ 282 , 97 },
		Vector2f{ 376 , 97 },
		Vector2f{ 376 , 43 },
		Vector2f{ 470 , 43 },
		Vector2f{ 470 , 114 },
		Vector2f{ 564 , 114 },
		Vector2f{ 564 , 7 },
		Vector2f{ 658 , 7 },
		Vector2f{ 658 , 92 },
		Vector2f{ 752 , 92 },
		Vector2f{ 752 , 118 },
		Vector2f{ 846 , 118 },
		Vector2f{ 846 , 25 },
		Vector2f{ 940 , 25 },
		Vector2f{ 846 , 0 },
		Vector2f{ 0 , 0 },
		Vector2f{ 0 , 94 }
	};

	std::vector<Vector2f> m_Platform
	{
		Vector2f{150, 200},
		Vector2f{150, 250},
		Vector2f{250, 250},
		Vector2f{250, 200},
		Vector2f{150, 200}
	};

	m_Vertices.push_back(m_Ground);
	m_Vertices.push_back(m_Platform);
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_P1.Update(elapsedSec, m_Vertices);
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_P1.Draw();
	for (const std::vector<Vector2f>& platform : m_Vertices)
	{
		for (size_t i = 0; i < platform.size() - 1; ++i)
		{
			utils::DrawLine(platform[i], platform[i + 1]);
		}
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
