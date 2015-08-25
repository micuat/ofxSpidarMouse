ofxSpidarMouse
========

Naoto Hieda <micuat@gmail.com> - Aug 25, 2015


About
--------

This is an ofxAddon for SPIDAR-mouse, a haptic mouse developed at [Sato Lab]( http://sklab-www.pi.titech.ac.jp/ ), Tokyo Tech.

This addon uses [hidapi]( https://github.com/signal11/hidapi ) for USB HID communication.


Usage
--------

    class ofApp : public ofBaseApp{
        ofxSpidarMouse spidar;
        ...
    }

    ofApp::setup()
    {
        spidar.init();
        spidar.open();
        ...
    }

	ofApp::update()
    {
        spidar.setForce(x, y, duration); // apply force feedback for `duration` milliseconds.
        spidar.setForce(x, y); // apply force feedback until next setForce is called.
        spidar.update();
        ...
    }

	ofApp::draw()
    {
        spidar.draw(); // show debug window (useful if developing without physical SPIDAR-mouse)
        ...
    }


License
--------
See LICENSE.
