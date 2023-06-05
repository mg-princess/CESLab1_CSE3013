#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("T-Rex Game");
    ofSetFrameRate(60);
    ofBackground(255);
    
    //Image & Fonts 초기설정
    wholeImage.load("sprite.png");
    tRex.cropFrom(wholeImage, 76, 5, 88, 91);
    tRex.resize(60, 60);
    tRexRun1.cropFrom(wholeImage,1514, 2, 88, 94);
    tRexRun2.cropFrom(wholeImage,1602, 2, 88, 94);
    tRexRun1.resize(60, 60);
    tRexRun2.resize(60, 60);
    tRexEnd.cropFrom(wholeImage,1690, 2, 87, 94);
    tRexEnd.resize(60, 60);
    ground.cropFrom(wholeImage,0, 100, 2000, 30);
    smallCactus.cropFrom(wholeImage,446, 2 , 68, 70);
    largeCactus1.cropFrom(wholeImage,652, 2, 50, 96);
    largeCactus2.cropFrom(wholeImage,850, 2, 102, 98);
    gameOver.cropFrom(wholeImage,954, 29, 381, 21);
    restartButton.cropFrom(wholeImage, 2, 2, 72, 64);
    font.load("ARCADECLASSIC.TTF", 24);
    restartButtonPosition.x = ofGetWidth()/2 - restartButton.getWidth() + 35;
    restartButtonPosition.y = ofGetHeight()/2 - restartButton.getHeight();
    printScoreButton.x = 5;
    printScoreButton.y = 5;
    printScoreButton.height = 35;
    printScoreButton.width = 195;
    
    //open score file 초기설정
    ofFile file("score.txt");
    if(!file.exists()) {
        file.create();
    }
    
    ofBuffer buffer = file.readToBuffer();
    
    //파일에서 한줄 씩 읽어와 scores에 저장
    for(string line : buffer.getLines()) {
        int tmp = ofToInt(line);
        scores.push_back(tmp);
    }
    //scores벡터가 비어있지 않으면 최댓값을 맨앞 값으로 저장
    if(!scores.empty()) {
        topScore = scores.front();
    }
    //비어있으면 최댓값은 0
    else topScore = 0;
    //변수 initialize
    tRexPosition = ofPoint(50, ofGetHeight() / 2);
    isJumping = false;
    jumpForce = -10.0f;
    imageTimer = 0.0f;
    groundOffset = 0.0f;
    groundSpeed = 5.0f;
    isSpaceKeyPressed = false;
    jumpTimer = 0.0f;
    maxJumpTime = 1.0f; // 최대 점프 지속 시간
    isTrex = true;
    isGameStarted = false;
    isGameOver = false;
    lastX = 0;
    score = 0;
    cactusCount = 0;
    isPrintScore = false;
    cleanScoreBox = false;
    
    //선인장 초깃값 설정
    for(int i = 0; i<MAX_CACTUS_COUNT; i++) {
        cactuses[i].cactus = smallCactus;
        cactuses[i].position.x = ofGetWidth() + i * 400;
        cactuses[i].position.y = ofGetHeight() / 2 + 6;
    }
    file.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    //게임 중
    if(isGameStarted && !isGameOver){
        if (isJumping) {    // 점프 동작 업데이트
            if (isSpaceKeyPressed) {    //스페이스바가 눌리면,
                jumpTimer += ofGetLastFrameTime();  //점프 경과 시간 측정
                if (jumpTimer <= maxJumpTime) {     //최대 점프 시간보다 작으면
                    tRexPosition.y += jumpForce;    //중력에 맞춰 y좌표 설정
                    jumpForce += 0.4f; // 점프 감도 조절
                }
            }
            else {
                tRexPosition.y += jumpForce;    //스페이스바가 안눌려있으면
                jumpForce += 0.5f; // 점프 감도 조절  //더 빠르게 움직임
            }
            // 점프가 지면에 닿으면 점프 종료
            if (tRexPosition.y >= ofGetHeight() / 2) {
                tRexPosition.y = ofGetHeight() / 2;
                jumpForce = -10.0f;
                isJumping = false;
                jumpTimer = 0.0f;
            }
        }
        
        //이미지 1,2를 번갈아 출력하며 달리는 움직임 구현
        imageTimer += ofGetLastFrameTime();
        if (imageTimer >= 0.1f) {
            imageTimer = 0.0f;
            isTrex = !isTrex;
        }
        
        //점수의 최댓값은 99999로 설정
        if(score <= 99999)
            score += 1;
        
        //땅 움직임
        groundOffset -= groundSpeed;
        //땅의 화면 왼쪽으로 완전히 나가면 오프셋을 0으로 초기화
        if (groundOffset < -ground.getWidth()) {
            groundOffset = 0.0f;
        }
        //충돌이 일어났는지 판단
        if(checkCollision()) {
            isGameOver = true;
        }
        //선인장이 왼쪽 화면 밖으로 나가면 다시 계산 후 오른쪽으로 설정
        for (int i = 0; i < MAX_CACTUS_COUNT; i++) {
            cactuses[i].position.x -= groundSpeed;
            
            //화면 밖으로 나갔는지 판단
            if (cactuses[i].position.x + cactuses[i].cactus.getWidth() < 0) {
                cactuses[i].position.x = ofGetWidth();
                cactuses[i].position.y = ofGetHeight() / 2 - 20;

                // 랜덤한 선인장 선택
                int randomCactus = ofRandom(0, 2); // 0부터 1까지의 랜덤한 정수
                if (randomCactus == 0) {
                    cactuses[i].cactus = smallCactus;
                    cactuses[i].position.y += 26;
                }
                else if (randomCactus == 1) {
                    cactuses[i].cactus = largeCactus1;
                }
                else {
                    cactuses[i].cactus = largeCactus2;
                }
                cactusCount--;
                if(cactusCount < 0) {
                    cactusCount = MAX_CACTUS_COUNT - 1;
                }
                lastX = cactuses[cactusCount].position.x;
                // 겹치지 않도록 다음 선인장 위치 조정
                int minX = 1;
                int maxX = 5;
                int randomX = 50*ofRandom(minX, maxX);
                cactuses[i].position.x += randomX + lastX;
            }
        }
        lastX = cactuses[cactusCount].position.x;
        //시간이 지날수록 땅 속도 증가
        groundSpeed += (float)score/100000;
    }
    //게임 오버 이후 현재 점수 벡터에 반영 후 내림차순 정렬
    if(isGameOver && !isCalEnd) {
        scores.push_back(score);
        sort(scores.begin(), scores.end());
        reverse(scores.begin(), scores.end());
        topScore = scores.front();
        isCalEnd = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //게임 시작 전
    if(!isGameStarted) {
        //화면 출력
        tRex.draw(tRexPosition);
        ground.draw(0, ofGetHeight()/2 + ground.getHeight() + 5);
        ofPushStyle();
        ofSetColor(0);
        font.drawString("TREX GAME", ofGetWidth()/2 - 80, ofGetHeight()/2 - 40);
        font.drawString("Press  Space  to  Start", ofGetWidth() / 2 - 170, ofGetHeight() / 2 - 10);
        font.drawString("PrintScore", 10, 30); // (10, 30) 위치에 텍스트 출력
        
        ofNoFill(); // 사각형 내부를 채우지 않도록 설정
        ofDrawRectangle(printScoreButton);
        //printScore 박스가 클릭되면,
        if(isPrintScore) {
            drawScoreBox();
        }
        //printScore가 출력된 상태에서 박스가 한번 더 클릭되면,
        if(cleanScoreBox) {
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(ofGetWidth()/2 - 40, 0, ofGetWidth()/2 + 100, 400);
            ofPopStyle();
            cleanScoreBox = false;
        }
        ofPopStyle();
    }
    //게임 중
    else {
        //t-rex 번갈아 출력
        if(isTrex)
            tRexRun1.draw(tRexPosition);
        else
            tRexRun2.draw(tRexPosition);
        //땅을 화면의 끝까지 이어붙어 출력
        for (float x = groundOffset; x < ofGetWidth(); x += ground.getWidth()) {
            ground.draw(x, ofGetHeight()/2 + ground.getHeight() + 5);
        }
        //선인장 모두 출력
        for(int i = 0; i<MAX_CACTUS_COUNT; i++) {
            cactuses[i].cactus.draw(cactuses[i].position.x, cactuses[i].position.y);
        }
        ofPushStyle();
        ofSetColor(0);
        font.drawString("Score  " + ofToString(score, 5, '0'), ofGetWidth() - 200, 35);
        ofPopStyle();
    }
    //게임 오버
    if(isGameOver) {
        //화면 출력
        tRexEnd.draw(tRexPosition);
        ofPushStyle();
        ofSetColor(255);
        ofDrawRectangle(0, 0, ofGetWidth(), 200);
        ofPopStyle();
        ofPushStyle();
        ofSetColor(0);
        gameOver.draw(ofGetWidth() / 2 - 191, ofGetHeight() / 2 - 130);
        string scoreString = ofToString(topScore, 5, '0');
        //top 5 점수 출력
        if(scoreString.size() > 5)
            scoreString = "99999";
        font.drawString("HI "+ scoreString + "   " + ofToString(score, 5, '0'), ofGetWidth() - 250, 35);
        ofPopStyle();
        restartButton.draw(restartButtonPosition.x, restartButtonPosition.y);
        font.drawString("PrintScore", 10, 30); // (10, 30) 위치에 텍스트 출력
        
        ofNoFill(); // 사각형 내부를 채우지 않도록 설정
        ofDrawRectangle(printScoreButton);
        //printScore 박스가 클릭되면,
        if(isPrintScore) {
            drawScoreBox();
        }
        //printScore가 출력된 상태에서 박스가 한번 더 클릭되면,
        if(cleanScoreBox) {
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(ofGetWidth()/2 - 40, 0, ofGetWidth()/2 + 100, 250);
            ofPopStyle();
            cleanScoreBox = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //스페이스바가 눌리면
    if (key == ' ') {
        isGameStarted = true;
        isSpaceKeyPressed = true;
        if (!isJumping) {
            isJumping = true; // 스페이스바를 누르면 점프 시작
        }
    }
    
    if (isGameStarted && key == 'q') {
        isGameOver = true;      //게임 오버
    }
    
    else if (isGameOver) {
        if (key == OF_KEY_RETURN) {     //게임 오버 이후 엔터가 눌리면
            
            //파일 출력
            ofFile file("score.txt", ofFile::ReadWrite);
            if (file.exists()) {
                file.remove();
            }
            file.create();
            ofBuffer buffer;
            //벡터의 모든 값을 버퍼에 담아 파일에 한번에  저장, 저장되는 점수들 터미널에 출력
            for (int tmp : scores) {
                string data = ofToString(tmp) + '\n';
                buffer.append(data);
                cout << data;
            }
            
            ofBufferToFile("score.txt", buffer);
            file.close();
            ofExit();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ')
        isSpaceKeyPressed = false;
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //왼쪽 마우스버튼이 클릭되면,
    if(button == OF_MOUSE_BUTTON_LEFT) {
        //게임 오버일때
        if(isGameOver) {
            //재시작 버튼 클릭됨
            if(x >= restartButtonPosition.x && y >= restartButtonPosition.y && x <= restartButton.getWidth() + restartButtonPosition.x && y <= restartButtonPosition.y +  restartButton.getHeight()) {
                restart();
            }
        }
        //게임이 시작되기 전, 게임 오버 이후
        if((!isGameStarted || isGameOver)) {
            //printScore 버튼이 클릭됨
            if(x >= printScoreButton.x && y >= printScoreButton.y && x <= printScoreButton.width + printScoreButton.x && y <= printScoreButton.y +  printScoreButton.height) {
                //출력된 점수 화면에서 지움
                if(isPrintScore) {
                    cleanScoreBox = true;
                    isPrintScore = false;
                }
                //점수 출력
                else {
                    isPrintScore = true;
                }
            }
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::restart() {
    tRexPosition = ofPoint(50, ofGetHeight() / 2);
    isJumping = false;
    jumpForce = -10.0f;
    imageTimer = 0.0f;
    groundOffset = 0.0f;
    groundSpeed = 5.0f;
    isSpaceKeyPressed = false;
    jumpTimer = 0.0f;
    maxJumpTime = 1.0f; // 최대 점프 지속 시간
    isTrex = true;
    isGameStarted = false;
    isGameOver = false;
    lastX = 0;
    score = 0;
    cactusCount = 0;
    isPrintScore = false;
    cleanScoreBox = false;

    for(int i = 0; i<MAX_CACTUS_COUNT; i++) {
        cactuses[i].cactus = smallCactus;
        cactuses[i].position.x = ofGetWidth() + i * 400;
        cactuses[i].position.y = ofGetHeight() / 2 + 6;
    }
}

//--------------------------------------------------------------
bool ofApp::checkCollision() {
    ofRectangle trexBounds(tRexPosition.x-2, tRexPosition.y, tRexRun1.getWidth()-3, tRexRun1.getHeight());
    for(int i = 0; i<MAX_CACTUS_COUNT; i++) {
        // 선인장의 충돌 박스 계산
        ofRectangle cactusBounds(cactuses[i].position.x+2, cactuses[i].position.y, cactuses[i].cactus.getWidth()-2, cactuses[i].cactus.getHeight());
        
        // 충돌 검사
        if(trexBounds.intersects(cactusBounds))
            return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::drawScoreBox() {
    font.drawString("Top 5", ofGetWidth()/2 - 38, 35);
    int idx = 0;
    ofRectangle scoreBox;
    scoreBox.width = 150;
    scoreBox.height = 30;
    scoreBox.x = ofGetWidth()/2 - 70;
    //벡터의 모든 값을 반복하며 화면에 출력
    for(int tmp : scores) {
        if(idx++ >= 5)
            break;
        scoreBox.y = 25 + idx*30;
        font.drawString(ofToString(idx) + "    " + ofToString(tmp, 5, '0'), ofGetWidth()/2 - 59, 50 + idx*30);
        ofDrawRectangle(scoreBox);
    }
    ofDrawLine(ofGetWidth()/2 - 33, 56, ofGetWidth()/2 - 33, 205);
}
