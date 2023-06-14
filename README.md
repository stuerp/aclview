
# aclview

[aclview](https://github.com/stuerp/aclview/releases) is a [Total Commander](https://www.ghisler.com/) lister plugin that shows the ACL of a file or directory.

## Features

* Supports dark mode.

## Requirements

* Tested on Microsoft Windows 10 and later.
* Tested with [Total Commander](https://www.ghisler.com/) 10.52 (32 or 64-bit) and later.

## Getting started

* Double-click `aclview.zip`.

or

* Select `aclview.zip` in the Total Commander Lister plugins dialog.

## Developing

The code builds out-of-the box with Visual Studio.

### Requirements

To build the code you need:

* [Microsoft Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/downloads/) or later

### Setup

Create the following directory structure:

    bin
    aclview

* `bin` contains a portable version of Total Commander 32-bit and 64-bit for debugging purposes.
* `aclview` contains the [Git](https://github.com/stuerp/aclview) repository.

### Building

Open `aclview.sln` with Visual Studio and build the solution.

### Packaging

To create the package first build the x86 configuration and next the x64 configuration.

## Contributing

If you'd like to contribute, please fork the repository and use a feature
branch. Pull requests are warmly welcome.

## Change Log

v0.1.0, 2023-06-13, *"Scratchin' the itch"*

* Initial release.

## Acknowledgements / Credits

* Christian Ghisler for [Total Commander](https://www.ghisler.com/).

## Reference Material

* Windows User Interface
  * [Desktop App User Interface](https://learn.microsoft.com/en-us/windows/win32/windows-application-ui-development)
  * [Windows User Experience Interaction Guidelines](https://learn.microsoft.com/en-us/windows/win32/uxguide/guidelines)
  * [Windows Controls](https://learn.microsoft.com/en-us/windows/win32/controls/window-controls)
  * [Control Library](https://learn.microsoft.com/en-us/windows/win32/controls/individual-control-info)
  * [Resource-Definition Statements](https://learn.microsoft.com/en-us/windows/win32/menurc/resource-definition-statements)
  * [Visuals, Layout](https://learn.microsoft.com/en-us/windows/win32/uxguide/vis-layout)

## Links

* Home page: https://github.com/stuerp/aclview
* Repository: https://github.com/stuerp/aclview.git
* Issue tracker: https://github.com/stuerp/aclview/issues

## License

![License: MIT](https://img.shields.io/badge/license-MIT-yellow.svg)
