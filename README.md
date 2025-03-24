# Gatherers

This is a continuation of the Gatherers-Historic repo that I will contribute to when I have some free time. See https://github.com/Broosky/Gatherers-Historic for more info.

## March 2025 Changes:
<ul>
  <li>Write current working directory.</li>
  <li>Added prefixes for diagnostics.</li>
  <li>New diagnostics: selected entities, selected workers, selection area location and size.</li>
  <li>Improved double buffer canvas stability on resize.</li>
  <li>Bumped some initial values to get building faster.</li>
  <li>Rearranged and renamed some menu items.</li>
  <li>Added a diagnosics toggle-all in the menu.</li>
  <li>Changed some diagnostic colours.</li>
  <li>Added ID's to entities.</li>
  <li>Minimap now shows selection area.</li>
  <li>Minimap allows left clicking to translate the main view.</li>
  <li>Minimap highlights selected entities.</li>
  <li>Added worker scattering when commanding to harvest. AI will find the nearest resources to the one selected to harvest for multiple selected workers.</li>
  <li>Reorganized file structure.</li>
  <li>Added Code::Blocks project file.</li>
  <li>Added some assembly info.</li>
  <li>Updated toolchain for Windows and Linux.</li>
  <li>Added .gitignore.</li>
  <li>Adjust typedef's.</li>
  <li>Review code and comments.</li>
  <li>Distance calc's</li>
  <li>Added Dev-C++ project file and toolchain.
    <ul>
      <li>Note, Dev-C++ will generate *_private.h and *_private.rc files which, AFAIK, can't be turned off even when the project file option for assembly information is disabled. For completeness, I've aligned them with whatever is in the main.rc.</ul>
    </ul>
  </li>
</ul>