/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();
	drawFlag = 0;
} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here
	if (drawFlag) {
		int x = 0, y = 0;
		for (i = 0; i < HEIGHT; i++) {
			x = 0;
			for (j = 0; j < WIDTH - 1; j += 2) {
				if (maze[i][j] == '+' && maze[i][j + 1] == '-')
					ofDrawLine(x, y, x + 30, y);
				else if (maze[i][j] == '|')
					ofDrawLine(x, y, x, y + 30);
				x += 30;
			}
			if (maze[i][WIDTH - 1] == '|')
				ofDrawLine(x, y, x, y + 30);
			if (i % 2) y += 30;
		}
		
	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			// TO DO
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			//string tmp;

			int i = 0;
			for(string tmp : buffer.getLines()) {
				WIDTH = 0;
				vector<char> row;
				cout << tmp << '\n';
				while (tmp[WIDTH] != '\0') {
					row.push_back(tmp[WIDTH]);
					WIDTH++;
				}
				HEIGHT++;
				maze.push_back(row);
			}

			drawFlag = 1;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	for (int i = 0; i < HEIGHT; i++) {
		maze[i].clear();
		vector<char>().swap(maze[i]);
	}
	maze.clear();
	vector<vector<char>>().swap(maze);
}

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	//TO DO
	//DFSŽ���� �ϴ� �Լ� ( 3����)
	
	bool flag = false;
	int x = 0, y = 0, width = WIDTH/2, height = HEIGHT/2;
	visited.resize(height, vector<int>(width, 0));
	visited[y][x] = 1;
	S.push(y * width + x);
	bestLine.push_back(make_pair(0, 0));
	while (!S.empty()) {
		int top = S.top();
		int i = top / width;
		int j = top % width;
		y = i * 2 + 1;
		x = j * 2 + 1;
		if (top == (height - 1) * width + width - 1) {
			printf("finished\n");
			isdfs = true;
			tmpFlag = false;
			return true;
		}

		if (maze[y + 1][x] != '-' && visited[i + 1][j] != 1) {
			S.push((i + 1) * width + j);
			bestLine.push_back(make_pair(i+1, j));
			visited[i + 1][j] = 1;
			flag = true;
		}
		else if (maze[y][x + 1] != '|' && visited[i][j + 1] != 1) {
			S.push(i * width + j + 1);
			bestLine.push_back(make_pair(i, j+1));
			visited[i][j + 1] = 1;
			flag = true;
		}
		else if (maze[y - 1][x] != '-' && visited[i - 1][j] != 1) {
			S.push((i - 1) * width + j);
			bestLine.push_back(make_pair(i-1, j));
			visited[i - 1][j] = 1;
			flag = true;
		}
		else if (maze[y][x - 1] != '|' && visited[i][j - 1] != 1) {
			S.push(i * width + j - 1);
			bestLine.push_back(make_pair(i, j-1));
			visited[i][j - 1] = 1;
			flag = true;
		}

		if (flag) {
			flag = false;
		}
		else {
			bestLine.pop_back();
			S.pop();
		}
	}
	return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS�� ������ ����� �׸���. (3���� ����)
	ofSetColor(0, 255, 0);
	for (int i = 0; i < (HEIGHT / 2) - 1; i++) {
		for (int j = 0; j < (WIDTH / 2) - 1; j++) {
			if (visited[i][j] == 1 && visited[i][j + 1] == 1 && maze[i*2+1][j*2+2] != '|')
				ofDrawLine(j * 30 + 15, i * 30 + 15,  j * 30 + 45, i * 30 + 15);
			
			if (visited[i][j] == 1 && visited[i+1][j] == 1 && maze[i * 2 + 2][j * 2 + 1] != '-')
				ofDrawLine(j * 30 + 15, i * 30 + 15, j * 30 + 15, i * 30 + 45);
			//if(!tmpFlag)	printf("%d ", visited[i][j]);
		}
		//if (!tmpFlag)	printf("\n");
	}
	
	for (int j = 0; j < (WIDTH / 2) - 1; j++) {
		if (visited[HEIGHT / 2 - 1][j] == 1 && visited[HEIGHT / 2 - 1][j + 1] == 1 && maze[HEIGHT - 2][j * 2 + 2] != '|') {
			ofDrawLine(j * 30 + 15, (HEIGHT / 2 - 1) * 30 + 15, j * 30 + 45, (HEIGHT / 2 - 1) * 30 + 15);
			
		}
	}
	tmpFlag = true;
	int size = bestLine.size();
	ofSetColor(255, 0, 0);
	for (int i = 0; i < size - 1; i++) {
		ofDrawLine(bestLine[i].second * 30 + 15, bestLine[i].first * 30 + 15, bestLine[i+1].second * 30 + 15, bestLine[i + 1].first * 30 + 15);
	}
}

bool ofApp::BFS()
{
	int x = 0, y = 0, width = WIDTH / 2, height = HEIGHT / 2;
	vector< vector< pair<int, int> > > prev(HEIGHT, vector< pair<int, int> >(WIDTH, make_pair(-1, -1)));
	visited.resize(height, vector<int>(width, 0));
	bestLine.push_back(make_pair(0, 0));
	visited[y][x] = 1;
	Q.push(y * WIDTH + x);

	while (!Q.empty()) {
		int front = Q.front();
		Q.pop();
		int i = front / width;
		int j = front % width;
		int curY = i * 2 + 1;
		int curX = j * 2 + 1;

		if (front == (height - 1) * width + width - 1) {
			// �ִ� ��θ� ã���� ��, �������Ͽ� bestLine ���Ϳ� ����
			int prevY = height - 1;
			int prevX = width - 1;

			while (prevY != -1 && prevX != -1) {
				bestLine.push_back(make_pair(prevY, prevX));
				int tempY = prev[prevY][prevX].first;
				int tempX = prev[prevY][prevX].second;
				prevY = tempY;
				prevX = tempX;
			}
			
			// bestLine ���͸� �������� ������
			reverse(bestLine.begin(), bestLine.end());
			bestLine.pop_back();
			isdfs = true;
			// bestLine ���Ϳ� �ִ� ��� ��ǥ���� ����Ǿ� ����
			return true;
		}

		if (maze[curY + 1][curX] != '-' && visited[i + 1][j] != 1) {
			Q.push((i + 1) * width + j);
			visited[i + 1][j] = 1;
			prev[i + 1][j] = make_pair(i, j);
		}

		if (maze[curY][curX + 1] != '|' && visited[i][j + 1] != 1) {
			Q.push(i * width + j + 1);
			visited[i][j + 1] = 1;
			prev[i][j + 1] = make_pair(i, j);
		}

		if (maze[curY - 1][curX] != '-' && visited[i - 1][j] != 1) {
			Q.push((i - 1) * width + j);
			visited[i - 1][j] = 1;
			prev[i - 1][j] = make_pair(i, j);
		}

		if (maze[curY][curX - 1] != '|' && visited[i][j - 1] != 1) {
			Q.push(i * width + j - 1);
			visited[i][j - 1] = 1;
			prev[i][j - 1] = make_pair(i, j);
		}
	}
	return false;
}
void ofApp::bfsdraw()
{

}


