#include "ios_json.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

const char *json = "{"
                   "\"string\": \"Hello, World!\","
                   "\"int\": 4,"
                   "\"float\": 6.9,"
                   "\"true\": true,"
                   "\"false\": false,"
                   "\"null_value\": null,"
                   "\"array\": ["
                   "    1,"
                   "    \"two\","
                   "    false,"
                   "    null,"
                   "    {"
                   "        \"key\": \"value\""
                   "    }"
                   "],"
                   "\"object\": {"
                   "    \"key\": \"value\","
                   "    \"nested_array\": ["
                   "        1,"
                   "        \"two\","
                   "        false,"
                   "        null,"
                   "        {"
                   "            \"key\": \"value\""
                   "        }"
                   "    ],"
                   "    \"nested_object\": {"
                   "        \"nested_key\": \"nested_value\""
                   "    }"
                   "}"
                   "}";

int main()
{
	jnode *n = parse(json);
	assert(strcmp(n->get(n, "string")->to.s, "Hello, World!") == 0);
	assert(n->get(n, "int")->to.n == 4);
	assert(n->get(n, "float")->to.n == 6.9);
	assert(n->get(n, "true")->to.b == true);
	assert(n->get(n, "false")->to.b == false);
	// assert(n->get(n, "null_value") == NULL);
	assert(n->get(n, "array")->to.a[0]->to.n == 1);
	assert(strcmp(n->get(n, "array")->to.a[1]->to.s, "two") == 0);
	assert(n->get(n, "array")->to.a[2]->to.b == false);
	// assert(n->get(n, "array")->to.a[3] == NULL);
	jnode *o = n->get(n, "array")->to.a[4];
	assert(strcmp(o->get(o, "key")->to.s, "value") == 0);
	free((void *)n);
}
