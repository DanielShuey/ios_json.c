#include "ios_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

thread_local jnode *_curnode;

static void parse_node(jnode *n, char **s);

// clang-format off
#define skip_cond(c) c==' '||c=='\t'||c =='\n'||c=='\r'||c ==','||c ==':'
static inline void skip(char **s) { while (skip_cond(*s[0])) s[0]++; }
// clang-format on

jnode *_json_get(jnode *n, const char *key)
{
	if (n == NULL) n = _curnode;

	for (int i = 0; i < n->len; i++) {
		if (strcmp(n->to.a[i]->key, key) == 0) {
			_curnode = n->to.a[i];
			return n->to.a[i];
		}
	}
	return NULL;
}

static char *parse_str(char **s)
{
	char *str = s[0] + 1;
	int   sz  = 0;
	char *res;
	while (str[sz] != '"')
		(str[sz] == '\\' && str[sz + 1]) ? sz += 2 : sz++;
	res = malloc(sz + 1);
	memcpy(res, str, sz);
	res[sz] = '\0';
	s[0] += sz + 2;
	return res;
}

static double parse_double(char **s) { return strtod(*s, s); }

static void parse_arr(jnode *n, char **s)
{
	n->len  = 0;
	n->type = jtype_obj;
	while (s[0]++, skip(s), *s[0] != ']') {
		n->len++;
		n->to.a = realloc(n->to.a, n->len * sizeof(jnode *));
		n->to.a[n->len - 1] = malloc(sizeof(jnode));
		parse_node(n->to.a[n->len - 1], s);
	}
}

static void parse_obj(jnode *n, char **s)
{
	n->len  = 0;
	n->type = jtype_obj;
	n->get  = &_json_get;
	while (s[0]++, skip(s), *s[0] != '}') {
		n->len++;
		n->to.o   = realloc(n->to.o, n->len * sizeof(jnode *));
		jnode *_n = malloc(sizeof(jnode));
		_n->key   = parse_str(s);
		skip(s);
		parse_node(_n, s);
		n->to.o[n->len - 1] = _n;
	}
}

// clang-format off
static void parse_node(jnode *n, char **s)
{
	switch (*s[0]) {
	case '{':  parse_obj(n, s); break;
	case '[':  parse_arr(n, s); break;
	case '"':  n->to.s = parse_str(s);   
		   n->len = strlen(n->to.s);
		   n->type = jtype_str;
		   break;
	case 't':  s[0] += 4; n->to.b = true; break;
	case 'f':  s[0] += 5; n->to.b = false; break;
	case 'n':  s[0] += 4; break;
	default:   n->to.n = parse_double(s); break;
	}
}

__attribute__((overloadable)) void json_free(jnode *n)
{
	switch (n->type) {
	case jtype_obj:
		for (size_t i = 0; i < n->len; i++) json_free(n->to.a[i]);
		free(n->to.a); break;
	case jtype_str: free(n->to.s); break;
	default: break;
	}
	free(n);
}
// clang-format on

__attribute__((overloadable)) jnode *parse(const char *json)
{
	char  *s = (char *)json;
	jnode *n = malloc(sizeof(jnode));
	parse_node(n, &s);
	return n;
}
