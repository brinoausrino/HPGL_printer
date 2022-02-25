# HPGL printer

An openframeworks app controls a HPGL compatible device in plotting svgs and hpgl files.

## Installation

* download and unzip [openframworks](https://openframeworks.cc/download/) for your system
* add [ofxHPGL](https://github.com/brinoausrino/ofxHPGL) fork to `OF-FOLDER/addons`
* clone the project in `OF-Folder/apps`
* use the project generator to create the project
* in `APPFOLDER/bin/settings.json` edit `device` with your plotter address
* compile and run

## Usage

The app shows a very basic interface. In the top left corner of the screen you see:

* `isConnected` - connection status of the plotter
* `currentFile` - filename of the opened file
* `paperSize` - the current paper size (only important if you open svg files and use a HP7475)

Hotkeys are:

* `d` - switch format between A3 and A4
* `space` - open the file load dialog
* `p` - print the open file
* `del` - clear working screen

## Acknowledgments

This program is using a fork of [ofxHPGL addon](https://github.com/NickHardeman/ofxHPGL) by [Nick Hardeman](https://nickhardeman.com/).