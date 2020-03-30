#pragma once


// CMaterialDlg 대화 상자

class CMaterialDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMaterialDialog)

public:
	CMaterialDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMaterialDialog();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MATERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_DefaultShaderCombo;
	afx_msg void OnCbnSelchangeComboDefaultshader();
	CComboBox m_InstancingShaderCombo;
	afx_msg void OnCbnSelchangeComboInstancingshader();
	CComboBox m_InstancingLayoutCombo;
	afx_msg void OnCbnSelchangeComboInstancinglayout();
	CComboBox m_RenderStateCombo;
	afx_msg void OnCbnSelchangeComboRenderstate();
	CComboBox m_ShaderStyleCombo;
	afx_msg void OnCbnSelchangeComboShaderstyle();
	BOOL m_bIsInstancing;
	afx_msg void OnBnClickedCheckInstancingEnable();
	CListBox m_ContainerList;
	afx_msg void OnLbnSelchangeListContainer();
	afx_msg void OnBnClickedButtonContainerCreate();
	afx_msg void OnBnClickedButtonContainerDelete();
	afx_msg void OnBnClickedButtonSubsetCreate();
	afx_msg void OnBnClickedButtonSubsetDelete();
	CString m_strMaterialName;
	afx_msg void OnEnChangeEditMaterialName();
	afx_msg void OnBnClickedOk();
	CListBox m_SubsetList;
	afx_msg void OnLbnSelchangeListSubset();
	afx_msg void OnBnClickedButtonContainerModify();
};
