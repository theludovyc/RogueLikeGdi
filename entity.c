struct Entity{
	const char *name;
	unsigned char symbol;
	Vec pos;
	unsigned char hp;
};
typedef struct Entity Entity;

void Entity_set(Entity *e, const char *n, uchar s, uchar x, uchar y, uchar hp){
	(*e).name = n;
	(*e).symbol = s;
	Vec_set(&(*e).pos, x, y);
	(*e).hp = hp;
}

void Entity_setPos(Entity *e, unsigned char x, unsigned char y){
	Vec_set(&(*e).pos, x, y);
}

void Entity_Vec_setPos(Entity *e, Vec v){
	(*e).pos = v;
}

void Entity_addX(Entity *e, unsigned char x){
	return Vec_addX(&(*e).pos, x);
}

void Entity_addY(Entity *e, unsigned char y){
	return Vec_addY(&(*e).pos, y);
}

void Entity_subX(Entity *e, unsigned char x){
	return Vec_subX(&(*e).pos, x);
}

void Entity_subY(Entity *e, unsigned char y){
	return Vec_subY(&(*e).pos, y);
}

void Entity_draw(HDC memHdc, Entity e){
	drawChar(memHdc,e.pos.x*9+10,e.pos.y*13+13,e.symbol);
}