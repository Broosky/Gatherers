# Gatherers

## Ideas & Upcoming changes:

- Optimize pathfinding...
  - Selectable pathfinding mode: raycast, bisection...
  - Memoize pathfinding points.
  - For main pathfinding points, curve the transitions such that the movement is not jagged.
- Move entity resource diagnostic rendering out of entity rendering logic.
- Draw scaled down versions of entities in the minimap instead of coloured blocks.
- Engine:
  - Homebrew renderer (DIBSECTION)...
  - Selectable renderer: GDI32/SDL/Direct2D...
  - Networking...
  - Sound...
- Mouse wheel zoom in or out.
- Change constant `#define`s types.
- Load settings from file/hot reload.
- Replace classic `char` buffer copying with modern safer alternatives.
- Fail fast on bad `malloc`s.
- When building, if the entity extends past the client area, disallow it.
- All functions to reference passed pointers & variables instead of global declarations/externs (where applicable).
- Double check and update `const` parameters.
- New assets...
- `#include` dependency graph organization.
- Use stack allocations instead of repetitive pointer dereferences.
- Add forward declaration for internally referenced structs (e.g. `struct ENTITY* p_Next`, `p_Operating`).
- Crossplatform support: Refactor IO, clock, etc...