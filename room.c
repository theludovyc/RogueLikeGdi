struct Room{
	const char *roomM;
	uchar roomL;
	uchar roomH;
	Entity *roomE;
	uchar roomNbE;
	Vec northPortal;
	Vec southPortal;
	Vec estPortal;
	Vec westPortal;
	uchar northId;
	uchar southId;
	uchar estId;
	uchar westId;
};
typedef struct Room Room;

void Room_set(Room *r, uchar rL, uchar rH, const char *rM, Entity *rE, uchar rNbE){
	(*r).roomL = rL;
	(*r).roomH = rH;
	(*r).roomM = rM;
	(*r).roomE = rE;
	(*r).roomNbE = rNbE;
}

void Room_setPortals(Room *r, uchar x, uchar y, uchar id, uchar x1, uchar y1, uchar id1, uchar x2, uchar y2, uchar id2, uchar x3, uchar y3, uchar id3){
	Vec_set(&(*r).northPortal, x, y);
	(*r).northId = id;
	Vec_set(&(*r).southPortal, x1, y1);
	(*r).southId = id1;
	Vec_set(&(*r).estPortal, x2, y2);
	(*r).estId = id2;
	Vec_set(&(*r).westPortal, x3, y3);
	(*r).westId = id3;
}

void Room_setNorthPortal(Room *r, uchar x, uchar y, uchar id){
	Vec_set(&(*r).northPortal, x, y);
	(*r).northId = id;
}

void Room_setSouthPortal(Room *r, uchar x, uchar y, uchar id){
	Vec_set(&(*r).southPortal, x, y);
	(*r).southId = id;
}

void Room_setEstPortal(Room *r, uchar x, uchar y, uchar id){
	Vec_set(&(*r).estPortal, x, y);
	(*r).estId = id;
}

void Room_setWestPortal(Room *r, uchar x, uchar y, uchar id){
	Vec_set(&(*r).westPortal, x, y);
	(*r).westId = id;
}

uchar Room_getCase(Room r, uchar x, uchar y){
	return r.roomM[(y*r.roomL)+x];
}

uchar Room_Vec_getCase(Room r, Vec v){
	return r.roomM[(v.y*r.roomL)+v.x];
}

Entity *Room_Vec_getEntity(Room r, Vec v){
	unsigned int i;

	for(i=0; i<r.roomNbE; i++){
		if(Vec_equal(r.roomE[i].pos, v)){
			return &r.roomE[i];
		}
	}

	return (Entity *)NULL;
}

void Room_drawCase(HDC memHdc, uchar i, uchar j, uchar c){
	drawChar(memHdc, (i*9)+10, (j*13)+13, c);
}

void Room_drawPortal(HDC memHdc, Vec portal){
	if(portal.x != 0 || portal.y != 0){
		Room_drawCase(memHdc, portal.x, portal.y, 'O');
	}
}

void Room_draw(HDC memHdc, Room r){
	uchar i,j;

	for(j=0;j<r.roomH;j++){
        for(i=0;i<r.roomL;i++){
            Room_drawCase(memHdc, i, j, r.roomM[(j*r.roomL)+i]);
        }
    }

    Room_drawPortal(memHdc, r.northPortal);
    Room_drawPortal(memHdc, r.southPortal);
    Room_drawPortal(memHdc, r.estPortal);
    Room_drawPortal(memHdc, r.westPortal);

	for(i=0; i<r.roomNbE; i++){
		Entity_draw(memHdc, r.roomE[i]);
	}
}