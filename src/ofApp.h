#pragma once

#include "ofMain.h"
#include "ofxHPGL.h"
#include "ofxGui.h"
#include "ofxSvg.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void setPaperSize(float width, float height);
    void setPenSelectSending(bool b);
    
	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void addFileSelection(ofFileDialogResult openFileResult);

	vector<string> analyseFile(ofBuffer buffer);
	void drawPreview(vector<string> commands);
	ofVec2f convertCmdPosToFbo(int x,int y);
	void proceedPrint(vector<string> commands);

	string fileName = "";
	ofBuffer hpglFile;


	void createFont();
	void drawCircles();




	void createPolyLines(vector<vector<glm::tvec3<float>> >  p);
	vector<vector<glm::tvec3<float>> >  points;
    
    ofxHPGL hp;
//    vector< ofPolyline > polylines;
    ofPolyline polyline;
	ofxPanel panel;
	ofPath testChar;
	ofParameterGroup params;

	ofFbo preview;

	ofxSVG svg;

	float paperWidth = 297.;
	float paperHeight = 210.;

	bool isHpgl = true;

};
