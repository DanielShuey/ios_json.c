# ios_json.c

Ref: <https://github.com/callumgran/json-struct>

Intended to be a convenient API for parsing JSON files in iOS. Looking to maybe optimize for use with `.gltf` files.

- [x] Concept
- [ ] Chaining? (`->get->get->get->etc`)
- [ ] Scalar
- [ ] NEON

Example usage

```
 strcmp(n->get(n, "string")->to.s, "Hello, World!") == 0);
 n->get(n, "int")->to.n == 420);
 n->get(n, "float")->to.n == 6.9);
 n->get(n, "bool")->to.b == true);
```
