#include <iostream>
#include "MainGame.h"
#include <GameEngine3D\QuadTree.h>
#include <GameEngine3D\Wall.h>

int main(int, char**)
{

	
	/*GameEngine3D::Wall* wallA = new GameEngine3D::Wall();
	GameEngine3D::Wall* wallB = new GameEngine3D::Wall();
	GameEngine3D::Wall* wallC = new GameEngine3D::Wall();
	wallA->placeCube(0, 0, 0);
	wallB->placeCube(2, 2, 0);
	wallC->placeCube(4, 4, 0);


	GameEngine3D::AABB qbound;
	qbound.center.x = 0;
	qbound.center.y = 0;
	qbound.halfSize.x = 20;
	qbound.halfSize.y = 20;


	GameEngine3D::QuadTree<GameEngine3D::Wall>* quadTree = new GameEngine3D::QuadTree<GameEngine3D::Wall>(qbound);

	GameEngine3D::Point p = { 0, 0 };
	GameEngine3D::Data<GameEngine3D::Wall> d1;
	d1.pos = p;
	d1.load = wallA;
	quadTree->insert(d1);

	p = { 2, 2 };
	d1.pos = p;
	d1.load = wallB;
	quadTree->insert(d1);

	p = { 4, 4 };
	d1.pos = p;
	d1.load = wallC;
	quadTree->insert(d1);

	GameEngine3D::AABB aabb;
	aabb.center.x = 3;
	aabb.center.y = 3;
	aabb.halfSize.x = 1.1;
	aabb.halfSize.y = 1.1;

	std::vector<GameEngine3D::Data<GameEngine3D::Wall>> res = quadTree->queryRange(aabb);
	*/
	MainGame mainGame;
	mainGame.run();

	return 0;
}