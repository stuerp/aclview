
# aclview

[aclview](https://github.com/stuerp/aclview/releases) is a [Total Commander](https://www.ghisler.com/) lister plugin that shows the NTFS ACL of a file or directory.

![Screenshot](/Artwork/Screenshot.png?raw=true "Screenshot")

## Features

* Verbose description of the ALC instead of the microscopic default Windows dialog.
* Any NTFS file system item can be examined.
* Supports dark mode.
* Support Total Commander 32-bit (.wlx) and 64-bit (.wlx64).

## Requirements

* Tested on Microsoft Windows 10 and later.
* Tested with [Total Commander](https://www.ghisler.com/) 10.52 (32 or 64-bit) and later.

## Getting started

* Double-click `aclview.zip` in Total Commander.

## Developing

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

v0.3.0, 2023-xx-xx, *"Is it dark in here?"*

* Light and dark mode implemented. Still having problems with the scrollbar.

v0.2.0, 2023-06-15, *"A small step"*

* Implemented percentage display for the Lister window menu bar.
* The text can be searched using the default search feature of the Lister.
* The content gets updated when switching between items in Quick View mode.
* Improved: Optimized text highlighting.

v0.1.0, 2023-06-14, *"Scratchin' the itch"*

* Initial release.

## Acknowledgements / Credits

* Christian Ghisler for [Total Commander](https://www.ghisler.com/).

## Reference Material

* Windows User Interface
  * [Desktop App User Interface](https://learn.microsoft.com/en-us/windows/win32/windows-application-ui-development)
  * [Windows User Experience Interaction Guidelines](https://learn.microsoft.com/en-us/windows/win32/uxguide/guidelines)
  * [Windows Controls](https://learn.microsoft.com/en-us/windows/win32/controls/window-controls)
    * [RichEdit](https://learn.microsoft.com/en-us/windows/win32/controls/rich-edit-controls)
  * [Control Library](https://learn.microsoft.com/en-us/windows/win32/controls/individual-control-info)
  * [Resource-Definition Statements](https://learn.microsoft.com/en-us/windows/win32/menurc/resource-definition-statements)
  * [Visuals, Layout](https://learn.microsoft.com/en-us/windows/win32/uxguide/vis-layout)

## Links

* Home page: https://github.com/stuerp/aclview
* Repository: https://github.com/stuerp/aclview.git
* Issue tracker: https://github.com/stuerp/aclview/issues

## License

![License: MIT](https://img.shields.io/badge/license-MIT-yellow.svg)
