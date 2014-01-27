
// server.cpp : D�finit les comportements de classe pour l'application.
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CserverApp

BEGIN_MESSAGE_MAP(CserverApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// construction CserverApp

CserverApp::CserverApp()
{
	// prend en charge le Gestionnaire de red�marrage
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}


// Seul et unique objet CserverApp

CserverApp theApp;


// initialisation de CserverApp

BOOL CserverApp::InitInstance()
{
	// InitCommonControlsEx() est requis sur Windows�XP si le manifeste de l'application
	// sp�cifie l'utilisation de ComCtl32.dll version�6 ou ult�rieure pour activer les
	// styles visuels.  Dans le cas contraire, la cr�ation de fen�tres �chouera.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// � d�finir pour inclure toutes les classes de contr�les communs � utiliser
	// dans votre application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// Cr�er le gestionnaire de shell, si la bo�te de dialogue contient
	// des contr�les d'arborescence ou de liste de shell.
	CShellManager *pShellManager = new CShellManager;

	// Active le gestionnaire visuel "natif Windows" pour activer les th�mes dans les contr�les MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalit�s et que vous souhaitez r�duire la taille
	// de votre ex�cutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation sp�cifiques dont vous n'avez pas besoin.
	// Changez la cl� de Registre sous laquelle nos param�tres sont enregistr�s
	// TODO: modifiez cette cha�ne avec des informations appropri�es,
	// telles que le nom de votre soci�t� ou organisation
	SetRegistryKey(_T("Applications locales g�n�r�es par AppWizard"));

	CserverDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: placez ici le code d�finissant le comportement lorsque la bo�te de dialogue est
		//  ferm�e avec OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: placez ici le code d�finissant le comportement lorsque la bo�te de dialogue est
		//  ferm�e avec Annuler
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Avertissement : �chec de cr�ation de la bo�te de dialogue, par cons�quent, l'application s'arr�te de mani�re inattendue.\n");
		TRACE(traceAppMsg, 0, "Avertissement : si vous utilisez les contr�les MFC de la bo�te de dialogue, vous ne pouvez pas ex�cuter #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Supprimer le gestionnaire de shell cr�� ci-dessus.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Lorsque la bo�te de dialogue est ferm�e, retourner FALSE afin de quitter
	//  l'application, plut�t que de d�marrer la pompe de messages de l'application.
	return FALSE;
}

