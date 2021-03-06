#include "stdafx.h"
#include "mfcClient1.h"
#include "mfcClient1Dlg.h"
#include "LoginDlg.h"
#include "CDPI.h"

#include <fstream>
using namespace std;

extern const char seperator[3];    //分隔符

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CLoginDlg::OnOk)
    ON_BN_CLICKED(IDSET, &CLoginDlg::OnSet)
    ON_BN_CLICKED(IDC_regist, &CLoginDlg::OnRegist)
    ON_BN_CLICKED(IDC_CHECK_remPw, &CLoginDlg::OnCheckRemPW)
    ON_BN_CLICKED(IDC_Swap, &CLoginDlg::OnSwap)
    ON_BN_CLICKED(IDC_ShowPw, &CLoginDlg::OnShowPW)
END_MESSAGE_MAP()


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_userID, userID);
    DDV_MaxChars(pDX, userID, 16);
    DDX_Text(pDX, IDC_pw, pw);
    DDV_MaxChars(pDX, pw, 16);
    DDX_Control(pDX, IDC_IPADDRESS1, ipAddrCtrl);
    DDX_Text(pDX, IDC_EDIT1, port);
}

BOOL CLoginDlg::OnInitDialog()
{
    DOUBLE d = CDPI::Scale(100.0);
    ((CButton*)GetDlgItem(IDC_CHECK_remPw))->SetCheck(TRUE);
    CDialog::OnInitDialog();
    CString ipAddr = AfxGetApp()->GetProfileString("Login", "IP", IP_SERVER);	//需要连接的服务器ip地址
    ipAddrCtrl.SetWindowText(ipAddr);
    b_strech = FALSE;
    CString id_pw = AfxGetApp()->GetProfileString("UserInfo", "userID_pw");
    int i = id_pw.Find(seperator);
    if (i != -1) {
        userID = id_pw.Left(i);
        pw = id_pw.Right(id_pw.GetLength() - i - 2);
    }
    else {
        char _user[17] = "", _pw[17] = "";
        ifstream user_pw(ClientInfo::myDIR + "user_pw.dat", ios::in);
        if (!user_pw.fail()) {
            user_pw >> _user >> _pw;
            user_pw.close();
            if (_user[0] != 0 && _pw[0] != 0) {
                userID = _user;
                pw = _pw;
            }
        }
        else {
            MessageBox("很感谢亲的试用哦，首次使用请先注册一个自己的账号吧！", "温馨提示");
        }
    }
    UpdateData(0);
    GetWindowRect(&rc);
    SetWindowPos(NULL, rc.left, rc.top, rc.Width(), CDPI::Scale(200.0), SWP_NOMOVE | SWP_SHOWWINDOW);
    GetDlgItem(IDC_userID)->SetFocus();
    return TRUE;
}

bool CLoginDlg::onlyAlNum(const CString &s)
{
    for (int i = s.GetLength() - 1; i >= 0; --i)
        if (s[i] > 0 && !isalnum(s[i]))	//不是字母不是数字，也不是中文字符
            return 0;
    return 1;
}
//检查数据是否存在无效信息
bool CLoginDlg::isDataInvalide()
{
    UpdateData(1);
    ipAddrCtrl.GetWindowText(ip);
    userID.Replace(" ", "");
    pw.Replace(" ", "");
    if (userID == "") {
        MessageBox("用户名不能为空", "温馨提示");
        GetDlgItem(IDC_userID)->SetFocus();
        return true;
    }
    if (!onlyAlNum(userID)) {
        MessageBox("用户名只能包含字母、数字或汉字", "温馨提示");
        CEdit* edit = (CEdit*)GetDlgItem(IDC_userID);
        edit->SetFocus();
        edit->SetSel(0, 20);
        return true;
    }
    if (pw == "") {
        MessageBox("密码不能为空", "温馨提示");
        GetDlgItem(IDC_pw)->SetFocus();
        return true;
    }
    if (port < 1024 || port > 65535) {
        MessageBox("端口号设置有误，有效范围为1024-65535！", "温馨提示");
        GetDlgItem(IDC_EDIT1)->SetFocus();
        return true;
    }
    OnCheckRemPW();
    return false;
}

void CLoginDlg::OnOk()
{
    if (isDataInvalide() == 1)
        return;
    CClientSocket sock(userID + "*test");
    if (!sock.Create()) {       //创建套接字
        MessageBox("创建套接字失败！", "温馨提示", MB_ICONERROR);
        return;
    }
    SetTimer(0, 3000, NULL);		//设置延时
    if (!sock.Connect(ip, port)) {  //连接服务器
        MessageBox("连接服务器失败：" + sock.getLastErrorStr(), "温馨提示", MB_ICONERROR);
        return;
    }
    sock.Close();
    AfxGetApp()->WriteProfileString("Login", "IP", ip);
    CDialogEx::OnOK();
}

void CLoginDlg::OnSet()
{
    int dt = CDPI::Scale(20.0);
    if (b_strech) {
        for (int i = 0; rc.Height() - i >= CDPI::Scale(200.0); i += dt) {
            SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height() - i, SWP_NOMOVE | SWP_SHOWWINDOW);
            Sleep(10);
        }
        b_strech = FALSE;
    }
    else {
        for (int i = 0; i + CDPI::Scale(200.0) <= rc.Height(); i += dt) {
            SetWindowPos(NULL, rc.left, rc.top, rc.Width(), i + CDPI::Scale(200.0), SWP_NOMOVE | SWP_SHOWWINDOW);
            Sleep(10);
        }
        b_strech = TRUE;
    }
}

void CLoginDlg::OnRegist()
{
    if (isDataInvalide())
        return;
    static CmfcClient1Dlg *pDlg = (CmfcClient1Dlg*)theApp.GetMainWnd();
    CClientSocket sock(userID);
    if (!sock.Create()) {       //创建套接字
        MessageBox("注册失败——创建套接字失败！", "温馨提示", MB_ICONERROR);
        return;
    }
    SetTimer(0, 3000, NULL);		//设置延时
    if (!sock.Connect(ip, port)) {  //连接服务器
        MessageBox("注册失败——连接服务器失败：" + sock.getLastErrorStr(), "温馨提示", MB_ICONERROR);
        puts("\a");
        return;
    }
    if (KillTimer(0)) {
        CString dataToSend = pDlg->mymsg.join("", TYPE[Register], userID, "", "", pw);
        sock.Send(dataToSend, dataToSend.GetLength() + 1);
        sock.Close();
        MessageBox("注册信息发送成功，审核通过后即可登陆！", "温馨提示");
        OnOK();
    }
}

void CLoginDlg::OnCheckRemPW()
{
    if (userID != "" && pw != "") {
        if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_remPw))->GetCheck()) {
            ofstream user_pw(ClientInfo::myDIR + "user_pw.dat");
            user_pw << userID << "\t" << pw;
            user_pw.close();
            AfxGetApp()->WriteProfileString("UserInfo", "userID_pw", userID + seperator + pw);
        }
        else {
            DeleteFile(ClientInfo::myDIR + "user_pw.dat");
            AfxGetApp()->WriteProfileString("UserInfo", "userID_pw", "");
        }
    }
}

void CLoginDlg::OnSwap()
{
    if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_Swap))->GetCheck())
        ipAddrCtrl.SetWindowText(IP_LOCALHOST);
    else
        ipAddrCtrl.SetWindowText(IP_SERVER);
}

void CLoginDlg::OnShowPW()
{
    CEdit *edit = (CEdit*)GetDlgItem(IDC_pw);
    if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_ShowPw))->GetCheck()) {
        edit->SetPasswordChar(0);
    }
    else {
        edit->SetPasswordChar(_T('*'));
    }
    edit->RedrawWindow(NULL, NULL);
}