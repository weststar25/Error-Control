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
//	unsigned int p :5;				//���� ������
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
//	WSADATA wsaData;		//���� �ʱ�ȭ ���� ����ü
//	SOCKET S_Socket;
//	SOCKET C_Socket;
//	SOCKADDR_IN s_Addr;
//	SOCKADDR_IN c_Addr;
//
//	//[0] frame ���� Ƚ�� | [1]ack ����Ƚ�� | [2]�����۵� ��Ŷ �� for NAK | [3]�����۵� ��Ŷ �� for timeout
//	int cnt[4]={0};	
//	int timeout[30] = {0};
//	header frame[100];	//���Źް� ack�� ������ frame
//
//	srand(time(NULL));
//	printf("Selective Repeat \n--Receiver\n\n");
//	openSocket(wsaData, S_Socket, C_Socket, s_Addr, c_Addr);
//
//	printf("window size = 5\n");
//	printf("sequence_number : 5bit\n");
//	printf("transmission rate : 70%%\n");
//	printf("NakȮ��: 20%, timeoutȮ��: 10%%\n\n");
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
//	{//���� �ʱ�ȭ(������ ����2.2 | �ʱ�ȭ�� ���¸� ������ ����)
//		printf("WSAStartup() error");
//		exit(1);
//	}
//	if((S_Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
//	{//���� ����(�ּ� ü�� | ���� ������ ���� | ��������)
//		printf("socket() error");
//		exit(1);
//	}
//
//	//���� ��û�� ������ �ּ� ����
//	memset(&s_Addr, 0, sizeof(s_Addr));
//	s_Addr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP�ּ�
//	s_Addr.sin_family = AF_INET;				//����ü ����
//	s_Addr.sin_port = htons(PORT);				//��Ʈ ��ȣ
//
//	if(bind(S_Socket, (SOCKADDR*)&s_Addr, sizeof(s_Addr)) == SOCKET_ERROR)
//	{//������ ��Ʈ�� ����
//		printf("bind() error");
//		exit(1);
//	}
//
//	if(listen(S_Socket, 5) == SOCKET_ERROR)
//	{//Ŭ���̾�Ʈ ���� ��û ����
//		printf("listen() error");
//		exit(1);
//	}
//
//	szc_Addr = sizeof(c_Addr);
//	if((C_Socket = accept(S_Socket,(SOCKADDR*)&c_Addr, &szc_Addr)) == INVALID_SOCKET)
//	{//Ŭ���̾�Ʈ�κ����� ���� ��û�� ����
//		printf("accept() error");
//		exit(1);
//	}
//}
//void packet(header (&frame)[100], int (&timeout)[30], SOCKET &C_Socket, int (&cnt)[4])
//{
//	int start[30] = {0};
//	int end[30] = {0};
//	int flag=0;	//frame loss �Ǵ� timeout �߻��� ��츦 �Ǵ��ϴ� ����
//	int i=0;
//
//	while(cnt[0]<30)	//30���� frame�� ����
//	{		
//		printf("������������������������������������������������������������\n");
//		while(cnt[0]<30)
//		{
//
//			start[cnt[0]] = clock();	//�����ϴ� ���۽ð�
//			//sender�� ���� frame�� ���Ź���
//			recv(C_Socket, (char *)&frame[cnt[0]], sizeof(frame[cnt[0]]), 0);
//
//			frame[cnt[0]].p = random(10);	//0~9�� ���ڰ� �������� ����
//			if(frame[cnt[0]].p < 7)			//p�� 0~6�϶� ����(70%)
//			{
//				printf("frame ���� �� seq number : %d��°, Data : %s\n", frame[cnt[0]].Seq_num, frame[cnt[0]].Data);
//				end[cnt[0]] = clock();
//				timeout[cnt[0]] = end[cnt[0]] - start[cnt[0]];
//			}
//			else							//p�� 7~9�϶� frame loss(30%)
//			{
//				textcolor(LIGHTRED, BLACK);
//				printf("frame �ս� �� seq number : %d��°\n", frame[cnt[0]].Seq_num);
//				textcolor(LIGHTGRAY, BLACK);
//			}
//			cnt[0]++;
//			if(cnt[0]%5 == 0){ break; }	//window size��ŭ ������ ������ ack ����
//		}
//
//		printf("������������������������������������������������������������\n");
//
//		while(cnt[1] < cnt[0])	//ACK �Ǵ� NAK ����
//		{
//			if(frame[cnt[1]].p < 7)	//���ſϷ�
//			{
//
//				//������ �ޱ⸦ ����ϴ� sequence_number�� ACK�� ��� ������.
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("ACK ����   �� ACK : %d \n\n", frame[cnt[1]].ACK_num+1);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//ack ����
//			}
//			else if(frame[cnt[1]].p < 9) //(20% NAK ����)
//			{
//
//				//�������� ����ϴ� sequence_number�� NAK�� ��� ������.
//				cnt[2]++;
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;
//				printf("���� ����  �� seq number : %d��° ������ ��û\n", frame[cnt[1]].Seq_num);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//sender�� NAK ����
//				recv(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//sender�κ��� �ش� frame ����
//				printf("frame ���� �� seq number : %d��°, Data : %s\n", frame[cnt[1]].Seq_num, frame[cnt[1]].Data);
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("ACK ����   �� ACK : %d\n\n", frame[cnt[1]].ACK_num+1);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//ack ����
//				end[cnt[1]] = clock();
//				timeout[cnt[1]] = end[cnt[1]] - start[cnt[1]];
//			}
//			else	//(10% timeout)
//			{
//
//				cnt[3]++;
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("���� ����  �� seq number : %d��° ������ ��û\n", frame[cnt[1]].ACK_num);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	
//				recv(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//sender�κ��� �ش� frame ����
//				printf("frame ���� �� seq number : %d��°, Data : %s\n", frame[cnt[1]].Seq_num, frame[cnt[1]].Data);
//				frame[cnt[1]].ACK_num = frame[cnt[1]].Seq_num;	
//				printf("ACK ����   �� ACK : %d\n\n", frame[cnt[1]].ACK_num+1);
//				send(C_Socket, (char *)&frame[cnt[1]], sizeof(frame[cnt[1]]), 0);	//ack ����
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
//	printf("\n�����������������������������۰����������������������������\n");
//	printf("���ŵ� ��Ŷ �� : %d\n", cnt[0]);
//	printf("�������� ��Ŷ �� for NAK : %d\n", cnt[2]);
//	printf("�������� ��Ŷ �� for time out : %d\n", cnt[3]);
//	printf("������������������������������������������������������������\n");
//
//	for(i=0; i<cnt[0]; i++)
//	{
//		printf("%d�������� receive�ð��� %d ms �Դϴ�.\n", i, timeout[i]);
//		total += timeout[i];
//	}   
//
//	printf("�������� receive�ð��� %d ms �Դϴ�.\n", total);
//	printf("������������������������������������������������������������\n");
//	closesocket(C_Socket);   //���� ���� ����.;
//	printf("%s ���� ��������\n",inet_ntoa(c_Addr.sin_addr));
//	WSACleanup();   //���� ����
//}
//int random(int n) 
//{ 
//	int res; 
//	res = rand() % n; // 0���� n-1������ ���� ���� �߻� 
//	return res;
//} 
//void textcolor(int foreground, int background) 
//{ 
//	int color=foreground+background*16; 
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); 
//} 