ofxSpidarMouse
========

Naoto Hieda <micuat@gmail.com>
Mar 04, 2013


About
--------

ofxAddon for [SPIDAR-mouse (in Japanese)]( http://spidar-string.com ), a haptic mouse developed by Sato lab, Tokyo Tech.


Usage
--------

1. Declare `ofxSpidarMouse spidar;` in `testApp.h`.
2. `spidar.init();` and `spidar.open()` in `testApp::setup()`.
3. `spidar.update();` in `testApp::update()`.
4. `spidar.setForce(x, y, duration);` to apply force feedback. `spidar.setForce(x, y);` for infinite duration.
5. `spidar.draw();` in `testApp::draw()` to show debug window (useful if developing without physical SPIDAR-mouse)


License
--------
See LICENSE.

