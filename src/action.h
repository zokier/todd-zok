#ifndef ACTION_H
#define ACTION_H

typedef void (*Action_function)();

typedef struct Action Action;
struct Action {
	char hotkey; // hotkey == description[0]
	char *description_prefix;
	char *description;
	Action_function function;
};

#endif //ACTION_H
