
// FtpClientDlg.cpp : ʵ���ļ�
//
// FTP ���������ӣ�Ҫ�ȷ������˿ڣ�Ȼ�� ���ͻ��˷�list����ʱ���������Ż�������������˿ڡ�
// ��������һ��ʼ��������������ⶫ����ip�˿�û�ܴ�Խ�ã�ftpЭ�̵�ʱ�����ݲ���д��ip��ͷ����ģ�������UPnP ��Խ����Ҫ����������Ŷ������еġ�

#include "stdafx.h"
#include "FtpClient.h"
#include "FtpClientDlg.h"
#include "afxdialogex.h"
#include<iostream>
#include<string>
#include<sstream>
#pragma comment(lib,"ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFtpClientDlg �Ի���




CFtpClientDlg::CFtpClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFtpClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Address = _T("192.168.10.107");
	m_Account = _T("test");
	m_Password = _T("test");
	m_Login = _T("");
	m_Directory = _T("");
}

void CFtpClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_Account);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_EDIT_LOGIN, m_Login);
	DDX_Text(pDX, IDC_EDIT_DIRECTORY, m_Directory);
}

BEGIN_MESSAGE_MAP(CFtpClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CFtpClientDlg::OnConnect)
	ON_BN_CLICKED(IDC_BTN_LIST, &CFtpClientDlg::OnList)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CFtpClientDlg::OnQuit)
END_MESSAGE_MAP()


// CFtpClientDlg ��Ϣ�������

BOOL CFtpClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFtpClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFtpClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFtpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



bool CFtpClientDlg::SendCommand(void)
{
	 int nSend;
   nSend=send(SocketControl,Command,strlen(Command),0);
   if (nSend==SOCKET_ERROR)
   {
      MessageBox("Socket����ʧ�ܣ�");
	  closesocket(SocketControl);
	  WSACleanup();
	  return false;
   }

   return true;
}


bool CFtpClientDlg::RecvRespond(void)
{
		int nRecv;
    memset(Respond,0,MAX_SIZE);
	nRecv=recv(SocketControl,Respond,MAX_SIZE,0);//ͨ�����ӽ�����Ӧ
	if (nRecv==SOCKET_ERROR)
	{
       MessageBox("Socket����ʧ�ܣ�");
	   closesocket(SocketControl);
	   WSACleanup();
	   return false;
	}

	//����Ӧ�н�����Ӧ��
	char* ReplyCodes=new char[3];
	memset(ReplyCodes,0,3);
	memcpy(ReplyCodes,Respond,3);
	RespondCode=atoi(ReplyCodes);
	return true;
}


bool CFtpClientDlg::DataConnect(void)
{
	 memset(Command,0,MAX_SIZE);
   //�ϳ�PASV����
   memcpy(Command,"PASV",strlen("PASV"));
   memcpy(Command+strlen("PASV"),"\r\n",2);

   if (!SendCommand())
	   return false;
  
   if (!RecvRespond())
	   return false;
   else
   {
      //�ж�PASV��Ӧ��
	   if (RespondCode!=227)
	   {
         MessageBox("PASV��Ӧ����");
		 closesocket(SocketControl);
		 WSACleanup();
		 return false;
	   }
	   
   }


   char* part[6];
   //����PASVӦ����Ϣ
  if (strtok(Respond,"("))
     {
	   for (int i=0;i<5;i++)
	   {
          part[i]=strtok(NULL,",");
		  if (!part[i])
			  return false;
	   }
	   part[5]=strtok(NULL,")");
	  
	   if (!part[5])
		   return false;

   }
    else
		return false;

	
	unsigned short ServerPort;   //��ȡ���������ݶ˿�
	ServerPort=unsigned short((atoi(part[4])<<8)+atoi(part[5]));
	SocketData=socket(AF_INET,SOCK_STREAM,0);
	if (SocketData==INVALID_SOCKET)
	{
      MessageBox("��������Socketʧ�ܣ�");
	  closesocket(SocketControl);
	  WSACleanup();
	  return false;
	}

	u_long IpAddress;
	IpAddress=inet_addr(m_Address);
	if (IpAddress==INADDR_NONE)
	{
      hostent* pHostent=gethostbyname(m_Address);
	  if (pHostent)
	  	 IpAddress=(*(in_addr*)pHostent->h_addr_list[0]).s_addr;	  
	}

	sockaddr_in serveraddr2;
	memset(&serveraddr2,0,sizeof(serveraddr2));
	serveraddr2.sin_family=AF_INET;
	serveraddr2.sin_port=htons(ServerPort);
	serveraddr2.sin_addr.s_addr=IpAddress;

	int nConnect=connect(SocketData,(sockaddr*)&serveraddr2,sizeof(serveraddr2));
	
	
	
	if (nConnect==SOCKET_ERROR)
	{
      MessageBox("��������ʧ�ܣ�");
	  closesocket(SocketControl);
	  closesocket(SocketData);
	  WSACleanup();
	  return false;
	}

	std::string  local_IP,local_IP2;
	struct sockaddr_in sin;
	int addrlen = sizeof(sin);
	if (getsockname(SocketControl, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		local_IP = inet_ntoa(sin.sin_addr);

	}
	if (getsockname(SocketData, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		local_IP2 = inet_ntoa(sin.sin_addr);

	}
	Respond;
	return true;
}

bool CFtpClientDlg::DataConnectPossive(void)
{
	const int BUF_SIZE = 64;
	WSADATA         wsd;            //WSADATA����  
	SOCKET          sServer;        //�������׽���  
	SOCKET          sClient;        //�ͻ����׽���  
	SOCKADDR_IN     addrServ;;      //��������ַ  
	char            buf[BUF_SIZE];  //�������ݻ�����  
	char            sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����  
	int             retVal;         //����ֵ  
									//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return 1;
	}

	//�����׽���  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		WSACleanup();//�ͷ��׽�����Դ;  
		return  -1;
	}

	//�������׽��ֵ�ַ   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(0);//�󶨶˿�,0��������˿�
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//���׽���  
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}

	//��ʼ����   
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return -1;
	}

	//��һ�����׽���
	SocketListen = sServer;

	int local_port = 0;
	std::string local_IP;
	struct sockaddr_in sin;
	int addrlen = sizeof(sin);
	if (getsockname(SocketListen, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		local_port = ntohs(sin.sin_port);
		
	}

	if (getsockname(SocketControl, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		local_IP = inet_ntoa(sin.sin_addr);

	}


	memset(Command, 0, MAX_SIZE);
	//�ϳ�PORT����
	std::string t = local_IP;
	for (size_t i = 0; i < t.length(); i++)
		if (t[i] == '.')t[i] = ',';
	char p[256];
	sprintf(p, "%d", local_port >> 8);
	std::string IPandPort = t + "," + p + ",";
	sprintf(p, "%d", local_port & ((1<<8)-1));
	IPandPort += p;
	IPandPort += "\r\n";
	memcpy(Command, "PORT ", strlen("PORT "));
	memcpy(Command + strlen("PORT "), IPandPort.c_str(), IPandPort.length());

	if (!SendCommand())
		return false;

	if (!RecvRespond())
		return false;
	else
	{
		if (RespondCode != 200)
		{
			MessageBox("PORT��Ӧ����");
			closesocket(SocketControl);
			WSACleanup();
			return false;
		}

	}

	Respond;


//���ܿͻ�������  
	//sockaddr_in addrClient;
	//int addrClientlen = sizeof(addrClient);
	//SocketData = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	//if (INVALID_SOCKET == sClient)
	//{
	//	closesocket(sServer);   //�ر��׽���  
	//	WSACleanup();           //�ͷ��׽�����Դ;  
	//	return 0;
	//}

	return true;
}





void CFtpClientDlg::OnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	  UpdateData(TRUE);
	
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2,2),&WSAData)!=0)//�����׽��ְ�
	{
		MessageBox("��ʼ��Winsockʧ�ܣ�");
		return;
	}

	SocketControl=socket(AF_INET,SOCK_STREAM,0);//���������׽���
	if (SocketControl==INVALID_SOCKET)
	{
      MessageBox("��������Socketʧ�ܣ�");
	  WSACleanup();
	  return;
	}

	
	u_long IpAddress; //�ж�������IP��ַ
	IpAddress=inet_addr(m_Address);
	if (IpAddress==INADDR_NONE)
	{
		hostent* pHostent=gethostbyname(m_Address);
		if (pHostent)
			IpAddress=(*(in_addr*)pHostent->h_addr_list[0]).s_addr;

	}

	sockaddr_in serveraddr;  //�����׽��ֵ�ַ
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(21);
	serveraddr.sin_addr.s_addr=IpAddress;

	m_Login+=m_Address;
	m_Login+="...\r\n";
	int nConnect;
	nConnect=connect(SocketControl,(sockaddr*)&serveraddr,sizeof(serveraddr));//����FTP��������
	if (nConnect==SOCKET_ERROR)
	{
        MessageBox("�������ӽ���ʧ�ܣ�");
		closesocket(SocketControl);
		WSACleanup();
		return;
	}

	if (!RecvRespond())
		return;
    else
	{//�ж�����Ӧ����
      if (RespondCode==220)
	  {
         m_Login+="Server:  ";
		 m_Login+=Respond;
	  }
	  else
	  {
		  MessageBox("����������Ӧ����");
		  closesocket(SocketControl);
		  WSACleanup();
		  return;
	  }

	}

	
	memset(Command,0,MAX_SIZE);
	//�ϳ�USER����
	memcpy(Command,"USER ",strlen("USER "));
	memcpy(Command+strlen("USER "),m_Account,strlen(m_Account));
	memcpy(Command+strlen("USER ")+strlen(m_Account),"\r\n",2);
	if (!SendCommand())
		return;

	m_Login+="Client:  ";
	m_Login+=Command;

	if (!RecvRespond())
		return;
	else
	{//�ж�USERӦ����
       if (RespondCode==220 || RespondCode==331)
	   {
          m_Login+="Server: ";
		  m_Login+=Respond;
	   }
	   else
	   {
          MessageBox("USER��Ӧ����");
		  closesocket(SocketControl);
		  WSACleanup();
		  return;
	   }
	}

	if (RespondCode==331)//User name okay,need password
	{ //�ϳ�PASS����
		memset(Command,0,MAX_SIZE);
		memcpy(Command,"PASS ",strlen("PASS "));
		memcpy(Command+strlen("PASS "),m_Password,strlen(m_Password));
		memcpy(Command+strlen("PASS ")+strlen(m_Password),"\r\n",2);
		if (!SendCommand())
			return;
		m_Login+="Client: PASS ******\r\n";

		if (!RecvRespond())
			return;
		else
		{//�ж�PASS��Ӧ��
			if (RespondCode==230)
            {
               m_Login+="Server: ";
			   m_Login+=Respond;
			}
			else
			{
               MessageBox("PASS��Ӧ����");
			   closesocket(SocketControl);
			   WSACleanup();
			   return;
			}
		}

	}

	UpdateData(false);
}


void CFtpClientDlg::OnList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!DataConnectPossive())
		return;

	memset(Command,0,MAX_SIZE);
	//�ϳ�LIST����
	memcpy(Command,"LIST",strlen("LIST"));
    memcpy(Command+strlen("LIST"),"\r\n",2);
	if (!SendCommand())
		return;

	m_Login+="Client: ";
	m_Login+=Command;
	if (!RecvRespond())
		return;
	else
	{
		if (RespondCode==125 || RespondCode==150 || RespondCode==226)
		{
          m_Login+="Server: ";
		  m_Login+=Respond;
		}
		else
		{
           MessageBox("LIST��Ӧ����");
		   closesocket(SocketControl);
		   WSACleanup();
		   return;
		}
	}
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	SocketData = accept(SocketListen, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == SocketData)
	{
		closesocket(SocketListen);   //�ر��׽���  
		WSACleanup();           //�ͷ��׽�����Դ;  
		return;
	}




	m_Login+="Client: ......\r\n";



	while (true)
	{
      char ListBuf[MAX_SIZE];
	  memset(ListBuf,0,MAX_SIZE);
	  int nRecv;
	  nRecv=recv(SocketData,ListBuf,MAX_SIZE,0);//��ȡĿ¼��Ϣ
	  if (nRecv==SOCKET_ERROR)
	  {
		  MessageBox("LIST���մ���");
		  closesocket(SocketData);
		  WSACleanup();
		  return;
	  }
	  if (nRecv<=0) break;
      
	  m_Directory=ListBuf;//��ʾĿ¼��Ϣ
	} 

	closesocket(SocketData);
	if (!RecvRespond())
		return;
	else
	{
		if (RespondCode==226)
		{
          m_Login+="Server: ";
		  m_Login+=Respond;
		}
		else
		{
           MessageBox("LIST��Ӧ����");
		   closesocket(SocketControl);
		   WSACleanup();
		   return;
		}
	}

	UpdateData(false);
}


void CFtpClientDlg::OnQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	memset(Command,0,MAX_SIZE);
	//�ϳ�QUIT����
	memcpy(Command,"QUIT",strlen("QUIT"));
    memcpy(Command+strlen("QUIT"),"\r\n",2);
	if (!SendCommand())
		return;
    
	m_Login+="Client: ";
	m_Login+=Command;
	if (!RecvRespond())
		return;
	else
	{
      if (RespondCode==221)
	  {
         m_Login+="Server: ";
		 m_Login+=Respond;
		 m_Directory="";
	  }
	  else
	  {
        MessageBox("QUIT��Ӧ����");
		closesocket(SocketControl);
		WSACleanup();
		return;
	  }
	}

	UpdateData(false);
	closesocket(SocketControl);
	WSACleanup();
}
