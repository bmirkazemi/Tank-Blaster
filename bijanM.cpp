//Name: Bijan Mirkazemi
//Description: creating an enemy tower object
//Course: cmps 3350 Software Engineering
//Constructor initializes data memebrs
FriendlyManager::FriendlyManager() {

	carCount 	 = 0;
	carNode.name = "Car Node";
	lastCar		 = time(0);
	initRoad();

}

void FriendlyManager::initRoad() {

	Shape *asphault = new Shape();

	asphault->height = WINDOW_HEIGHT*.15;
	asphault->width  = WINDOW_WIDTH;

	streetNode.location.y = WINDOW_HEIGHT - asphault->height/2 - WINDOW_HEIGHT/10;
	streetNode.location.x = WINDOW_WIDTH  - asphault->width/2;

	streetNode.attachChild(*asphault);
	asphault->color = Vec(111,110,99);

	for (int i = 0; i < 10; i++) {

		Shape *stripe 	   = new Shape();
		stripe->color      = yellow;
		stripe->height     = asphault->height/20;
		stripe->width      = asphault->width/15;
		stripe->location.y = 0;
		stripe->location.x = (-asphault->width/2) + (asphault->width/20) + ( (asphault->width/10)*i);
		streetNode.attachChild(*stripe);
	
	}

	//streetNode.printTree();

}

void FriendlyManager::move() {


	//Act on list of cars
	for (int i = 0; i < carCount; i++) {

		cars[i]->behavior->behave(*cars[i]->model);
		float xDist = cars[i]->model->location.x;
		if (xDist < 0) xDist *= -1;

		if (xDist > WINDOW_WIDTH+30 || cars[i]->health <= 0) {

			//std::cout << "Removing Car" << std::endl;
			//Detach Child From Render Node
			carNode.detachChild(*cars[i]->model);
			//Delete because tanks put on heap
			delete cars[i]->model;
			delete cars[i]->behavior;
			delete cars[i];
			//Remove Car from List
			cars[i] = cars[carCount-1];
			carCount--;

		}

	}

}

void FriendlyManager::carCreateCheck() {

	if (time(0) - lastCar < 2) return;

	//Create a car if not enough
	if (carCount < 10) {
		lastCar = time(0);

		bool skip = 0 + (rand() % (int) (1 - 0 + 1));

		if (skip)
			return;

		bool left = 0 + (rand() % (int) (1 - 0 + 1));

		int x;
		int y;
		//Randomize location
		if (left) {
			x = WINDOW_WIDTH;
			y = WINDOW_HEIGHT*.87;
		}
		
		else {
			x = 0;
			y = WINDOW_HEIGHT*.78;
		}

		//Local Variable put on heap
		Car* 		  c = new Car(Vec(x,y,0));
		CarBehavior*  b = new CarBehavior(left);
		CarFriend* 	  f = new CarFriend(*c,*b);

		int t = 1 + (rand() % (int) (3 - 1 + 3));
		Vec color;
		switch(t) {
			case (1): color = red; break;
			case (2): color = yellow; break;
			case (3): color = green; break;
			case (4): color = blue; break;
			default:  color = pink;
		}
		c->body.color  = color;
		c->front.color = color;
		c->back.color = color;

		//Add to list and render node
		cars[carCount] = f;
		carNode.attachChild(*c);
		carCount++;

	}

}

//Called on Loop
void FriendlyManager::update() {

	carCreateCheck();
	move();

}


class Tower: public Node {
    private: 
	char dir;
    public:
	Shape c1,c2,c3,c4;
	Shape body, roof, gun, gunbase;
	Tower();
	Tower(Vec loc);
};

Tower::Tower(Vec loc) {
    
    Vec black(0,0,0);
    Vec lightgray(70,70,70);
    Vec gray(50,50,50);
    Vec green(90,140,90);

    body.height = 40;
    body.width = 40;
    body.color = green;
    
    roof.height = 20;
    roof.width = 20;
    roof.color = lightgray;

    gun.color = black;
    gun.width = 18;
    gun.height = 4;

    gunbase.color = black;
    gunbase.width = 8;
    gunbase.height = 8;

	c1.color  = lightgray;
	c1.width  = 6;
	c1.height = 6;
    
	c2.color  = lightgray;
	c2.width  = 6;
	c2.height = 6;
    
	c3.color  = lightgray;
	c3.width  = 6;
	c3.height = 6;

	c4.color  = lightgray;
	c4.width  = 6;
	c4.height = 6;

	body.location = Vec(0,0,0);
	c1.location = Vec(14,18,0);    
	c2.location = Vec(14,-18,0);    
	c3.location = Vec(-14,18,0);    
	c4.location = Vec(-14,-18,0);    
	roof.location = Vec(0,0,0);
    gun.location = Vec(10,0,0);
    gunbase.location = Vec(0,0,0);


    body.name = "Body";
	c1.name = "Corner 1";
	c2.name = "Corner 2";
	c3.name = "Corner 3";
	c4.name = "Corner 4";
    roof.name = "Roof";
    gun.name = "Gun";
    gunbase.name = "Gun Base";
    name = "Tower";

    attachChild(body);
	attachChild(c1);
	attachChild(c2);
	attachChild(c3);
	attachChild(c4);
    attachChild(roof);
    attachChild(gun);
    attachChild(gunbase);


    angle = 0;
    location = loc;
}

struct TowerBehavior : public Behavior {
	TowerBehavior(int min, int max);
	int minimum, maximum;
	bool goingup, init;
	void behave(Node &model);
};

TowerBehavior::TowerBehavior(int min, int max) {
	goingup = false;
	init = false;
	maximum = max; 
	minimum = min;
}

void TowerBehavior::behave(Node &model) {

	Tower &tower = (Tower&) model;
	if(!init) {
		tower.gun.angle = minimum + 1;
	init = true;
	}
	if(goingup) 
		tower.gun.angle += 0.5;
	else 
		tower.gun.angle -= 0.5;
	if(tower.gun.angle > maximum || tower.gun.angle < minimum) 
		goingup = !goingup;
}


//Tanks without starting points are not tanks
Tank::Tank(){}

//Construct Tanks with a starting point
Tank::Tank(Vec loc) {

	//set up color vectors
	Vec black(0,0,0);
	Vec gray(50,50,50);
	Vec lightgray(80,80,80);
	Vec green(90,140,90);

	//Body Dimensions
	body.width     = 24;
	body.height    = 16;

	//Body is the tank body
	body.location  = Vec(0,0,0);

	//turret dimensions
	turret.width   = 14;
	turret.height  = 10;

	//set turret color
	turret.color   = lightgray;

	turret.location = Vec(0,0,0);
	
	//Set body color
	body.color     = green;

	//set right wheel dimensions
	rw.width       = 28;
	rw.height      = 4;

	//set left wheel dimensions	
	lw.width       = 28;
	lw.height      = 4;

	//set wheel color
	lw.color       = gray;
	rw.color       = gray;
	
	//wheels are at (12,0) from 
	//center of tank
	lw.location    = Vec(0,-9,0);
	rw.location    = Vec(0, 9,0);

	//Set Hatch Dimensions
	hatch.width    = 6;
	hatch.height   = 6;

	//Hatch is at CENTER OF TANK
	hatch.location = Vec(0,0,0);

	//Set Hatch Color
	hatch.color    = green;

	//Set Gun Dimensions
	gun.width      = 14;
	gun.height     = 3;

	//Gun is located at (0,12) from
	//Center of Tank
	gun.location   = Vec(12,0,0);
	
	//Set gun color
	gun.color      = black;

	//Set Names of Tank
	body.name  = "Body";
	hatch.name = "Hatch";
	gun.name   = "Gun";
	turret.name = "Turret";
	lw.name    = "LWheel";
	rw.name    = "RWheel";
	name 	   = "Tank";

	//ATTACH CHILDREN TO TANK
	attachChild(lw);
	attachChild(rw);

	attachChild(body);
	attachChild(turret);
	attachChild(hatch);
	attachChild(gun);

	//Tank starts facing upward
	angle		   =   90;
	dir	           = 'r';
	location 	   = loc;

	moveSpeed = .005;

}


Bullet Tank::shoot() {

	//std::cout << "Shoot\n";
	Bullet b;
	Vec    spot;
	b.body.width  = 5;
	b.body.height = 5;
	b.dir		  = dir;

	switch(dir) {
		
		case 'u':
			spot.x =  0;	
			spot.y = 12;
			break;
		case 'd':
			spot.x =   0;	
			spot.y = -12;
			break;
		case 'l':
			spot.x = -12;	
			spot.y =   0;
			break;
		case 'r':
			spot.x =  12;	
			spot.y =   0;
			break;

		case 'q':
			spot.x = -12/(sqrt(2)/2);	
			spot.y =  12/(sqrt(2)/2);
			break;
		case 'p':
			spot.x =  12/(sqrt(2)/2);	
			spot.y =  12/(sqrt(2)/2);
			break;
		case 'z':
			spot.x = -12/(sqrt(2)/2);	
			spot.y = -12/(sqrt(2)/2);
			break;
		case 'm':
			spot.x =   12/ (sqrt(2)/2);	
			spot.y =  -12/(sqrt(2)/2);
			break;

	}

	spot.x 		 += location.x;
	spot.y 		 += location.y;
	b.body.location = spot;
	
	return b;

}

//Directions are a char
//u d l r
//p q z m
//p q z m represent diagnals
//I chose these because they
//are the corner of the 
//keyboard

//The Following Functions
//are used by the player
//as well as the tank
//behaviors

void Tank::moveUp() {
	//Add to y if moving up
	location.y   += moveSpeed;
	//Set angle and direction
	dir 		  = 'u';
	angle		  =  90;
}

void Tank::moveDown() {
	//Subtract y if moving up
	location.y   -= moveSpeed;
	//Set angle and direction
	dir 		  = 'd';
	angle		  =  270;
}

void Tank::moveLeft() {
	//Subtract x if moving left
	location.x     -= moveSpeed;
	//Set angle and direction
	dir 		    = 'l';
	angle		    =  180;
}

void Tank::moveRight() {
	//Add x if moving right
	location.x   += moveSpeed;
	//Set angle and direction
	dir 		  = 'r';
	angle		  =   0;
}

void Tank::moveUpRight() {
	//Move up and right
	//Divide by 2 because
	//Both directions are
	//Being added to
	location.y   += moveSpeed/2;
	location.x   += moveSpeed/2;
	//Set angle and direction
	dir 		  = 'p';
	angle		  = 45;
}
void Tank::moveUpLeft() {
	//Move up and left
	//Divide by 2 because
	//Both directions are
	//Being added to
	location.y   += moveSpeed/2;
	location.x   -= moveSpeed/2;
	//Set angle and direction
	dir 		  = 'q';
	angle		  =  135;
}
void Tank::moveDownRight() {
	//Move down and right
	//Divide by 2 because
	//Both directions are
	//Being added to
	location.y   -= moveSpeed/2;
	location.x   += moveSpeed/2;
	//Set angle and direction
	dir 		  = 'm';
	angle		  =  315;
}
void Tank::moveDownLeft() {
	//Move down and left
	//Divide by 2 because
	//Both directions are
	//Being added to
	location.y   -= moveSpeed/2;
	location.x   -= moveSpeed/2;
	//Set angle and direction
	dir 		  =  'z';
	angle		  =  225;
}
