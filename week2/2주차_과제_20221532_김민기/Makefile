cc = gcc					#gcc로 컴파일
target = main				#컴파일의 결과파일이름은 main
cflags = -g					#바이너리 파일에 디버깅 정보 포함

objects = main.o cleanList.o count.o printList.o	#오브젝트 파일들 모음

$(target) : $(objects)				
	$(cc) $(cflags) -o $(target) $(objects)			#실행파일 생성

%.o : %.c											#오브젝트파일 생성
	$(cc) $(cflags) -c -o $@ $<

main.o cleanList.o count.o printList.o : Header.h	#헤더 의존성 지정

clean:										#clean 명령어 실행시 target과 objects모두 삭제
	rm $(target) $(objects)