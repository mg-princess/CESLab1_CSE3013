#include "Header.h"

void count(int N, int list[]) {
    for(int i = 1; i<=N; i++) {     //1부터 N까지 반복
        int n = i;                  //N이 바뀌지 않기위해 임의 변수 선언
        while(n != 0) {             //n에서 1의자리 하나씩 빼기 때문에 n 이 0이 아닐 때까지 반복
            list[n%10]++;           //n의 1의자리는 그대로 list인덱스에 넣어 해당 숫자가 나왔음을 확인.
            n/=10;                  //다음은 10의자리를 판단해야하므로 n을 10으로 나눔
        }
    }
}