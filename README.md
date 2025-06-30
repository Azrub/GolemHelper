# GolemHelper BETA

A Guild Wars 2 addon for [Nexus](https://github.com/RaidcoreGG/Nexus) that that automates boon application and golem configuration in the Special Forces Training Area with support for different DPS role modes.

## Description

GolemHelper eliminates the tedious manual clicking required to set up optimal training conditions. With a single keypress, you can apply all necessary boons or configure golem settings.
The addon automatically scales coordinates for different resolutions and UI sizes.

Particularly useful for players who:
- Regularly practice rotations on training golems
- Test different build configurations
- Play multiple DPS roles (Quickness DPS, Alacrity DPS, Chronomancer DT+DR)
- Want to minimize setup time and focus on actual gameplay practice

## Features

- Automated Boon Application: Apply boon configuration based on dps mode.
- Automated Golem Configuration: Apply SnowCrows benchmark settings on golem with one keypress.

DPS Modes:
- Quick DPS Mode: Automatically skips Quickness boon
- Alac DPS Mode: Automatically skips Alacrity boon
- Chronomancer Mode: Automatically skips Slow debuff on golem

## Planned Features (No ETA)

- ImGui Interface: Visual interface for easier mode switching and configuration (coming in v1.0.0 final)
- Additional DPS Modes: More specialized role configurations based on community feedback
- Custom Presets: User-defined boon and golem configurations

## Installation

1. Install [Nexus Addon Loader](https://github.com/RaidcoreGG/Nexus)
2. Download `GolemHelper.dll` from the [latest release](https://github.com/Azrub/GolemHelper/releases)
3. Place the DLL in your `Guild Wars 2/addons` folder
4. Start the game
5. Configure keybinds in Nexus addon settings

## Building from Source

### Requirements
- Visual Studio 2022
- Windows SDK
- C++17 or later

### Steps
1. Clone the repository
2. Open the solution in Visual Studio
3. Build the solution in Release mode
4. The compiled DLL will be in `x64/Release/GolemHelper.dll`

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
