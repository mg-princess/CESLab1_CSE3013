#pragma once

#include "ofMain.h"
#define MAX_CACTUS_COUNT 4

struct cactus {                         //선인장 종류와 위치 구조체
    ofImage cactus;
    ofPoint position;
};

class ofApp : public ofBaseApp{
	public:
        int score;                      //현재 점수
        ofImage tRex, ground;           //시작 t-rex이미지 / 땅 이미지
        ofImage smallCactus;            //작은 선인장 이미지
        ofImage largeCactus1, largeCactus2; //큰선인장 이미지
        ofImage gameOver;               //게임오버 플래그
        ofImage tRexRun1, tRexRun2, tRexEnd;    //움직이는 t-rex이미지1,2, 게임오버된 t-rex이미지
        ofImage wholeImage;             //파일에서 불러올 전체 이미지
        ofImage restartButton;          //재시작 버튼 이미지
        ofPoint restartButtonPosition;  //재시작 버튼 위치
        ofPoint tRexPosition;           //t-rex 위치
        ofRectangle printScoreButton;   //PrintScore 버튼 위치
        cactus cactuses[MAX_CACTUS_COUNT];  //선인장 종류와 위치 배열
        ofTrueTypeFont font;            //폰트
    
        //점프 중 플래그 / 게임 시작 플래그 / 게임오버 플래그 / 점수출력 플래그 / 점수출력 지우는 플래그
        bool isJumping, isTrex, isGameStarted, isGameOver, isPrintScore, cleanScoreBox;
        float jumpForce, imageTimer;    //중력 / t-rex 이미지 변경 시간 측정
        float groundOffset, groundSpeed;    //땅 이미지 위치 / 땅 및 선인장 이동 속도
        bool isSpaceKeyPressed;         //스페이스바 입력 플래그
        float jumpTimer;                //점프 시간 측정
        float maxJumpTime;              //최대 점프 시간
        int cactusCount;                //이전 선인장 인덱스
        float lastX;                    //이전 선인장의 x좌표
        bool isCalEnd;                  //점수 계산 완료 플래그
        vector<int> scores;             //파일에서 읽어온 점수 저장
        int topScore;                   //최대 점수
        
        void setup();
		void update();
		void draw();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        bool checkCollision();          //t-rex와 선인장이 만나는지 확인
        void restart();                 //재시작 함수
        void drawScoreBox();            //점수 출력 함수
};
