# ios_json.c

Ref: <https://github.com/callumgran/json-struct>

Intended to be a convenient API for parsing JSON files in iOS. Looking to maybe optimize for use with `.gltf` files.

- [x] Concept
- [x] Chaining? (`->get->get->get->etc`)
- [ ] Scalar
- [ ] NEON

Example usage

```
 jnode *n = parse(json);
 n->get("string")->to.s); => Hello, World!
 n->get("int")->to.n; => 420
 n->get("float")->to.n; => 6.9
 n->get("bool")->to.b; => true
 n->get("object")->get("key")->to.s; => "value"
 n->get("array")->to.a[4]->get("key")->to.s; => "value"
```
