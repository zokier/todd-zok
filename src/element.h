#ifndef ELEMENT_H
#define ELEMENT_H

typedef enum Element Element;
enum Element
{
	ELEM_WOOD = 0,
	ELEM_FIRE,
	ELEM_EARTH,
	ELEM_METAL,
	ELEM_WATER,
	ELEM_COUNT
};
extern const char *element_names[ELEM_COUNT];

#endif //ELEMENT_H
