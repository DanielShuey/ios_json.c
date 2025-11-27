#pragma once
#include <stdbool.h>
typedef enum jtype {
	jtype_null,
	jtype_obj,
	jtype_str,
} jtype;
typedef struct jnode jnode;
typedef struct jnode {
	char *key;
	jtype type;
	int   len;
	jnode *(*get)(jnode *n, const char *key);
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
