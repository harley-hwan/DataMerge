
// DataMergeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DataMerge.h"
#include "DataMergeDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>

#include "DataLoader.h"
#include "DataMatching.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataMergeDlg 대화 상자



CDataMergeDlg::CDataMergeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATAMERGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataMergeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDataMergeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDataMergeDlg::OnBnClickedButtonSearch)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CDataMergeDlg 메시지 처리기

BOOL CDataMergeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	SetWindowText(_T("DataMerge"));

	readFolderPath();
	SetDlgItemText(IDC_EDIT_FOLDERPATH, m_cstrFolderPath);
	GetDlgItemText(IDC_EDIT_FOLDERPATH, m_cstrFolderPath);

	SetDlgItemText(IDC_EDIT_DIFFTIME, _T("14"));

	GetDlgItem(IDC_STATIC_GCQ)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_WAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_NX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MAX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_GRADAR)->ShowWindow(SW_HIDE);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDataMergeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDataMergeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDataMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDataMergeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();

	DataLoader loader;
	loader.loadIncludePath(std::string(CT2CA(m_cstrFolderPath)) + "\\");

	stShotInfo* mergedData[(int)SENSOR_TOTALCNT];
	loader.excute(mergedData);


	if (mergedData[(int)SENSOR_GCQ]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_GCQ)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_WAVE]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_WAVE)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_TM]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_TM)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_GRADAR]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_GRADAR)->ShowWindow(SW_SHOW);
	
	if (mergedData[(int)SENSOR_MAX]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_MAX)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_NX]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_NX)->ShowWindow(SW_SHOW);



	vector<string> clubtypeVec;
	vector<string> ballspeedVec;
	vector<string> launchAngleVec;
	for (int i = 0; i < mergedData[SENSOR_GCQ]->dataVec.size(); i++)
	{
		string clubtype = mergedData[SENSOR_GCQ]->dataVec[i][1];
		string ballspeed = mergedData[SENSOR_GCQ]->dataVec[i][3];
		string launchAngle = mergedData[SENSOR_GCQ]->dataVec[i][4];
		clubtypeVec.push_back(clubtype);
		ballspeedVec.push_back(ballspeed);
		launchAngleVec.push_back(launchAngle);
	}

	CString diffTimeStr = 0;
	GetDlgItemText(IDC_EDIT_DIFFTIME, diffTimeStr);

	DataMatching matcher;
	matcher.setMaxDiffTime(_ttoi(diffTimeStr));
	matcher.setDataSet(*mergedData);
	matcher.excute();


}


void CDataMergeDlg::OnBnClickedButtonSearch()
{
	CString strInitPath = _T("C:\\");
	CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		// 선택된 폴더 경로얻음
		m_cstrFolderPath = Picker.GetPathName();
		SetDlgItemText(IDC_EDIT_FOLDERPATH, m_cstrFolderPath);
		writeFolderPath(m_cstrFolderPath);
	}
}

void CDataMergeDlg::writeFolderPath(CString str)
{
	string line = std::string(CT2CA(str));
	ofstream file("../temp.txt");
	if (file.is_open())
	{
		file << line;
		file.close();
	}
}

void CDataMergeDlg::readFolderPath()
{
	string line;
	ifstream file("../temp.txt");
	if (file.is_open())
	{
		while (getline(file, line)) {

			string s =  line;
			m_cstrFolderPath = line.c_str();
		}
		file.close();
	}
}




void CDataMergeDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	UINT numFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	if (numFiles != 1)
		return;

	TCHAR folderPath[256];
	DragQueryFile(hDropInfo, 0, folderPath, 255);
	m_cstrFolderPath = folderPath;

	SetDlgItemText(IDC_EDIT_FOLDERPATH, m_cstrFolderPath);
	writeFolderPath(m_cstrFolderPath);

	DataLoader loader;
	loader.loadIncludePath(std::string(CT2CA(m_cstrFolderPath)) + "\\");

	stShotInfo* mergedData[(int)SENSOR_TOTALCNT];
	loader.excute(mergedData);


	if (mergedData[(int)SENSOR_GCQ]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_GCQ)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_WAVE]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_WAVE)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_TM]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_TM)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_GRADAR]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_GRADAR)->ShowWindow(SW_SHOW);

	//if (mergedData[(int)SENSOR_MAX]->dataVec.size() != 0)
	//	GetDlgItem(IDC_STATIC_MAX)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_MAX]->item.size() != 0)  // 2025-02-05 KJH: 헤더만 있어도 TRUE 되도록 수정.
		GetDlgItem(IDC_STATIC_MAX)->ShowWindow(SW_SHOW);

	if (mergedData[(int)SENSOR_NX]->dataVec.size() != 0)
		GetDlgItem(IDC_STATIC_NX)->ShowWindow(SW_SHOW);

	CDialogEx::OnDropFiles(hDropInfo);
}
