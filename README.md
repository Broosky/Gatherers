﻿# 💡 **Gatherers**

A continuation of the [Gatherers-Legacy](https://github.com/Broosky/Gatherers-Legacy) StarCraft-style simulation. Active development continues here when time allows. See the legacy repo for early progress and design context.

> If you found this project useful, interesting, or worth keeping an eye on, consider giving it a ⭐️.
> It helps others discover the project and motivates me to keep building and sharing more.

## 🔹 Ideas & Upcoming Changes

- Optimize pathfinding...
  - Selectable pathfinding mode: raycast (original), bisection...
  - Memoize pathfinding points, or compute full path + memoize.
  - For main pathfinding points, curve the transitions such that the movement is not jagged.
- Move entity resource diagnostic rendering out of entity rendering logic.
- Selectable renderer: Direct3D.
- Engine:
  - Homebrew renderer (DIBSECTION)...
  - Networking...
  - Sound...
  - 3D spectator mode...
- Crossplatform support: Refactor IO, clock, etc...
- Mouse wheel zoom in or out.
- Asset consolidation and compression.
- Save/load sim...
  - Drop save file onto exe to quick load.
- Replace classic `char` buffer copying with modern safer alternatives.
- Double check and update `const` parameters.
- Use stack allocations instead of repetitive pointer dereferences for tight loops.
- Single vs multiple instances.
- Doxygen documentation.
- Don't allow workers to move inside an entity.
- Check include paths for Linux toolchain.
- Unit movement acceleration/deceleration + framerate independent unit movement.
- Decompose blur algorithm from SIMD to non-SIMD for better compatibility at the cost of performance.
- Fixed point arithmetic?
- New assets...

## 🔹 June 2025 Updates

- Tiled terrain + minimap.
- Engine pre/post-processing.
- Draw scaled down versions of entities in the minimap instead of coloured blocks w/optional transparency.
- Instant FPS meter alongside the long-term average.
- Pretty print signed numbers.
- Min & max client size.
- Blitter index as enum.
- Directories for the logger.
- Consolidate TTS.

## 🔹 May 2025 Updates

- Rough-in pathfinding optimizations.
- Rough-in renderer dirty zones.
- Memory pools for critical allocation paths (pathfinding).
- Refactor existing `malloc`s to use memory pools where applicable.
- Change constant `#define`s to types.
- Migrate volatile settings to file + hot reload.
- Restructure `DOUBLE_BUFFER_T` to `RENDERER_T` and allow it to operate on a renderer implementation strategy.
- Selectable renderer: wireframe, GDI single/double buffer, SDL, Direct2D.
- Modified the resource bar for large numbers: 999,999,999.
- Crop blit selective background on redraw.
- Improve log flow with trace, warning, and others + common writing functions.
- Maintenance timebase + daily bookending of log file.
- Linked list traversal helpers.
- Rough in some engine pre/post-processing.
- Experiment with frame buffer channel masking and blurs using SSE SIMD vectorization.
- Restructured menu items.
- Split large functions into smaller ones.
- Other architectural and miscellaneous changes + toolchain updates.

## 🔹 April 2025 Updates

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
- Menu structure refinements.
- Reviewed local function types.
- Images -> Assets (renamed + consolidated GDI brushes/pens).
- `#include` dependency graph organization.
- All functions to reference passed pointers & variables instead of global declarations/externs (where applicable).
- Fail fast on bad `malloc`s.
- Bump map size.
- Borderless windowed mode (escape toggleable) + other chord QoL keys.
- Consolidate MoveTo/LineTo's.
- Replace I/FPOINT structs with I/FDELTA structs where applicable.
- When building, if the entity extends past the client area, disallow it.
- Right-click -> build none.
- Comma print large positive numbers.
- Clean up sorting.
- Render + slight pathfinding optimizations.
  
## 🔹 March 2025 Updates

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
