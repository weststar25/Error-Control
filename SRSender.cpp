//#include<stdio.h> //�⺻ ���
//#include<stdlib.h> // rand()�Լ� ���� �ʿ�
//#include<string.h> // ���ڿ��Լ� ���� �ʿ�
//#include<winsock2.h> // ���� ���α׷��ֽ� �ʿ�
//#include<conio.h> // ��� ������Ʈ ����� �ʿ� ���� �Ƚᵵ ����..(stdio.h)�� �����ϰ�����..
//#include<time.h>
//
//#pragma comment(lib,"ws2_32.lib")  // ���� ���α׷��ֽ� �ʿ�
//#pragma warning (disable : 4996)
//
//#define IP "127.0.0.1"
//#define PORT 3000
//
//typedef struct FRAME
//{
//	unsigned int p : 5;				// ���� ������ ���庯��
//	unsigned int Seq_num : 7;		// sequence_number : 7bit
//	unsigned int ACK_num : 7;		// ACK_number : 7bit
//	unsigned long sender;	
//	unsigned long receiver;
//	char Data[200];					// Data 100byte
//}header;                            // typedef struct�� ���� FRAME�̶� ����ü�� header��� �������� �θ����ִ�. 
//
//void openSocket(WSADATA &wsaData, SOCKET &hSocket, SOCKADDR_IN &servAddr, header &frame);
//void packet(header &frame, header (&store_frame)[200], int (&timeout)[30], SOCKET &hSocket, int (&cnt)[4]);
//void finish(SOCKET &hScoket, SOCKADDR_IN &servAddr, int(&timeout)[30], int (&cnt)[4], header &frame);
//
//int main(int argc, char *argv[])
//{
//
//	WSADATA wsaData; // WSAStartup , WSAcleanup�� ����ϰ� �̶� ����� ���Ŵ� ���� �̷����
//	SOCKET hSocket;
//	SOCKADDR_IN servAddr; // ip,port�� ���� ������ �Է��ϰ� ���ִ� ����ü
//	
//	// [0] - frame �۽� Ƚ�� | [1] - ACK �� NAK ���� Ƚ��
//	// [2] - �����۵� ��Ŷ �� for NAK | [3] - �����۵� ��Ŷ �� for timeout
//	int cnt[4]={0};
//	int total=0;
//	int timeout[30] = {0};
//	srand((unsigned)time(NULL));
//
//
//	header frame;				// ������ frame
//	header store_frame[200];	// frame ���� �� ����
//
//	printf("Selective Repeat \n--Sender\n\n");
//
//	openSocket(wsaData, hSocket, servAddr, frame);
//	
//	printf("window size = 5\n");
//	printf("sequence_number : 5bit\n");
//	printf("transmission rate : 70%%\n");
//	printf("NakȮ��: 20%, timeoutȮ��: 10%%\n\n");
//
//	packet(frame, store_frame, timeout, hSocket, cnt);
//	finish(hSocket, servAddr, timeout, cnt, frame);
//	
//	return 0;
//}
//void openSocket(WSADATA &wsaData, SOCKET &hSocket, SOCKADDR_IN &servAddr, header &frame)
//{
//	// �������� - ����(������������α׷���) 2.2����, WSADATA ����ü�� ������ �ּ�
//	// ������ΰ���
//	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)       
//	{												
//		printf("WSAStartup() error!");				
//		exit(1);									
//	}												
//
//	/* PF_INET(ipV4 ���ͳ� ��������)�� ����ϸ� 
//	SOCK_STREAM(TCP/IP ��������)�� �̿��Ͽ� ���� ����
//	�ϴµ� Ư�� �������� ����� �������ִ� 0�� �����Ͽ���*/
//	hSocket=socket(PF_INET, SOCK_STREAM,0);
//	
//	if(hSocket==INVALID_SOCKET) // ����� ������ ���� �ʾ��� �� 
//	{
//		printf("hSocketet() error");
//		exit(1);
//	}
//	frame.sender=inet_addr(IP); // sender frame�� 127.0.0.1(LoopBack�ּ�) ���� (inet_addr�� �ּҺ�ȯ���影������)
//	memset(&servAddr,0,sizeof(servAddr)); // servAddr�� 0���� �ʱ�ȭ
//	
//	servAddr.sin_family=AF_INET; // servAddr�� IP �ּҸ� ���� (AF_INET�� ������ ���� IPv4�� ��Ÿ��)
//	servAddr.sin_addr.s_addr=inet_addr(IP); // 32bit IPv4 �ּ�
//	servAddr.sin_port=htons(PORT); // servAddr�� ��Ʈ ��ȣ�� ����(3000��)
//
//
//	// Ŭ���̾�Ʈ���� �������� �����û
//	// coonect �Լ��� �������� ������ ������� ������ ��û�Ҷ� ���� �Լ�
//	if(connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr))==SOCKET_ERROR) 
//	{
//		printf("connect() error!");
//		exit(1);
//	}
//	frame.receiver=inet_addr(IP);
//	// �������� ���۷� 100%�� �����Ѵ�.
//
//	// frame �ʱ�ȭ
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
//	int flag=0;	// frame loss �Ǵ� timeout �߻��� ��츦 �Ǵ��ϴ� ���� 
//
//	while(cnt[0]<30)	// 30���� frame�� ����
//	{
//		printf("������������������������������������������������������������\n");
//		while(cnt[0]<30) // frame ����
//		{
//			start[cnt[0]] = clock(); // �� ������ ���۽ð��� ���α׷��� ���۵� �� ����� �ð����� ����
//
//			itoa(rand()%500+512,frame.Data,2);  // int->char�� ��ȯ���ִ� �Լ�, ���� data�� ����
//
//			store_frame[cnt[0]]=frame;	// �������� ���� frame ����.
//			printf("Frame ���� �� seq number : %d��°, Data : %s\n",frame.Seq_num, frame.Data);	
//			// receiver�� frame�� �����Ѵ�.
//			send(hSocket,(char *)&frame, sizeof(frame),0);
//			frame.Seq_num++; // sequence number ����
//			cnt[0]++;
//			if(cnt[0]%5==0) { break; } // window size��ŭ ������ ������ ack ����
//		}
//		printf("������������������������������������������������������������\n");
//
//		while(cnt[1]<cnt[0])	//ACK �Ǵ� NAK ����
//		{
//			// receiver���� ACK�Ǵ� NAK�� ������ ��ٸ���.
//			recv(hSocket,(char *)&frame,sizeof(frame),0);
//
//			if(frame.p<7)	// ACK�� ���� �� ���� �Ϸ�(70%Ȯ��)
//			{
//				printf("Receiver�κ��� ACK : %d ���۹��� \n\n",frame.ACK_num+1);
//				end[cnt[1]] = clock();
//				timeout[cnt[1]]= end[cnt[1]]-start[cnt[1]];
//			}
//			else if(frame.p<9) // NAK�� ���� �� ������(20%Ȯ��)
//			{
//				cnt[2]++;
//				frame=store_frame[cnt[1]];	// ���۽� �ӽ������ߴ� frame�� ����
//				//printf("Frame Resend for NAK �� seq : %d, Data : %s\n",frame.Seq_num, frame.Data);
//				printf("Receiver�κ��� NAK�� ���۹޾� Frame ������\n");
//				printf("�������� �����ؾ��� Seq Number : %d��°, Data : %s\n", frame.Seq_num, frame.Data);
//				send(hSocket,(char *)&frame, sizeof(frame), 0); // receiver�� �ش� frame����
//				recv(hSocket,(char *)&frame, sizeof(frame), 0); // receiver�� ���� ACK�� �޴´�.
//				printf("Receiver�κ��� ACK : %d ���۹��� \n\n",frame.ACK_num+1);
//				end[cnt[1]] = clock();
//				timeout[cnt[1]]=end[cnt[1]]-start[cnt[1]];
//			}
//			else // timeout �߻� (10%Ȯ��)	
//			{
//				while(1) // timeout �߻�
//				{
//					end[cnt[1]] = clock();
//					timeout[cnt[1]]=end[cnt[1]]-start[cnt[1]];
//					if(timeout[cnt[1]]>2000) // timeout= 2s�� ����
//					{
//						cnt[3]++;
//						frame=store_frame[cnt[1]];	// ���۽� �ӽ������ߴ� frame�� ����
//						printf("Timeout���� ���� Frame ������\n");
//						printf("������ �����ؾ��� Seq Number : %d��°, Data : %s\n", frame.Seq_num, frame.Data);
//						send(hSocket,(char *)&frame, sizeof(frame),0); // receiver�� �ش� frame����
//						recv(hSocket,(char *)&frame, sizeof(frame),0); // receiver�� ���� ACK�� �޴´�.
//						printf("Receiver�κ��� ACK : %d ���۹��� \n\n",frame.ACK_num+1);
//						break;
//					}
//				}
//			}
//			cnt[1]++;
//		}
//		frame.Seq_num++; // sequence number ����
//		
//	}
//	return;
//}
//void finish(SOCKET &hSocket, SOCKADDR_IN &servAddr, int(&timeout)[30], int (&cnt)[4], header &frame)
//{
//	int i, total=0;
//	printf("\n�����������������������������۰����������������������������\n");
//	printf("������ ��Ŷ �� : %d\n", frame.ACK_num+1);
//	printf("�������� ��Ŷ �� for NAK : %d\n", cnt[2]);
//	printf("�������� ��Ŷ �� for time out : %d\n", cnt[3]);
//	printf("������������������������������������������������������������\n");
//
//	for(i=0;i<cnt[0];i++)
//	{
//		printf("%d�� Frame ���۽ð��� %d ms �Դϴ�.\n", i, timeout[i]);
//		total+=timeout[i];
//	}
//
//	printf("�������� ���۽ð��� %d ms �Դϴ�.\n",total);
//	printf("������������������������������������������������������������\n");
//	closesocket(hSocket); // ���� ���� ����.
//	printf("%s ���� ��������\n",inet_ntoa(servAddr.sin_addr));
//	WSACleanup(); // ���� ����
//	return;
//}