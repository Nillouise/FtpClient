
// FtpClientDlg.cpp : 实现文件
//
// FTP 的主动连接，要先发监听端口，然后 当客户端发list命令时，服务器才会连上这个监听端口。
// 另外由于一开始我在虚拟机上做这东西，ip端口没能穿越好（ftp协商的时候，数据不是写在ip报头里面的，不能用UPnP 穿越），要在真机上做着东西才行的。

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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFtpClientDlg 对话框




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


// CFtpClientDlg 消息处理程序

BOOL CFtpClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFtpClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
      MessageBox("Socket发送失败！");
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
	nRecv=recv(SocketControl,Respond,MAX_SIZE,0);//通过连接接收响应
	if (nRecv==SOCKET_ERROR)
	{
       MessageBox("Socket接收失败！");
	   closesocket(SocketControl);
	   WSACleanup();
	   return false;
	}

	//从响应中解析响应码
	char* ReplyCodes=new char[3];
	memset(ReplyCodes,0,3);
	memcpy(ReplyCodes,Respond,3);
	RespondCode=atoi(ReplyCodes);
	return true;
}


bool CFtpClientDlg::DataConnect(void)
{
	 memset(Command,0,MAX_SIZE);
   //合成PASV命令
   memcpy(Command,"PASV",strlen("PASV"));
   memcpy(Command+strlen("PASV"),"\r\n",2);

   if (!SendCommand())
	   return false;
  
   if (!RecvRespond())
	   return false;
   else
   {
      //判断PASV响应码
	   if (RespondCode!=227)
	   {
         MessageBox("PASV响应错误！");
		 closesocket(SocketControl);
		 WSACleanup();
		 return false;
	   }
	   
   }


   char* part[6];
   //分离PASV应答信息
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

	
	unsigned short ServerPort;   //获取服务器数据端口
	ServerPort=unsigned short((atoi(part[4])<<8)+atoi(part[5]));
	SocketData=socket(AF_INET,SOCK_STREAM,0);
	if (SocketData==INVALID_SOCKET)
	{
      MessageBox("创建数据Socket失败！");
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
      MessageBox("数据连接失败！");
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
	WSADATA         wsd;            //WSADATA变量  
	SOCKET          sServer;        //服务器套接字  
	SOCKET          sClient;        //客户端套接字  
	SOCKADDR_IN     addrServ;;      //服务器地址  
	char            buf[BUF_SIZE];  //接收数据缓冲区  
	char            sendBuf[BUF_SIZE];//返回给客户端得数据  
	int             retVal;         //返回值  
									//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return 1;
	}

	//创建套接字  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		WSACleanup();//释放套接字资源;  
		return  -1;
	}

	//服务器套接字地址   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(0);//绑定端口,0代表随机端口
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//绑定套接字  
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}

	//开始监听   
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		closesocket(sServer);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return -1;
	}

	//进一步绑定套接字
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
	//合成PORT命令
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
			MessageBox("PORT响应错误！");
			closesocket(SocketControl);
			WSACleanup();
			return false;
		}

	}

	Respond;


//接受客户端请求  
	//sockaddr_in addrClient;
	//int addrClientlen = sizeof(addrClient);
	//SocketData = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	//if (INVALID_SOCKET == sClient)
	//{
	//	closesocket(sServer);   //关闭套接字  
	//	WSACleanup();           //释放套接字资源;  
	//	return 0;
	//}

	return true;
}





void CFtpClientDlg::OnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	  UpdateData(TRUE);
	
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2,2),&WSAData)!=0)//建立套接字绑定
	{
		MessageBox("初始化Winsock失败！");
		return;
	}

	SocketControl=socket(AF_INET,SOCK_STREAM,0);//创建连接套接字
	if (SocketControl==INVALID_SOCKET)
	{
      MessageBox("创建控制Socket失败！");
	  WSACleanup();
	  return;
	}

	
	u_long IpAddress; //判断域名或IP地址
	IpAddress=inet_addr(m_Address);
	if (IpAddress==INADDR_NONE)
	{
		hostent* pHostent=gethostbyname(m_Address);
		if (pHostent)
			IpAddress=(*(in_addr*)pHostent->h_addr_list[0]).s_addr;

	}

	sockaddr_in serveraddr;  //定义套接字地址
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(21);
	serveraddr.sin_addr.s_addr=IpAddress;

	m_Login+=m_Address;
	m_Login+="...\r\n";
	int nConnect;
	nConnect=connect(SocketControl,(sockaddr*)&serveraddr,sizeof(serveraddr));//建立FTP控制连接
	if (nConnect==SOCKET_ERROR)
	{
        MessageBox("控制连接建立失败！");
		closesocket(SocketControl);
		WSACleanup();
		return;
	}

	if (!RecvRespond())
		return;
    else
	{//判断连接应答码
      if (RespondCode==220)
	  {
         m_Login+="Server:  ";
		 m_Login+=Respond;
	  }
	  else
	  {
		  MessageBox("控制连接响应错误！");
		  closesocket(SocketControl);
		  WSACleanup();
		  return;
	  }

	}

	
	memset(Command,0,MAX_SIZE);
	//合成USER命令
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
	{//判断USER应答码
       if (RespondCode==220 || RespondCode==331)
	   {
          m_Login+="Server: ";
		  m_Login+=Respond;
	   }
	   else
	   {
          MessageBox("USER响应错误！");
		  closesocket(SocketControl);
		  WSACleanup();
		  return;
	   }
	}

	if (RespondCode==331)//User name okay,need password
	{ //合成PASS命令
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
		{//判断PASS响应码
			if (RespondCode==230)
            {
               m_Login+="Server: ";
			   m_Login+=Respond;
			}
			else
			{
               MessageBox("PASS响应错误！");
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
	// TODO: 在此添加控件通知处理程序代码
	if (!DataConnectPossive())
		return;

	memset(Command,0,MAX_SIZE);
	//合成LIST命令
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
           MessageBox("LIST响应错误！");
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
		closesocket(SocketListen);   //关闭套接字  
		WSACleanup();           //释放套接字资源;  
		return;
	}




	m_Login+="Client: ......\r\n";



	while (true)
	{
      char ListBuf[MAX_SIZE];
	  memset(ListBuf,0,MAX_SIZE);
	  int nRecv;
	  nRecv=recv(SocketData,ListBuf,MAX_SIZE,0);//读取目录信息
	  if (nRecv==SOCKET_ERROR)
	  {
		  MessageBox("LIST接收错误！");
		  closesocket(SocketData);
		  WSACleanup();
		  return;
	  }
	  if (nRecv<=0) break;
      
	  m_Directory=ListBuf;//显示目录信息
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
           MessageBox("LIST响应错误！");
		   closesocket(SocketControl);
		   WSACleanup();
		   return;
		}
	}

	UpdateData(false);
}


void CFtpClientDlg::OnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	memset(Command,0,MAX_SIZE);
	//合成QUIT命令
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
        MessageBox("QUIT响应错误！");
		closesocket(SocketControl);
		WSACleanup();
		return;
	  }
	}

	UpdateData(false);
	closesocket(SocketControl);
	WSACleanup();
}
