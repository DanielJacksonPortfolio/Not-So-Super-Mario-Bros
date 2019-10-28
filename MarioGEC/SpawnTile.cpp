#include "SpawnTile.h"

SpawnTile::SpawnTile(Coordi gridPosition,int enemyType, int collisionType)
{
	this->collisionType = collisionType;
	this->enemyType = enemyType;

 	this->gridPosition = gridPosition;
	this->worldPosition = Coordf(gridPosition.x * width + width * 0.5f, gridPosition.y * height + height * 0.5f);
	this->image = nullptr;
	this->hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };
}
//
//int SpawnTile::GetEnemyType()
//{
//	return enemyType;
//}

int SpawnTile::ResolveCollision(int direction)
{
	return Collisions::COLLISION_SPAWN;
}