
// FtpClientDlg.h : 头文件
//

#pragma once


// CFtpClientDlg 对话框
class CFtpClientDlg : public CDialogEx
{
// 构造
public:
	CFtpClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FTPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_Address;
	CString m_Account;
	CString m_Password;
	CString m_Login;
	CString m_Directory;
private:
	SOCKET SocketControl;
	SOCKET SocketData;
	SOCKET SocketListen;
	int RespondCode;
	char Respond[MAX_SIZE];
	char Command[MAX_SIZE];

	bool SendCommand(void);
	bool RecvRespond(void);
	bool DataConnect(void);
	bool DataConnectPossive(void);
public:
	afx_msg void OnConnect();
	afx_msg void OnList();
	afx_msg void OnQuit();
};
