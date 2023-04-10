#include "Header.h"

void cleanList(int list[]) {
    for(int i = 0; i<10; i++) {     
        list[i] = 0;                //list변수의 주소는 바뀌지 않으므로 한번 출력이 끝나면 0으로 초기화
    }
}