# GolemHelper

A Guild Wars 2 addon for [Nexus](https://github.com/RaidcoreGG/Nexus) that automates optimal boon and golem configurations in the Special Forces Training Area.

## Features

DPS Benchmarking:

- hi dps
- Quick DPS
- Alac DPS

Healer Support:

- Environment Damage with Mild/Moderate/Extreme settings
- Specialized healer boon sequences (qHeal/aHeal modes)

Golem Customization:

- Hitbox selection: Small/Medium/Large (Small is benchmark default)
- Advanced condition modifiers: Skip Slow, Skip Burning, 5 Bleeding Stacks

Tips:

- If the sequence is too fast and missing clicks, increase Custom Delays in the UI
- You can move your mouse during the sequence, just don't hover over UIs or click

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
