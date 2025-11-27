#pragma once
#include <Block.h>
#include <stdbool.h>
typedef struct jnode jnode;
typedef jnode * (^get_jnode)(const char *key);
typedef struct jnode {
	char     *key;
	int       len;
	get_jnode get;
	union {
		bool    b;
		double  n;
		char   *s;
		jnode **a;
		jnode **o;
	} to;
} jnode;
__attribute__((overloadable)) void   free(jnode *n);
__attribute__((overloadable)) jnode *parse(const char *json);
