#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofJson settings = ofLoadJson("settings.json");
	hp.setup( settings["device"].get<string>());
    
	setPaperSize(settings["paperSize"][0].get<float>(),settings["paperSize"][1].get<float>());
    cout << "sendPenSelectCommand " << settings["sendPenSelectCommand"].get<bool>() <<endl;
    hp.setPenSelectSending(settings["sendPenSelectCommand"].get<bool>());
    
	cout << "max buffer size : " << hp.getAvailBufferSize() <<endl;
	hp.enableCapture();
}

//--------------------------------------------------------------
void ofApp::update() {
    
    hp.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(100);
	ofSetColor(255);

	ofRectangle src = ofRectangle(0,0, preview.getWidth(),preview.getHeight());
	ofRectangle dst = ofRectangle(20,20,ofGetWidth()-40,ofGetHeight()-40);
	src.scaleTo(dst);

	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(src.x,src.y);
	ofPushMatrix();
	ofScale(1,-1,1);
	ofTranslate(0,-src.height);
	ofDrawRectangle(0,0,src.width,src.height);
	preview.draw(0,0,src.width,src.height);
	ofPopMatrix();
	ofPopMatrix();

	svg.draw();

ofSetColor(200);
	//ofSetColor( hp.isConnected() ? ofColor( 20, 230, 10 ) : ofColor( 220, 10, 50 ) );
	//ofDrawCircle( 20, 20, 4 );
    
    stringstream ss;
    ss << "isConnected: " << (hp.isConnected() ? "yes" : "no") << endl;
	ss << "currentFile : " << fileName << endl;
	ss << "paperSize " << paperWidth << " x " << paperHeight << "mm (" << (paperWidth == 297. ? "A4" : "A3") << ") - change format (d)" <<endl;
	ss << "loadFile( )" <<endl;
    ss << "print(p) " << endl;
    ss << "toggleSendPenSelect(s) "<< endl;
    ss << "clear(DEL)" << endl;
    ofSetColor( 30 );
	ofDrawBitmapString( ss.str(), 30, 22 );

	//panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'p' ) {
		//hp.clear();
		//hp.print();
		//ofSleepMillis(7000);
		if(isHpgl){
			proceedPrint(analyseFile(hpglFile));
		}else{
			hp.print(true);
		}

    }
	if(key == 'f')ofToggleFullscreen();

    if(key == 's'){
        hp.setPenSelectSending(!hp.getPenSelect());
    }
    
	if(key == 'd'){
		if(paperWidth == 297.){
			setPaperSize(420.,297.);
		}else{
			setPaperSize(297.,210.);
		}
	}
	if(key == 'r'){
		//Load the selected file
		string ending = ofSplitString(fileName,".")[1];

		if(ending == "svg"){
			ofxSVG svg;
			svg.load(fileName);
			hp.clear();
			for (auto& p:svg.getPaths()){
				auto i = p.getOutline();
				for(auto& polyline : p.getOutline()){
					hp.polyline(polyline);
				}
			}

			preview.begin();
			ofClear(0,0);
			ofPushMatrix();
			ofScale(1,-1,1);
			ofTranslate(0,-preview.getHeight());

			hp.draw();
			ofPopMatrix();

			preview.end();
		}else{
			ofImage image;
			hpglFile = ofBufferFromFile(fileName);

			drawPreview(analyseFile(hpglFile));
		}

	}
	if (key == ' '){

		//Open the Open File Dialog
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a svg or hpgl");

		//Check if the user opened a file
		if (openFileResult.bSuccess){

			ofLogVerbose("User selected a file");

			//We have a file, check it and process it
			processOpenFileSelection(openFileResult);

		}else {
			ofLogVerbose("User hit cancel");
		}
	}
	if (key == 'a'){

		//Open the Open File Dialog
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a svg or hpgl");

		//Check if the user opened a file
		if (openFileResult.bSuccess){

			ofLogVerbose("User selected a file");

			//We have a file, check it and process it
			addFileSelection(openFileResult);

		}else {
			ofLogVerbose("User hit cancel");
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    polyline.addVertex( x, y );
}
	ofBuffer buffer = ofBufferFromFile("hpgl.hpgl");
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    polyline.clear();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if( polyline.getPerimeter() > 10 ) {
        polyline = polyline.getResampledBySpacing( 8 );
        
        hp.polyline( polyline );
        polyline.clear();
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
void ofApp::setPenSelectSending(bool b){
    hp.setPenSelectSending(b);
}
void ofApp::setPaperSize(float width, float height)
{
	paperWidth = width;
	paperHeight = height;

	int windowW = 1920;
	int windowH = float(windowW) * float(paperHeight/paperWidth);
	ofSetWindowShape( windowW, windowH );
	hp.setInputWidth( windowW );
	hp.setInputHeight( windowH );

	if (width == 420){
		hp.setPaperSize(ofxHPGL::Settings::PAPER_SIZE_A3);
	}else{
		hp.setPaperSize(ofxHPGL::Settings::PAPER_SIZE_A4);
	}

	preview.allocate(windowW,windowH);
	preview.begin();
	ofClear(0,0);
	preview.end();
}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult)
{
	ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());

	ofFile file (openFileResult.getPath());

	if (file.exists()){


		ofLogVerbose("The file exists - now checking the type via file extension");
		string fileExtension = ofToUpper(file.getExtension());

		//We only want hpgl
		if (fileExtension == "HPGL" ) {

			//Load the selected file
			fileName = openFileResult.getPath();
			hpglFile = ofBufferFromFile(openFileResult.getPath());

			drawPreview(analyseFile(hpglFile));
			isHpgl = true;
		}

		else if (fileExtension == "SVG") {
			fileName = openFileResult.getPath();

			//ofxSVG svg;
			svg.load(fileName);
			hp.clear();
			for (auto& p:svg.getPaths()){
				auto i = p.getOutline();
				for(auto& polyline : p.getOutline()){
					hp.polyline(polyline);
				}
			}

			preview.begin();
			ofClear(0,0);
			ofPushMatrix();
			ofScale(1,-1,1);
			ofTranslate(0,-preview.getHeight());

			hp.draw();
			ofPopMatrix();



			preview.end();


			isHpgl = false;
		}

	}
}

void ofApp::addFileSelection(ofFileDialogResult openFileResult)
{
	ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());

	ofFile file (openFileResult.getPath());

	if (file.exists()){


		ofLogVerbose("The file exists - now checking the type via file extension");
		string fileExtension = ofToUpper(file.getExtension());

		//We only want hpgl
		if (fileExtension == "HPGL" ) {

			//Load the selected file
			auto v1 = analyseFile(hpglFile);

			fileName = openFileResult.getPath();
			hpglFile = ofBufferFromFile(openFileResult.getPath());

			auto v2 = analyseFile(hpglFile);

			v1.insert(v1.end(), v2.begin(), v2.end());

			drawPreview(v1);
		}

	}
}

vector<string> ofApp::analyseFile(ofBuffer buffer)
{
	vector<string> commands;

	size_t tMax = 200;
		if(buffer.size()) {

			auto cmds = ofSplitString(buffer.getLines().begin().asString(),";");
			for(auto& cmd:cmds){
				if(cmd.length() > tMax){
					string baseCmd = cmd.substr(0,2);
					string singleCmd = baseCmd;
					auto cmdParts = ofSplitString(cmd,",");
					cmdParts.front() = cmdParts.front().substr(2);

					for(size_t i= 0; i<cmdParts.size(); i+=2){
						string cmdTemp = singleCmd + cmdParts[i] + "," + cmdParts[i+1] + ",";
						if (cmdTemp.size() > tMax || i >= cmdParts.size() - 2){
							commands.push_back(cmdTemp);
							singleCmd = baseCmd;
						}else{
							singleCmd = cmdTemp;
						}
					}
				}else{
					commands.push_back(cmd);
				}
			}
		}
		return commands;
}

void ofApp::drawPreview(vector<string> commands)
{
	preview.begin();
	ofClear(0,0);

	string currendCmd = commands.front().substr(0,2);
	ofVec2f lastPos;
	for(auto& cmd:commands){
		auto cmdParts = ofSplitString(cmd,",");
		currendCmd = cmd.substr(0,2);
		if(cmdParts.size()>=2){
			if(cmdParts.back() == ""){
				cmdParts.pop_back();
			}
			cmdParts.front() = cmdParts.front().substr(2);
			for(size_t i= 0; i<cmdParts.size(); i+=2){
				ofVec2f currentPos = convertCmdPosToFbo(ofToInt(cmdParts[i]),ofToInt(cmdParts[i+1]));
				if(currendCmd == "PD"){
					ofSetColor(0);
					ofDrawLine(lastPos,currentPos);
				}
				lastPos = currentPos;
			}
		}
	}
	preview.end();
}

ofVec2f ofApp::convertCmdPosToFbo(int x, int y)
{
	float tConversion = 0.025*preview.getWidth()/paperWidth;
	return ofVec2f(x,y)*tConversion;
}

void ofApp::proceedPrint(vector<string> commands)
{
	for(auto& cmd:commands){
		hp.addPrinterCommand(cmd +";");
	}
	//hp.addPrinterCommand("PU7721,11040;");


}


void ofApp::drawCircles()
{

}

void ofApp::createPolyLines(vector<vector<glm::tvec3<float>> > p)
{
	//delete double line segments
	vector<int> del;

	//get double segments forward
	for(int l1= 0;l1<p.size();l1++){
		for(int l2= 0;l2<p.size();l2++){
			if(l1 <l2 && p[l1].size() == p[l2].size()){
				bool delVec = true;
				//forward
				for(int i = 0; i<p[l1].size(); i++){
						if(p[l1][i] != p[l2][i]){
							delVec = false;
						}
				}
				if (delVec) del.push_back(l2);
			}
		}
	}

	//backwards
	for(int l1= 0;l1<p.size();l1++){
		for(int l2= 0;l2<p.size();l2++){
			if(l1 <l2 && p[l1].size() == p[l2].size()){
				bool delVec = true;
				//forward
				int s = p[l1].size();
				for(int i = 0; i<s; i++){

						if(p[l1][i] != p[l2][s-1-i]){
							delVec = false;
						}
				}
				if (delVec) del.push_back(l2);
			}
		}
	}

	sort(del.begin(),del.end());
	del.erase( unique( del.begin(), del.end() ), del.end() );

	for(int i=del.size()-1;i>=0;i--){
		p.erase(p.begin()+del[i]);
	}


	for(auto& line:p){
		ofPolyline poly;
		poly.addVertices(line);
		//poly.simplify();
		hp.polyline( poly );
	}
}






//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
