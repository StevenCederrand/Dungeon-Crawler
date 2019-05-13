#pragma once

union HitDescription 
{
	class GameObject* owner;
	class Player* player;
	class Walker* walker;
	class Shooter* shooter;
	class Boss* boss;
	class PowerUps* powerUp;
};