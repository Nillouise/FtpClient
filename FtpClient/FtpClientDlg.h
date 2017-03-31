
// FtpClientDlg.h : ͷ�ļ�
//

#pragma once


// CFtpClientDlg �Ի���
class CFtpClientDlg : public CDialogEx
{
// ����
public:
	CFtpClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FTPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
