# Gatherers

This is a continuation of the Gatherers-Historic repo that I will contribute to when I have some free time. See https://github.com/Broosky/Gatherers-Historic for more info.

## April 2025 Changes:

- At long last, update pointer deref style.
- Arrow keys adjust translations.
- Uptime.
- Combine engine consideration in Win32 message loop.
- Crop images for DC blit operations.
- Build mode + date & time.
- Updates to toolchain.
- Benchmarking capture rough-in.
- Logging system.
- Add Visual Studio Community 2022 solution, project, compiler directives, and code formatting.
- Debug and release flags for MSVC.
- Partial `#include` dependency graph organization.
- Menu shuffling.
- Local function type review.

## March 2025 Changes:

- Write current working directory.
- Added prefixes for diagnostics.
- New diagnostics: selected entities, selected workers, selection area location and size.
- Improved double buffer canvas stability on resize.
- Bumped some initial values to get building faster.
- Rearranged and renamed some menu items.
- Added a diagnostics toggle-all in the menu.
- Changed some diagnostic colours.
- Added ID's to entities.
- Minimap now shows selection area.
- Minimap allows left clicking to translate the main view.
- Minimap highlights selected entities.
- Added worker scattering when commanding to harvest. AI will find the nearest resources to the one selected to harvest for multiple selected workers.
- Reorganized file structure.
- Added Code::Blocks project file.
- Added some assembly info.
- Updated toolchain for Windows and Linux.
- Added .gitignore.
- Adjust typedef's.
- Review code and comments.
- Distance calc's.
- Added Dev-C++ project file and toolchain.
  - Note, Dev-C++ will generate *_private.h and *_private.rc files which, AFAIK, can't be turned off even when the project file option for assembly information is disabled. For completeness, I've aligned them with whatever is in the main.rc.