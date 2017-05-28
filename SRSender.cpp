//#include<stdio.h> //기본 헤더
//#include<stdlib.h> // rand()함수 사용시 필요
//#include<string.h> // 문자열함수 사용시 필요
//#include<winsock2.h> // 소켓 프로그래밍시 필요
//#include<conio.h> // 명령 프로프트 실행시 필요 굳이 안써도 무방..(stdio.h)가 포함하고있음..
//#include<time.h>
//
//#pragma comment(lib,"ws2_32.lib")  // 소켓 프로그래밍시 필요
//#pragma warning (disable : 4996)
//
//#define IP "127.0.0.1"
//#define PORT 3000
//
//typedef struct FRAME
//{
//	unsigned int p : 5;				// 전송 성공률 저장변수
//	unsigned int Seq_num : 7;		// sequence_number : 7bit
//	unsigned int ACK_num : 7;		// ACK_number : 7bit
//	unsigned long sender;	
//	unsigned long receiver;
//	char Data[200];					// Data 100byte
//}header;                            // typedef struct를 통해 FRAME이란 구조체를 header라는 별명으로 부를수있다. 
//
//void openSocket(WSADATA &wsaData, SOCKET &hSocket, SOCKADDR_IN &servAddr, header &frame);
//void packet(header &frame, header (&store_frame)[200], int (&timeout)[30], SOCKET &hSocket, int (&cnt)[4]);
//void finish(SOCKET &hScoket, SOCKADDR_IN &servAddr, int(&timeout)[30], int (&cnt)[4], header &frame);
//
//int main(int argc, char *argv[])
//{
//
//	WSADATA wsaData; // WSAStartup , WSAcleanup을 사용하고 이때 개방과 제거는 쌍을 이뤄야함
//	SOCKET hSocket;
//	SOCKADDR_IN servAddr; // ip,port와 같은 정보를 입력하게 해주는 구조체
//	
//	// [0] - frame 송신 횟수 | [1] - ACK 및 NAK 수신 횟수
//	// [2] - 재전송된 패킷 수 for NAK | [3] - 재전송된 패킷 수 for timeout
//	int cnt[4]={0};
//	int total=0;
//	int timeout[30] = {0};
//	srand((unsigned)time(NULL));
//
//
//	header frame;				// 전송할 frame
//	header store_frame[200];	// frame 전송 전 저장
//
//	printf("Selective Repeat \n--Sender\n\n");
//
//	openSocket(wsaData, hSocket, servAddr, frame);
//	
//	printf("window size = 5\n");
//	printf("sequence_number : 5bit\n");
//	printf("transmission rate : 70%%\n");
//	printf("Nak확률: 20%, timeout확률: 10%%\n\n");
//
//	packet(frame, store_frame, timeout, hSocket, cnt);
//	finish(hSocket, servAddr, timeout, cnt, frame);
//	
//	return 0;
//}
//void openSocket(WSADATA &wsaData, SOCKET &hSocket, SOCKADDR_IN &servAddr, header &frame)
//{
//	// 전달인자 - 윈속(윈도우소켓프로그래밍) 2.2버전, WSADATA 구조체의 포인터 주소
//	// 소켓통로개방
//	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)       
//	{												
//		printf("WSAStartup() error!");				
//		exit(1);									
//	}												
//
//	/* PF_INET(ipV4 인터넷 프로토콜)을 사용하며 
//	SOCK_STREAM(TCP/IP 프로토콜)을 이용하여 소켓 생성
//	하는데 특정 프로토콜 사용을 지정해주는 0을 전달하였다*/
//	hSocket=socket(PF_INET, SOCK_STREAM,0);
//	
//	if(hSocket==INVALID_SOCKET) // 제대로 생성이 되지 않았을 때 
//	{
//		printf("hSocketet() error");
//		exit(1);
//	}
//	frame.sender=inet_addr(IP); // sender frame에 127.0.0.1(LoopBack주소) 저장 (inet_addr는 주소변환저장쉽게해줌)
//	memset(&servAddr,0,sizeof(servAddr)); // servAddr를 0으로 초기화
//	
//	servAddr.sin_family=AF_INET; // servAddr에 IP 주소를 저장 (AF_INET은 아이피 버전 IPv4를 나타냄)
//	servAddr.sin_addr.s_addr=inet_addr(IP); // 32bit IPv4 주소
//	servAddr.sin_port=htons(PORT); // servAddr에 포트 번호를 저장(3000번)
//
//
//	// 클라이언트에서 서버로의 연결요청
//	// coonect 함수는 서버측에 소켓을 기반으로 연결을 요청할때 쓰는 함수
//	if(connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr))==SOCKET_ERROR) 
//	{
//		printf("connect() error!");
//		exit(1);
//	}
//	frame.receiver=inet_addr(IP);
//	// 재전송은 전송률 100%로 가정한다.
//
//	// frame 초기화
//	frame.Seq_num=0;	
//	frame.ACK_num=0;			
//	frame.p=0;
//
//	return;
//}
//void packet(header &frame, header (&store_frame)[200], int (&timeout)[30], SOCKET &hSocket, int (&cnt)[4])
//{
//	int start[30] = {0};
//	int end[30] = {0};
//	int flag=0;	// frame loss 또는 timeout 발생한 경우를 판단하는 변수 
//
//	while(cnt[0]<30)	// 30개의 frame을 전송
//	{
//		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//		while(cnt[0]<30) // frame 전송
//		{
//			start[cnt[0]] = clock(); // 한 프레임 전송시간을 프로그램이 시작된 후 경과한 시간으로 대입
//
//			itoa(rand()%500+512,frame.Data,2);  // int->char로 변환해주는 함수, 랜덤 data값 생성
//
//			store_frame[cnt[0]]=frame;	// 재전송을 위해 frame 저장.
//			printf("Frame 전송 → seq number : %d번째, Data : %s\n",frame.Seq_num, frame.Data);	
//			// receiver로 frame을 전송한다.
//			send(hSocket,(char *)&frame, sizeof(frame),0);
//			frame.Seq_num++; // sequence number 증가
//			cnt[0]++;
//			if(cnt[0]%5==0) { break; } // window size만큼 전송이 끝나면 ack 수신
//		}
//		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//
//		while(cnt[1]<cnt[0])	//ACK 또는 NAK 수신
//		{
//			// receiver에서 ACK또는 NAK의 전송을 기다린다.
//			recv(hSocket,(char *)&frame,sizeof(frame),0);
//
//			if(frame.p<7)	// ACK를 수신 시 전송 완료(70%확률)
//			{
//				printf("Receiver로부터 ACK : %d 전송받음 \n\n",frame.ACK_num+1);
//				end[cnt[1]] = clock();
//				timeout[cnt[1]]= end[cnt[1]]-start[cnt[1]];
//			}
//			else if(frame.p<9) // NAK를 수신 시 재전송(20%확률)
//			{
//				cnt[2]++;
//				frame=store_frame[cnt[1]];	// 전송시 임시저장했던 frame를 전송
//				//printf("Frame Resend for NAK → seq : %d, Data : %s\n",frame.Seq_num, frame.Data);
//				printf("Receiver로부터 NAK을 전송받아 Frame 재전송\n");
//				printf("재전송을 시작해야할 Seq Number : %d번째, Data : %s\n", frame.Seq_num, frame.Data);
//				send(hSocket,(char *)&frame, sizeof(frame), 0); // receiver로 해당 frame전송
//				recv(hSocket,(char *)&frame, sizeof(frame), 0); // receiver로 부터 ACK를 받는다.
//				printf("Receiver로부터 ACK : %d 전송받음 \n\n",frame.ACK_num+1);
//				end[cnt[1]] = clock();
//				timeout[cnt[1]]=end[cnt[1]]-start[cnt[1]];
//			}
//			else // timeout 발생 (10%확률)	
//			{
//				while(1) // timeout 발생
//				{
//					end[cnt[1]] = clock();
//					timeout[cnt[1]]=end[cnt[1]]-start[cnt[1]];
//					if(timeout[cnt[1]]>2000) // timeout= 2s로 지정
//					{
//						cnt[3]++;
//						frame=store_frame[cnt[1]];	// 전송시 임시저장했던 frame를 전송
//						printf("Timeout으로 인한 Frame 재전송\n");
//						printf("재전송 시작해야할 Seq Number : %d번째, Data : %s\n", frame.Seq_num, frame.Data);
//						send(hSocket,(char *)&frame, sizeof(frame),0); // receiver로 해당 frame전송
//						recv(hSocket,(char *)&frame, sizeof(frame),0); // receiver로 부터 ACK를 받는다.
//						printf("Receiver로부터 ACK : %d 전송받음 \n\n",frame.ACK_num+1);
//						break;
//					}
//				}
//			}
//			cnt[1]++;
//		}
//		frame.Seq_num++; // sequence number 증가
//		
//	}
//	return;
//}
//void finish(SOCKET &hSocket, SOCKADDR_IN &servAddr, int(&timeout)[30], int (&cnt)[4], header &frame)
//{
//	int i, total=0;
//	printf("\n━━━━━━━━━━━━━전송결과━━━━━━━━━━━━━\n");
//	printf("생성된 패킷 수 : %d\n", frame.ACK_num+1);
//	printf("재전송한 패킷 수 for NAK : %d\n", cnt[2]);
//	printf("재전송한 패킷 수 for time out : %d\n", cnt[3]);
//	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//
//	for(i=0;i<cnt[0];i++)
//	{
//		printf("%d번 Frame 전송시간은 %d ms 입니다.\n", i, timeout[i]);
//		total+=timeout[i];
//	}
//
//	printf("총프레임 전송시간은 %d ms 입니다.\n",total);
//	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
//	closesocket(hSocket); // 소켓 연결 종료.
//	printf("%s 와의 연결종료\n",inet_ntoa(servAddr.sin_addr));
//	WSACleanup(); // 윈속 제거
//	return;
//}