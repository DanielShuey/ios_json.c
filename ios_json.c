#include "ios_json.h"
#include <Block.h>
#include <stdlib.h>
#include <string.h>

static void          parse_node(jnode *n, char **s);
static inline void   skip(char **s) { *s += strspn(*s, " \t\n\r,:"); }
static inline double parse_double(char **s) { return strtod(*s, s); }

#define jnode_get(n)                                                           \
	Block_copy(^jnode *(const char *k) {                                   \
	  for (int i = 0; i < n->len; i++)                                     \
		  if (!strcmp(n->to.o[i]->key, k)) return n->to.o[i];          \
	  return NULL;                                                         \
	})

static char *parse_str(char **s)
{
	char *start = ++(*s);
	char *end   = strchr(start, '"');
	*s          = end + 1;
	return strndup(start, end - start);
}

static void parse_obj(jnode *n, char **s, bool is_obj)
{
	n->get = jnode_get(n);
	for (const char end = is_obj ? '}' : ']'; ++*s, skip(s), **s != end;) {
		n->to.o      = realloc(n->to.o, (++n->len) * sizeof *n->to.o);
		jnode *child = calloc(1, sizeof *child);
		if (is_obj) child->key = parse_str(s), skip(s);
		parse_node(n->to.o[n->len - 1] = child, s);
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

__attribute__((overloadable)) void free(jnode *n)
{
	if (n->get) Block_release(n->get);
	for (size_t i = 0; i < n->len; ++i) free(n->to.o[i]);
	free(n->to.o); free(n->to.s); free(n->key); free((void *)n);
}
// clang-format on

__attribute__((overloadable)) jnode *parse(const char *json)
{
	char  *s = (char *)json;
	jnode *n = malloc(sizeof(jnode));
	parse_node(n, &s);
	return n;
}
