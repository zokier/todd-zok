#ifndef ACTION_H
#define ACTION_H

// Action_function is pointer to void function taking Player* as parameter
typedef void (*Action_function)(void *);

typedef struct Action Action;
struct Action {
	char hotkey;
	char *description;
	Action_function function;
};

#endif //ACTION_H
