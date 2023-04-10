#define SAM_DEF
#include "Header.h"

int main(){
    int T;                  //Testcase 변수
    int list[10] = {0};     //결과값이 저장될 
    scanf("%d", &T);        //T 입력 
    for(int i = 0; i<T; i++) {  //T만큼 반복
        int N;              //입력받을 변수 선언
        scanf("%d", &N);    //N 입력
        count(N, list);     //list에 숫자의 갯수 저장할 count 함수 호출
        printList(list);    //list 출력함수 호출
        cleanList(list);    //list 초기화 함수 호출
    }
    
    return 0;
}