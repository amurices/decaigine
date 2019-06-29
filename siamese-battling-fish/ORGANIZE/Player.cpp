#include "Player.h"

// See header file

Player::Player()
{
}

Player::Player(b2World* gameWorld, const glm::vec2 &pos, const glm::vec4 &drawData, const glm::vec4 &wpnDrawData, GLint bTex, GLint wTex, WeaponType wpn, FishType fish)
{
	_gameWorld = gameWorld;
	_pos = pos;
	_drawUvDims = drawData;
	_wpDrawUvDims = wpnDrawData;
	_bTex = bTex;
	_wTex = wTex;
	_boost = 1;
	_wpnType = wpn;
	_fishType = fish;
	_dealingDmg = false;
	_invulnerabilityDuration = 0;
	_cooldownBetweenAttacks = 0;
}

Player::~Player()
{
}

void Player::init()
{
	float fishDensity; // gives different weights based on class


	// Set speed and hp factors based on class
	if (_fishType == FishType::HUNTERFISH)
	{
		_speed	= HUNTER_SPEED;
		_hp		= HUNTER_HP;
		fishDensity = 10.0f;
	}

	else if (_fishType == FishType::WIZARDFISH)
	{
		_speed	= WIZARD_SPEED;
		_hp		= WIZARD_HP;
		fishDensity = 1.0f;
	}

	else if (_fishType == FishType::WARRIORFISH)
	{
		_speed	= WARRIOR_SPEED;
		_hp		= WARRIOR_HP;
		fishDensity = 100.0f;

	}

	_bodyEntity._entityWorld = _gameWorld;
	_bodyEntity.setEntityBody(Denjine::EntityFlags::DYNAMIC, _pos);

	b2Vec2 BERtices0[3];	//	Fishes can have a pretty rough shape
	b2Vec2 BERtices1[4];	//	They'll be made of a triangle
	b2Vec2 BERtices2[5];	//	a quad and a pentagon

	BERtices0[0].Set(0.0, 0.0);
	BERtices0[1].Set(-50.0f, -10.0f);
	BERtices0[2].Set(-15.0f, 25.0f);

	BERtices1[0].Set(-15.0f, 25.0f);
	BERtices1[1].Set(-25.0f, 40.0f);
	BERtices1[2].Set(15.0f, 40.0f);
	BERtices1[3].Set(25.0f, 25.0f);

	BERtices2[0].Set(25.0f, 25.0f);
	BERtices2[1].Set(50.0f, 10.0f);
	BERtices2[2].Set(40.0f, -15.0f);
	BERtices2[3].Set(0.0f, -40.0f);
	BERtices2[4].Set(0.0f, 0.0f);

	_bodyEntity.createEntityFixture(Denjine::ShapeType::POLYGON, BERtices0, 3, fishDensity, 0.1f);
	_bodyEntity.createEntityFixture(Denjine::ShapeType::POLYGON, BERtices1, 4, fishDensity, 0.1f);
	_bodyEntity.createEntityFixture(Denjine::ShapeType::POLYGON, BERtices2, 5, fishDensity, 0.1f);


	_bodyEntity.setTexData(_drawUvDims, _bTex);

	// ------------------------------------------------------------------- Body done, now for the weapon
	_weaponEntity._entityWorld = _gameWorld;

	
	// ------------------------------------------------------------------- Define class-specific weapon collision details
	if (_wpnType == WeaponType::HUNTER)
	{
		_weaponEntity.setEntityBody(Denjine::EntityFlags::DYNAMIC + Denjine::EntityFlags::NO_GRAVITY, _pos + glm::vec2(-55.0f, 0.0f));

		b2Vec2 vertices[4];
		vertices[0].Set(0.0f, 15.0f);
		vertices[1].Set(0.0f, -15.0f);
		vertices[2].Set(27.0f, 27.0f);
		vertices[3].Set(-27.0f, -27.0f);

		_weaponEntity.createEntityFixture(Denjine::ShapeType::POLYGON, vertices, 4, 0.00001f, 0.1f);

		// Set the integer corresponding to wpn's attack
		_attack = HUNTER_ATTACK;
	}
	else if (_wpnType == WeaponType::WIZARD)
	{
		_weaponEntity.setEntityBody(Denjine::EntityFlags::DYNAMIC + Denjine::EntityFlags::NO_GRAVITY, _pos + glm::vec2(85.0f, 0.0f));

		b2Vec2 vertices[4];
		vertices[0].Set(0.0f, 10.0f);
		vertices[1].Set(0.0f, -10.0f);
		vertices[2].Set(10.0f, 0.0f);
		vertices[3].Set(-10.0f, 0.0f);

		_weaponEntity.createEntityFixture(Denjine::ShapeType::POLYGON, vertices, 4, 0.00001f, 0.1f);

		// Set the integer corresponding to wpn's attack
		_attack = WIZARD_ATTACK;
	}

	else if (_wpnType == WeaponType::WARRIOR)
	{
		_weaponEntity.setEntityBody(Denjine::EntityFlags::DYNAMIC, _pos + glm::vec2(85.0f, 0.0f));

		b2Vec2 vertices[4];
		vertices[0].Set(0.0f, 12.0f);
		vertices[1].Set(0.0f, -12.0f);
		vertices[2].Set(30.0f, 27.0f);
		vertices[3].Set(-30.0f, -27.0f);

		_weaponEntity.createEntityFixture(Denjine::ShapeType::POLYGON, vertices, 4, 1.0f, 0.1f); // warrior's weapon is heavier

		// Set the integer corresponding to wpn's attack
		_attack = WARRIOR_ATTACK;
	}
	_weaponEntity.setTexData(_drawUvDims, _wTex);
	// ------------------------------------------------------------------- Define weapon's joint
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = _bodyEntity._entityBody;
	jointDef.bodyB = _weaponEntity._entityBody;
	jointDef.localAnchorA = _bodyEntity._entityBody->GetLocalCenter();
	jointDef.localAnchorB = _weaponEntity._entityBody->GetPosition() - _bodyEntity._entityBody->GetPosition();

	// ------------------------------------------------------------------- Now tie the weapon to the character
	jointDef.enableMotor = true;
	jointDef.maxMotorTorque = MAX_MOTOR_TORQUE;
	_wpBodyJoint =(b2RevoluteJoint*)_gameWorld->CreateJoint(&jointDef);
}

void Player::move(glm::vec2 direction)
{
	b2Vec2 current;
	current.x = direction.x * _boost * 100 * _speed;
	current.y = direction.y * _boost * 100 * _speed;

	_bodyEntity._entityBody->SetLinearVelocity(current);

	// Now correct bodyEntity's angular velocity according to direction it's heading
	b2Vec2 linearVel = b2Vec2(direction.x, direction.y);
	float linVelAng = atan2(linearVel.y, linearVel.x);
	float ang = _bodyEntity._entityBody->GetAngle();

	_bodyEntity._entityBody->SetAngularVelocity(3 * (linVelAng - ang));

}


void Player::jump()
{
	_boost = 256 * 4;
}

void Player::rotate(float speed)
{
	_wpBodyJoint->SetMotorSpeed(speed);
}

void Player::pierce()
{
	_attackDuration = 10;
	if (_cooldownBetweenAttacks <= 0)
		_cooldownBetweenAttacks = 70;
}

void Player::slash()
{
	_attackDuration = 20;
	if (_cooldownBetweenAttacks <= 0)
		_cooldownBetweenAttacks = 90;
}

void Player::attack()
{
	if (_cooldownBetweenAttacks > 0)
	{
		return; // don't attack if still on cooldown
	}

	if (_wpnType == WeaponType::WIZARD)
	{
		return; // wizard doesn't attack this way
	}


	if (_wpnType == WeaponType::HUNTER)
	{
		if (_attackDuration <= 0)
		{
			_dealingDmg = false;
			return;
		}
		_attackDuration--;
	}


	if (_wpnType == WeaponType::WARRIOR)
	{
		if (_attackDuration <= 0)
		{
			_dealingDmg = false;
			return;
		}
		_attackDuration--;
		_weaponEntity._entityBody->SetAngularVelocity(300);
	}
}


void Player::turnInvulnerable()
{
	if (_fishType == FishType::HUNTERFISH)
	{
		_invulnerabilityDuration = HUNTER_INVULNERABLE;
	}
	if (_fishType == FishType::WIZARDFISH)
	{
		_invulnerabilityDuration = WIZARD_INVULNERABLE;
	}
	if (_fishType == FishType::WARRIORFISH)
	{
		_invulnerabilityDuration = WARRIOR_INVULNERABLE;
	}
}

void Player::update()
{
	if (_boost > 1)
		_boost *= SQRT_2;
	if (_invulnerabilityDuration > 0)
	{
		if (_swapTex == _bTex)
			_swapTex = _invTex;
		else
			_swapTex = _bTex;

		_invulnerabilityDuration--;
	}
	else
	{
		_swapTex = _bTex;
	}

	if (_cooldownBetweenAttacks > 0)
	{
		if (_swapWTex == _wTex)
			_swapWTex = _wTex;
		else
			_swapWTex = _cdTex;
		_cooldownBetweenAttacks--;
	}
	else
	{
		_swapWTex = _wTex;
	}
	_bodyEntity.update();	
	_weaponEntity.update();								// update
	_wpDrawPosDims	= &(_weaponEntity._drawPosDim);
	_wpDrawAngle	= _weaponEntity._drawAngle;			// the
	_drawAngle		= _bodyEntity._drawAngle;
	_drawPosDims	= &(_bodyEntity._drawPosDim);		// visuals
}

void Player::die()
{
	_swapTex = _invTex; // invulnerability texture can also be death texture
	init();
}