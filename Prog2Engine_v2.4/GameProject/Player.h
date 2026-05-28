#pragma once
#include <vector>
#include "utils.h"

#include "Animation.h"

enum class PlayerState
{
	Standing,
	Walking,
	Jumping,
	Climbing,
	ClimbingStill,
	Knockback,
	Ducking,
	Throwing,
	DuckingThrow,
	KnockbackDead,
	Dead
};

enum class PlayerWeapon
{
	Lance,
	Knife,
	Tourch
};


class Player
{
public:
	Player(Vector2f Pos);
	~Player();
	//rule of 5
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, 
		const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
		const std::vector<Rectf>& ladders, 
		std::vector<std::vector<Vector2f>> platfroms); 

	Vector2f GetCenterPosition() const;
	Rectf GetHitbox() const;
	void TakeDamage();

	bool DoesWantToThrow() const;
	bool IsFacingRight() const;

	PlayerWeapon GetPlayerWeapon() const;
	void SetPlayerWeapon(PlayerWeapon weapon);

	int GetPlayerScore() const;
	void AddToPLayerScore(int Score);
	void SetPlayerScore(int NewScore);

	bool IsDeathAnimationFinished();

	void Respawn(const Vector2f& pos);

	void SetPos(const Vector2f& pos);

	void SetImmortal(bool isImmortal);

	bool IsImmortal() const;

	void SetFlying(bool isFlying);

	bool IsFlying() const;

private:
	void UpdateStates(const std::vector<Rectf>& ladders, float elapsedSec);
	void UpdateInput();
	void UpdateTimers(float elapsedSec);

	void UpdateMovmentHorisontal(const std::vector<std::vector<Vector2f>>& vertices,const std::vector<std::vector<Vector2f>>& playerOnlyVertices,float elapsedSec); 
	void UpdateMovmentVertical(const std::vector<std::vector<Vector2f>>& vertices,
		const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
		const std::vector<std::vector<Vector2f>>& platforms,
		float elapsedSec);

	bool TryClimb(const std::vector<Rectf>& ladders, bool isGoingUp);
	bool IsStillOnLadder() const;
	void SnapToCurrentLadderCenter();
	//movement(speeds)
	float m_Gravity{-60};
	float m_MovementSpeed{ 30 };
	float m_JumpSpeed{ 60 };
	float m_KnockBackSpeed{ 80 };
	float m_ClimbSpeed{ 40.f };

	//input
	int m_inputDirectionX{ 0 };
	int m_inputDirectionY{ 0 };
	bool m_IsShootButtonPressed{false};
	//POSITION
	bool m_isFacingRight = true;
	bool m_isOnTheGround = false;
	Rectf m_Collider;

	//jumping
	float m_JumpDirectionX{0.f};
	const float m_JumpTimeUpMax{ 0.3f };
	float m_JumpTimeUpCurrent{ 0.f };
	//KonkBack
	const float m_InvulnerableTimeMax{ 0.6f };
	float m_InvulnerableTimeCurrent{ 0.f };
	const float	m_KnockbackTimeMax{ 0.2f };
	float m_KnockbackTimeCurrent{ 0.f };
	float m_KnockBackDirectionX{ -1.f };
	//climbing 
	const Rectf* m_pCurrentLadder = nullptr;
	bool m_BlockVerticalActionsUntilReleased{ false };
	//Animations
	Animation m_WalkingArmour;
	Animation m_ClimbingArmour;
	Animation m_ThrowArmour;
	Animation m_DuckThrowArmour;

	Texture m_JumpingArmour;
	Texture m_DuckArmour;

	Animation m_WalkingNaked;
	Animation m_ClimbingNaked;
	Animation m_ThrowNaked;
	Animation m_DuckThrowNaked;

	Texture m_JumpingNaked;
	Texture m_DuckNaked;

	Texture m_Hit;

	Texture m_DeathKnockBack;
	Texture m_Death;

	//State machine
	bool m_IsWearingArmour = true;
	PlayerState m_Mystate{ PlayerState::Standing };

	//Throwing
	bool m_PreviousShootPressed{ false };
	bool m_DoesWantToThrow{ false };

	float m_ThrowCooldownMax{ 0.3f };
	float m_ThrowCooldownCurrent{ 0.0f };

	//wepon
	PlayerWeapon m_MyWeapon{ PlayerWeapon::Lance };
	//int Score
	float m_Score{ 0 };
	//Death
	bool m_IsImmortal{ false };
	const float m_DeathTimerMax{1.f};
	float m_DeathTimeCurrent{1.f};
	//DEBUG fly
	bool m_IsFlying{ false };
	float m_FlySpeed{ 120.f };
};

