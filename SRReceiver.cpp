//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//#include<winsock2.h>
//#include<conio.h>
//#include<time.h>
//#pragma comment(lib,"ws2_32.lib")
//#define PORT 3000
//#define BLACK 0
//#define LIGHTGRAY 7
//#define LIGHTRED 12 
//typedef struct FRAME{					
//	unsigned int p :5;				//전송 성공률
//	unsigned int Seq_num :7;		//seq
//	unsigned int ACK_num :7;
//	unsigned long sender;	
//	unsigned long receiver;
//	char Data[200];
//}header;
//
//void openSocket(WSADATA &wsaData, SOCKET &S_Socket, SOCKET &C_Socket, SOCKADDR_IN &s_Addr, SOCKADDR_IN &c_Addr);
//void packet(header (&frame)[100], int (&timeout)[30], SOCKET &C_Socket, int (&cnt)[4]);
//void finsh(SOCKET &C_Socket, SOCKADDR_IN &c_Addr, int (&timeout)[30], int (&cnt)[4]);
//void textcolor(int foreground, int background);
//int random(int n);
//
//int main(int argc, char **argv)
//{
//	WSADATA wsaData;		//윈속 초기화 정보 구조체
//	SOCKET S_Socket;
//	SOCKET C_Socket;
//	SOCKADDR_IN s_Addr;
//	SOCKADDR_IN c_Addr;
//
//	//[0] frame 수신 횟수 | [1]ack 전송횟수 | [2]재전송된 패킷 수 for NAK | [3]재전송된 패킷 수 for timeout
//	int cnt[4]={0};	
//	int timeout[30] = {0};
//	header frame[100];	//수신받고 ack를 전송할 frame
//
//	srand(time(NULL));
//	printf("Selective Repeat \n--Receiver\n\n");
//	openSocket(wsaData, S_Socket, C_Socket, s_Addr, c_Addr);
//
//	printf("window size = 5\n");
//	printf("sequence_number : 5bit\n");
//	printf("transmission rate : 70%%\n");
//	printf("Nak확률: 20%, timeout확률: 10%%\n\n");
//
//	packet(frame, timeout, C_Socket, cnt);
//	finsh(C_Socket, c_Addr, timeout, cnt);
//	return 0;
//}
//void openSocket(WSADATA &wsaData, SOCKET &S_Socket, SOCKET &C_Socket, SOCKADDR_IN &s_Addr, SOCKADDR_IN &c_Addr)
//{
//	int szc_Addr;
//
//	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
//	{//윈속 초기화(윈속의 버전2.2 | 초기화된 상태를 변수에 저장)
//		printf("WSAStartup() error");
//		exit(1);
//	}
//	if((S_Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
//	{//소켓 생성(주소 체계 | 연결 지향형 소켓 | 프로토콜)
//		printf("socket() error");
//		exit(1);
//	}
//
//	//연결 요청을 수신할 주소 설정
//	memset(&s_Addr, 0, sizeof(s_Addr));
//	s_Addr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP주소
//	s_Addr.sin_family = AF_INET;				//구조체 세팅
//	s_Addr.sin_port = htons(PORT);				//포트 번호
//
//	if(bind(S_Socket, (SOCKADDR*)&s_Addr, sizeof(s_Addr)) == SOCKET_ERROR)
//	{//소켓을 포트에 연결
//		printf("bind() error");
//		exit(1);
//	}
//
//	if(listen(S_Socket, 5) == SOCKET_ERROR)
//	{//클라이언트 연결 요청 받음
//		printf("listen() error");
//		exit(1);
//	}
//
//	szc_Addr = sizeof(c_Addr);
//	if((C_Socket = accept(S_Socket,(SOCKADDR*)&c_Addr, &szc_Addr)) == INVALID_SOCKET)
//	{//클라이언트로부터의 연결 요청을 수신
//		printf("accept() error");
//		exit(1);
//	}
//}
//void packet(header (&frame)[100], int (&timeout)[30], SOCKET &C_Socket, int (&cnt)[4])
//{
//	int start[30] = {0};
//	int end[30] = {0};
//	int flag=0;	//frame loss 또는 timeout 발생한 경우를 판단하는 변수
//	int i=0;
//
//	while(cnt[0]<30)	//30개의 frame을 수신
//	{		
//		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//		while(cnt[0]<30)
//		{
//
//			start[cnt[0]] = clock();	//수신하는 시작시간
//			//sender로 부터 frame을 수신받음
//			recv(C_Socket, (char *)&frame[cnt[0]], sizeof(frame[cnt[0]]), 0);
//
//			frame[cnt[0]].p = random(10);	//0~9의 숫자가 랜덤으로 저장
//			if(frame[cnt[0]].p < 7)			//p가 0~6일때 수신(70%)
//			{
//				printf("frame 수신 → seq number : %d번째, Data : %s\n", frame[cnt[0]].Seq_num, frame[cnt[0]].Data);
//				end[cnt[0]] = clock();
//				timeout[cnt[0]] = end[cnt[0]] - start[cnt[0]];
//			}
//			else							//p가 7~9일때 frame loss(30%)
//			{
//				textcolor(LIGHTRED, BLACK);
//				printf("frame 손실 → seq number : %d번째\n", frame[cnt[0]].Seq_num);
//				textcolor(LIGHTGRAY, BLACK);
//			}
//			cnt[0]++;
//			if(cnt[0]%5 == 0){ break; }	//window size만큼 수신이 끝나면 ack 전송
//		}
//
//		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//
//		while(cnt[1] < cnt[0])	//ACK 또는 NAK 전송
//		{
//			if(frame[cnt[1]].p < 7)	//수신완료
//			{
//
//				//다음에 받기를 기대하는 sequence_number를 ACK에 담아 보낸다.
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("ACK 전송   → ACK : %d \n\n", frame[cnt[1]].ACK_num+1);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//ack 전송
//			}
//			else if(frame[cnt[1]].p < 9) //(20% NAK 전송)
//			{
//
//				//재전송을 희망하는 sequence_number를 NAK에 담아 보낸다.
//				cnt[2]++;
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;
//				printf("수신 실패  → seq number : %d번째 재전송 요청\n", frame[cnt[1]].Seq_num);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//sender로 NAK 전송
//				recv(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//sender로부터 해당 frame 수신
//				printf("frame 수신 → seq number : %d번째, Data : %s\n", frame[cnt[1]].Seq_num, frame[cnt[1]].Data);
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("ACK 전송   → ACK : %d\n\n", frame[cnt[1]].ACK_num+1);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//ack 전송
//				end[cnt[1]] = clock();
//				timeout[cnt[1]] = end[cnt[1]] - start[cnt[1]];
//			}
//			else	//(10% timeout)
//			{
//
//				cnt[3]++;
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("수신 실패  → seq number : %d번째 재전송 요청\n", frame[cnt[1]].ACK_num);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	
//				recv(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//sender로부터 해당 frame 수신
//				printf("frame 수신 → seq number : %d번째, Data : %s\n", frame[cnt[1]].Seq_num, frame[cnt[1]].Data);
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("ACK 전송   → ACK : %d\n\n", frame[cnt[1]].ACK_num+1);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//ack 전송
//				end[cnt[1]] = clock();
//				timeout[cnt[1]] = end[cnt[1]] - start[cnt[1]];
//			}
//			cnt[1]++;
//		}
//	}
//}
//void finsh(SOCKET &C_Socket, SOCKADDR_IN &c_Addr, int (&timeout)[30], int (&cnt)[4])
//{
//	int i, total=0;
//
//	printf("\n━━━━━━━━━━━━━전송결과━━━━━━━━━━━━━\n");
//	printf("수신된 패킷 수 : %d\n", cnt[0]);
//	printf("재전송한 패킷 수 for NAK : %d\n", cnt[2]);
//	printf("재전송한 패킷 수 for time out : %d\n", cnt[3]);
//	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//
//	for(i=0; i<cnt[0]; i++)
//	{
//		printf("%d번프레임 receive시간은 %d ms 입니다.\n", i, timeout[i]);
//		total += timeout[i];
//	}   
//
//	printf("총프레임 receive시간은 %d ms 입니다.\n", total);
//	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//	closesocket(C_Socket);   //소켓 연결 종료.;
//	printf("%s 와의 연결종료\n",inet_ntoa(c_Addr.sin_addr));
//	WSACleanup();   //윈속 제거
//}
//int random(int n) 
//{ 
//	int res; 
//	res = rand() % n; // 0부터 n-1까지의 수중 난수 발생 
//	return res;
//} 
//void textcolor(int foreground, int background) 
//{ 
//	int color=foreground+background*16; 
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); 
//} 