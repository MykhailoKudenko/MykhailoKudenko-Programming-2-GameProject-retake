#pragma once
#include <vector>
#include "utils.h"

#include "Animation.h"




class Player final
{
public:

	enum class PlayerWeapon
	{
		Lance,
		Knife,
		Torch
	};

	Player(Vector2f startPos);
	~Player() = default;
	//rule of 5
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& vertices, 
		const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
		const std::vector<Rectf>& ladders, 
		const std::vector<std::vector<Vector2f>>& platfroms);

	Vector2f GetCenterPosition() const;
	Rectf GetHitbox() const;
	void TakeDamage();

	bool DoesWantToThrow() const;
	bool IsFacingRight() const;

	Player::PlayerWeapon GetPlayerWeapon() const;
	void SetPlayerWeapon(Player::PlayerWeapon weapon);

	int GetPlayerScore() const;
	void AddToPLayerScore(int score);
	void SetPlayerScore(int newScore);

	bool IsDeathAnimationFinished() const;

	void Respawn(const Vector2f& pos);

	void SetImmortal(bool isImmortal);

	bool IsImmortal() const;

	void SetFlying(bool isFlying);

	bool IsFlying() const;

	Vector2f GetThrowPosition() const;


private:

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

	void UpdateStates(const std::vector<Rectf>& ladders, float elapsedSec);
	void UpdateInput();
	void UpdateTimers(float elapsedSec);

	void UpdateMovementHorizontal(const std::vector<std::vector<Vector2f>>& vertices,const std::vector<std::vector<Vector2f>>& playerOnlyVertices,float elapsedSec); 
	void UpdateMovementVertical(const std::vector<std::vector<Vector2f>>& vertices,
		const std::vector<std::vector<Vector2f>>& playerOnlyVertices,
		const std::vector<std::vector<Vector2f>>& platforms,
		float elapsedSec);

	bool TryClimb(const std::vector<Rectf>& ladders, bool isGoingUp);
	bool IsStillOnLadder() const;
	void SnapToCurrentLadderCenter();
	//movement(speeds)
	float m_MovementSpeed{ 50 };
	float m_JumpSpeed{ 60 };
	float m_KnockBackSpeed{ 80 };
	float m_ClimbSpeed{ 60.f };

	//input
	int m_InputDirectionX{ 0 };
	int m_InputDirectionY{ 0 };
	bool m_IsShootButtonPressed{false};
	//POSITION
	bool m_IsFacingRight = true;
	bool m_IsOnTheGround = false;
	Rectf m_Collider;

	//jumping
	float m_JumpDirectionX{0.f};
	const float m_JumpTimeUpMax{ 0.3f };
	float m_JumpTimeUpCurrent{ 0.f };
	//KonkBack
	const float m_InvulnerableTimeMax{ 1.f };
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

	const Texture* m_JumpingArmour;
	const Texture* m_DuckArmour;

	Animation m_WalkingNaked;
	Animation m_ClimbingNaked;
	Animation m_ThrowNaked;
	Animation m_DuckThrowNaked;

	const Texture* m_JumpingNaked;
	const Texture* m_DuckNaked;

	const Texture* m_Hit;

	const Texture* m_DeathKnockBack;
	const Texture* m_Death;

	//State machine
	bool m_IsWearingArmour = true;
	PlayerState m_Mystate{ PlayerState::Standing };

	//Throwing
	bool m_PreviousShootPressed{ false };
	bool m_DoesWantToThrow{ false };

	float m_ThrowCooldownMax{ 0.2f };
	float m_ThrowCooldownCurrent{ 0.0f };

	//wepon
	PlayerWeapon m_MyWeapon{ PlayerWeapon::Lance };

	int m_Score{ 0 };
	//Death
	bool m_IsImmortal{ false };
	const float m_DeathTimerMax{1.f};
	float m_DeathTimeCurrent{1.f};
	//DEBUG fly
	bool m_IsFlying{ false };
	float m_FlySpeed{ 120.f };
};

