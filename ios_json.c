#include "ios_json.h"
#include <Block.h>
#include <stdlib.h>
#include <string.h>

static void          parse_node(jnode *n, char **s);
static inline void   skip(char **s) { *s += strspn(*s, " \t\n\r,:"); }
static inline double parse_double(char **s) { return strtod(*s, s); }

static char *parse_str(char **s)
{
	char *start = ++(*s);
	char *end   = strchr(start, '"');
	*s          = end + 1;
	return strndup(start, end - start);
}

static void parse_obj(jnode *n, char **s, bool is_obj)
{
	n->len = 0;

	n->get = (get_jnode)Block_copy(^jnode *(const char *key) {
	  for (int i = 0; i < n->len; i++) {
		  if (strcmp(n->to.o[i]->key, key) == 0) {
			  return n->to.o[i];
		  }
	  }
	  return NULL;
	});

	while (s[0]++, skip(s), *s[0] != (is_obj ? '}' : ']')) {
		n->len++;
		n->to.o   = realloc(n->to.o, n->len * sizeof(jnode *));
		jnode *_n = malloc(sizeof(jnode));
		if (is_obj) {
			_n->key = parse_str(s);
			skip(s);
		}
		parse_node(_n, s);
		n->to.o[n->len - 1] = _n;
	}
}

// clang-format off
static void parse_node(jnode *n, char **s)
{
	switch (*s[0]) {
	case '{':  case '[':  
		   parse_obj(n, s, *s[0] == '{'); break;
	case '"':  n->to.s = parse_str(s); n->len = strlen(n->to.s); break;
	case 't':  s[0] += 4; n->to.b = true; break;
	case 'f':  s[0] += 5; n->to.b = false; break;
	case 'n':  s[0] += 4; break;
	default:   n->to.n = parse_double(s); break;
	}
}
// clang-format on

__attribute__((overloadable)) void free(jnode *n)
{
	if (n->get) Block_release(n->get);
	for (size_t i = 0; i < n->len; ++i) free(n->to.o[i]);
	free(n->to.o);
	free(n->to.s);
	free(n->key);
	free((void *)n);
}

__attribute__((overloadable)) jnode *parse(const char *json)
{
	char  *s = (char *)json;
	jnode *n = malloc(sizeof(jnode));
	parse_node(n, &s);
	return n;
}
