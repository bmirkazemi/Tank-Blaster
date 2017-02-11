//Enemy Manager will handle enemies and enemy behavior

class EnemyManager {

	private:
		int   enemyCount;
		Tank* tanks[12];
		void  move();
		void  init();
		

	public:
		void update();
		Node enemyNode;
		EnemyManager();

};

//Constructor initializes data memebrs
EnemyManager::EnemyManager() {

	enemyCount 	   = 0;
	enemyNode.name = "Enemy Node";

}

void EnemyManager::move() {

	//Create an enemy if not enough
	if (enemyCount < 10) {

		//Randomize location
		int x = 0 + (rand() % (int) (800 - 0 + 1));
		int y = 0 + (rand() % (int) (500 - 0 + 1));

		//Local Variable put on heap
		Tank* t = new Tank(Vec(x,y,0));

		//Add to list and render node
		tanks[enemyCount] = t;
		enemyNode.attachChild(*t);
		enemyCount++;

	}


	//Act on list of enemies
	for (int i = 0; i < enemyCount; i++) {

		tanks[i]->moveUp();
		
		if (tanks[i]->location.y > 600) {

			//Detach Child From Render Node
			enemyNode.detachChild(*tanks[i]);
			//Delete because tanks put on heap
			delete tanks[i];
			//Remove Tank from List
			tanks[i] = tanks[enemyCount-1];
			enemyCount--;

		}

	}

}

//Called on Loop
void EnemyManager::update() {

	move();

}

