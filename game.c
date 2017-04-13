#include "helper.c"
#include "drawHelper.c"
#include "vec.c"
#include "entity.c"
#include "room.c"

#define entity_nb 2

static uchar roomStart_map[25] = {
    201,205,205,205,187,
    186,'.','.','.',186,
    186,'.','.','.',186,
    186,'.','.','.',186,
    200,205,205,205,188
};
#define roomStart_nbEntity 1
static Entity roomStart_entity[roomStart_nbEntity];

static uchar roomUp_map[15] = {
	201,205,205,205,187,
	186,'.','.','.',186,
	200,205,205,205,188
};

static uchar roomLeft_map[15] = {
	201,205,187,
	186,'.',186,
	186,'.',186,
	186,'.',186,
	200,205,188
};

/*const char roomStart[50] = {
    218,196,'O',196,191,
    179,'.','.','.',179,
    'O','.','.','.','O',
    179,'.','.','.',179,
    192,196,'O',196,217,
    201,205,'O',205,187,
    186,'.','.','.',186,
    'O','.','.','.','O',
    186,'.','.','.',186,
    200,205,'O',205,188
};*/

static Room room_tab[5];
static uchar room_id = 0;

static Entity player;
static Vec player_posW;

static uchar mode = 0;

static Entity explorer;
static uchar drawInfo = 0;
static const char *info;

static Entity entity_tab[entity_nb];

static uchar affKey = 0;

void init(){
	Entity_set(&roomStart_entity[0], "rat", 'r', 1, 1, 0);
	Room_set(&room_tab[0], 5, 5, roomStart_map, roomStart_entity, roomStart_nbEntity);
	Room_setPortals(&room_tab[0], 2, 0, 1, 2, 4, 2, 4, 2, 3, 0, 2, 4);

	Room_set(&room_tab[1], 5, 3, roomUp_map, null, 0);
	Room_setSouthPortal(&room_tab[1], 2, 2, 0);

	Room_set(&room_tab[2], 5, 3, roomUp_map, null, 0);
	Room_setNorthPortal(&room_tab[2], 2, 0, 0);

	Room_set(&room_tab[3], 3, 5, roomLeft_map, null, 0);
	Room_setWestPortal(&room_tab[3], 0, 2, 0);

	Room_set(&room_tab[4], 3, 5, roomLeft_map, null, 0);
	Room_setEstPortal(&room_tab[4], 2, 2, 0);
	

	Entity_set(&player, "player", 1, 2, 2, 50);

	Entity_set(&explorer, "explorer", 'X', 0, 0, 0);
}

void checkCollRoom0(){
	if(Room_Vec_getCase(room_tab[room_id], player_posW)=='.'){
		Entity_Vec_setPos(&player, player_posW);
	}
}

void checkCollEntity0(){
	Entity *e = Room_Vec_getEntity(room_tab[room_id], player_posW);

	if(e != NULL){
		return;
	}

	checkCollRoom0();
}

void checkCollNorthPortal0(){
	if(Vec_equal(player_posW, room_tab[room_id].northPortal)){
		room_id = room_tab[room_id].northId;
		Entity_setPos(&player, room_tab[room_id].southPortal.x, room_tab[room_id].southPortal.y-1);
		return;
	}

	checkCollEntity0();
}

void checkCollSouthPortal0(){
	if(Vec_equal(player_posW, room_tab[room_id].southPortal)){
		room_id = room_tab[room_id].southId;
		Entity_setPos(&player, room_tab[room_id].northPortal.x, room_tab[room_id].northPortal.y+1);
		return;
	}

	checkCollEntity0();
}

void checkCollEstPortal0(){
	if(Vec_equal(player_posW, room_tab[room_id].estPortal)){
		room_id = room_tab[room_id].estId;
		Entity_setPos(&player, room_tab[room_id].westPortal.x+1, room_tab[room_id].westPortal.y);
		return;
	}

	checkCollEntity0();
}

void checkCollWestPortal0(){
	if(Vec_equal(player_posW, room_tab[room_id].westPortal)){
		room_id = room_tab[room_id].westId;
		Entity_setPos(&player, room_tab[room_id].estPortal.x-1, room_tab[room_id].estPortal.y);
		return;
	}

	checkCollEntity0();
}

void checkCollRoom1(){
	switch(Room_Vec_getCase(room_tab[room_id], explorer.pos)){
		case 186:
		case 205:
			info = "wall";
			drawInfo = 1;
		return;

		case 187:
		case 188:
		case 200:
		case 201:
			info = "corner wall";
			drawInfo = 1;
		return;

		case '.':
			info = "ground";
			drawInfo = 1;
		return;
	}
}

void checkCollEntity1(){
	Entity *e = Room_Vec_getEntity(room_tab[room_id], explorer.pos);

	if(e != NULL){
		info = (*e).name;
		drawInfo = 1;
		return;
	}

	if(Vec_equal(player.pos, explorer.pos)){
		info = player.name;
		drawInfo = 1;
		return;
	}

	checkCollRoom1();
}

void checkCollPortal1(){
	if(Vec_equal(explorer.pos, room_tab[room_id].northPortal)){
		info = "north Portal";
		drawInfo = 1;
		return;
	}

	if(Vec_equal(explorer.pos, room_tab[room_id].southPortal)){
		info = "south Portal";
		drawInfo = 1;
		return;
	}

	if(Vec_equal(explorer.pos, room_tab[room_id].estPortal)){
		info = "est Portal";
		drawInfo = 1;
		return;
	}

	if(Vec_equal(explorer.pos, room_tab[room_id].westPortal)){
		info = "west Portal";
		drawInfo = 1;
		return;
	}

	checkCollEntity1();
}

void onUp(){
	switch(mode){
		case 0:
			Vec_set(&player_posW, player.pos.x, player.pos.y-1);
			checkCollNorthPortal0();
		return;

		case 1:
			Entity_subY(&explorer, 1);
			checkCollPortal1();
		return;
	}
}

void onDown(){
	switch(mode){
		case 0:
			Vec_set(&player_posW, player.pos.x, player.pos.y+1);
			checkCollSouthPortal0();
		return;

		case 1:
			if(explorer.pos.y<room_tab[room_id].roomH-1){
				Entity_addY(&explorer, 1);
			}
			checkCollPortal1();
		return;
	}
}

void onLeft(){
	switch(mode){
		case 0:
			Vec_set(&player_posW, player.pos.x-1, player.pos.y);
			checkCollWestPortal0();
		return;

		case 1:
			Entity_subX(&explorer, 1);
			checkCollPortal1();
		return;
	}
}

void onRight(){
	switch(mode){
		case 0:
			Vec_set(&player_posW, player.pos.x+1, player.pos.y);
			checkCollEstPortal0();
		return;

		case 1:
			if(explorer.pos.x<room_tab[room_id].roomL-1){
				Entity_addX(&explorer, 1);
			}
			checkCollPortal1();
		return;
	}
}

void onHelp(){
	if(mode != 1){
		Entity_Vec_setPos(&explorer, player.pos);
		checkCollEntity1();
		mode = 1;
	}else{
		mode = 0;
	}
}

void onKey(uchar wParam){
	affKey = wParam;
}

void draw(HDC memHdc){
	unsigned int i,j;

	Room_draw(memHdc, room_tab[room_id]);

	switch(mode){
		case 0:
			Entity_draw(memHdc, player);

			drawString(memHdc, 300, 5, "Hp:");

			if(player.hp == 100){
				drawString(memHdc, 330, 5, "100");
			}else{
				i = player.hp / 10;

				drawChar(memHdc, 338, 5, '0'+i);
				drawChar(memHdc, 346, 5, '0'+(player.hp-10*i));
			}
			
			drawString(memHdc, 360, 5, "/ 100");

			break;

		case 1:
			Entity_draw(memHdc, player);

			Entity_draw(memHdc, explorer);

			if(drawInfo){
				TextOut(memHdc, 0, 0, "info: ", 6);
				drawString(memHdc, 6*8, 0, info);
				drawInfo = 0;
			}
			break;
	}

	if(affKey > 0){
		drawBin(memHdc, 0, 0, affKey);
		affKey = 0;
	}

	
	//ascii table
	/*for(j=0;j<16;j++){
		for(i=0;i<16;i++){
			drawChar(memHdc, i*10, j*13, j*16+i);
		}
	}*/
}