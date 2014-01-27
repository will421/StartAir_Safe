
// serverDlg.h : fichier d'en-t�te
//

#pragma once


// bo�te de dialogue CserverDlg
class CserverDlg : public CDHtmlDialog
{
// Construction
public:
	CserverDlg(CWnd* pParent = NULL);	// constructeur standard

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_SERVER_DIALOG, IDH = IDR_HTML_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge de DDX/DDV

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Impl�mentation
protected:
	HICON m_hIcon;

	// Fonctions g�n�r�es de la table des messages
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
