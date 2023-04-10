#include "Str.hpp"

Str::Str(int leng) {
        len = leng;                                     //len 변수 leng으로 초기화
        str = new char[len+1];                            //str len 크기만큼 동적할당
} // leng 은 string의 길이.

Str::Str(char *neyong){
        len = strlen(neyong);                           //len 변수 neyong길이만큼 초기화
        str = new char[len+1];                             //str len 크기만큼 동적할당
        strncpy(str, neyong, len);                      //str에 neyong 값 복사
} // neyong은 초기화할 내용이 들어감.

Str::~Str(){    
        delete[] str;                                   //str 배열 동적할당 해제
} // 소멸자.

int Str::length(void){
        return len;                                     //len 리턴
} // string의 길이를 리턴하는 함수.

char* Str::contents(void){
        return str;                                     //str 리턴
} // string의 내용을 리턴하는 함수.

int Str::compare(class Str& a){
        return strcmp(str, a.contents());               //str과 a의 str을 strcmp로 비교
} // a의 내용과 strcmp.

int Str::compare(char *a){
        return strcmp(str, a);                          //str과 문자열 a를 strcmp로 비교
} // a의 내용과 strcmp.

void Str::operator=(char *a){
        strncpy(str, a, len);                           //str에 a값을 len 만큼 복사
        len = strlen(str);                              //len값 갱신
} // string의 값을 대입.

void Str::operator=(class Str& a){
        strncpy(str, a.contents(), len);                //str에 a의 str을 최대 str의 크기만큼 복사
        len = strlen(str);                              //len값 갱신
} // Str의 내용을 대입.