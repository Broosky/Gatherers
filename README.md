﻿# 🛠️ **Gatherers**

A continuation of the [Gatherers-Historic](https://github.com/Broosky/Gatherers-Historic) StarCraft-style simulation.  
Active development continues here when time allows — see the historic repo for early progress and design context.

---

## 💡 Ideas & Upcoming Changes

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

---

## 🔄 April 2025 Updates

- Updated pointer dereference style.
- Arrow keys now adjust view translation.
- Added uptime tracking.
- Combined engine logic into Win32 message loop.
- Cropped images for DC blit ops.
- Added build mode, date & time stamping.
- Toolchain updates.
- Began benchmarking capture system.
- Introduced a logging system.
- Added Visual Studio 2022 solution/project setup and formatting rules.
- Configured debug/release flags for MSVC.
- Partial organization of `#include` dependency graph.
- Menu structure refinements.
- Reviewed local function types.

---

## 📦 March 2025 Updates

- Output current working directory.
- Prefixed diagnostics for clarity.
- New diagnostics: selected entities, selection box dimensions, selected workers.
- Improved double-buffer canvas stability on resize.
- Tweaked initial conditions for quicker building.
- Renamed and rearranged menu items.
- Added a diagnostics toggle-all option.
- Diagnostic colour updates.
- Assigned IDs to entities.
- Minimap:
  - Displays selection area.
  - Allows left-click translation of main view.
  - Highlights selected entities.
- Worker AI:
  - Scatters to nearest resources when multiple are selected.
- File structure reorganization.
- Added project files for:
  - Code::Blocks
  - Dev-C++
- Updated Windows/Linux toolchain support.
- Reviewed and updated typedefs, code, and comments.
- General code review and distance calculations.
- Note: Dev-C++ generates `_private.h`/`.rc` files by default — aligned these with `main.rc` where applicable.