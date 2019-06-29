#pragma once

#include "Denjine/Entity.h"

const float DRAG_VELOCITY_FACTOR = 0.95f;

const int	HUNTER_ATTACK = 40;
const int	WARRIOR_ATTACK = 55;
const int	WIZARD_ATTACK = 28;

const int	HUNTER_HP = 90;
const int	WARRIOR_HP = 125;
const int	WIZARD_HP = 70;

const float	HUNTER_SPEED = 1.2f;
const float WARRIOR_SPEED = 0.9f;
const float WIZARD_SPEED = 1.4f;

const int HUNTER_INVULNERABLE = 100;
const int WARRIOR_INVULNERABLE = 72;
const int WIZARD_INVULNERABLE = 128;


const float MAX_MOTOR_TORQUE = 100000.0f;
const float MOTOR_SPEED_SLOWEST = 0.5f;
const float MOTOR_SPEED_SLOW = 1.2f;
const float MOTOR_SPEED_FASTEST = 7.0f;

enum WeaponType{ WIZARD, HUNTER, WARRIOR };
enum FishType{ WIZARDFISH, HUNTERFISH, WARRIORFISH };


class Player
{
public:
	Player();
	Player(b2World* gameWorld, const glm::vec2 &pos, const glm::vec4 &drawData, const glm::vec4 &wpnDrawData, GLint bTex, GLint wTex, WeaponType wpn, FishType fish);
	~Player();
	

	void init();

	void	move(glm::vec2 direction);	
	void	rotate(float speed);	// Wizard's attack centers around rotating
	void	pierce();				// Hunter's attack stays still, but he can also rotate blade
	void	slash();				// Warrior's attack covers a wide, quick area, but he cannot control rotation
	void	attack();				// performs attack move
	void	turnInvulnerable();		// depends on class
	void	jump();	
	void	update();
	void	die();


	WeaponType		_wpnType;
	FishType		_fishType;

	b2World*			_gameWorld;
	b2RevoluteJoint*	_wpBodyJoint;
	glm::vec2			_pos;	// pos of the duo is all that matters

	glm::vec4		_drawUvDims;
	glm::vec4		_wpDrawUvDims;

	GLint			_bTex;
	GLint			_invTex;
	GLint			_swapTex; // auxiliary

	GLint			_wTex;
	GLint			_cdTex;
	GLint			_swapWTex;
	float			_drawAngle;
	float			_wpDrawAngle;
	glm::vec4*		_drawPosDims; // we won't account for color. we want our game objects to be textured
	glm::vec4*		_wpDrawPosDims; 

	int			_hp;		// damage necessary to die
	int			_attack;	// damage dealt per hit
	float		_speed;		// speed factor

	int			_invulnerabilityDuration;
	int			_attackDuration;
	int			_cooldownBetweenAttacks;
	float		_boost;
	bool		_dealingDmg;

	int _DEATHCOUNTER;

	Denjine::Entity _bodyEntity;	// treating them as separate entities rather
	Denjine::Entity _weaponEntity;	// than separate fixtures. body takes damage, weapon deals it

};

