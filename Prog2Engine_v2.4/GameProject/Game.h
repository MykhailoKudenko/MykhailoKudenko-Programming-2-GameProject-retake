#pragma once
#include "BaseGame.h"

#include "Player.h"
#include <vector>
#include "utils.h"

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

	Player m_P1;

	std::vector<Vector2f> m_Vertices{
		Vector2f{ 0 , 94 } ,
		Vector2f{ 94 , 94 } ,
		Vector2f{ 94 , 38 } ,
		Vector2f{ 188 , 38 } ,
		Vector2f{ 188 , 65 } ,
		Vector2f{ 282 , 65 } ,
		Vector2f{ 282 , 97 } ,
		Vector2f{ 376 , 97 } ,
		Vector2f{ 376 , 43 } ,
		Vector2f{ 470 , 43 } ,
		Vector2f{ 470 , 114 } ,
		Vector2f{ 564 , 114 } ,
		Vector2f{ 564 , 7 } ,
		Vector2f{ 658 , 7 } ,
		Vector2f{ 658 , 92 } ,
		Vector2f{ 752 , 92 } ,
		Vector2f{ 752 , 118 } ,
		Vector2f{ 846 , 118 } ,
		Vector2f{ 846 , 25 } ,
		Vector2f{ 940 , 25 } ,
		Vector2f{ 846 , 0 } ,
		Vector2f{ 0 , 0 } ,
		Vector2f{ 0 , 94 }};
};