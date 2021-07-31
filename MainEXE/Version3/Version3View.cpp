
// Version3View.cpp : implementation of the CVersion3View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Version3.h"
#endif

#include "Version3Doc.h"
#include "Version3View.h"


#include <gdiplus.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//BackProcess* backProcess;

// CVersion3View

IMPLEMENT_DYNCREATE(CVersion3View, CView)

BEGIN_MESSAGE_MAP(CVersion3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PLAY_GO, &CVersion3View::OnPlayGo)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_INITIALIZEGAME, &CVersion3View::OnFileInitializegame)
	ON_COMMAND(ID_FILE_REPLAYGAME, &CVersion3View::OnFileReplaygame)
END_MESSAGE_MAP()


// CVersion3View construction/destruction

CVersion3View::CVersion3View() noexcept
{
	// TODO: add construction code here
	initializeGame = false;
	gameEnds = true;
}

CVersion3View::~CVersion3View()
{
}

BOOL CVersion3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


// CVersion3View drawing

void CVersion3View::OnDraw(CDC* pDC)
{
	CVersion3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;



	// TODO: add draw code for native data here

	// get window's size
	CWnd* pwndParent = this->GetParent();
	CRect rc;
	GetClientRect(&rc);
	rc.right -= 300;
	pDC->Rectangle(&rc);


	pwndParent->GetWindowRect(&rc);
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP1);
	CBitmap* oldbmp = memdc.SelectObject(&bmp);
	int w = 188;
	int h = 282;
	int i, j;
	i = 0;
	j = 0;

	int windowWidth, windowHeight;
	int cardSpaceWidth, cardSpaceHeight;
	try
	{
		auto set_Res_Pos = [&](int k, int l) {
			windowWidth = rc.Width();
			windowHeight = rc.Height();
			//std::cout << "Window width:" << windowWidth << std::endl;
			cardSpaceWidth = windowWidth / 38;
			cardSpaceHeight = windowHeight / 20;

			i = backProcess->getPaintGame().getPlayersCardsPos().at(k).at(l) - 1;
			j = backProcess->getPaintGame().getPlayersCardsPos().at(k).at(l + 1) - 1;
		};

		if (initializeGame && backProcess)
		{
			if (backProcess->getFileManager().getListBoxString() != L"")
			{
				int nCount = listBox.GetCount();
				listBox.SetCurSel(nCount - 1);
				listBox.SetTopIndex(nCount);
				listBox.AddString(backProcess->getFileManager().getListBoxString());
				backProcess->getFileManager().setListBoxString(L"");
			}
			// draw players' cards images
			for (int k = 0; k < backProcess->getPaintGame().getPlayersCardsPos().size(); k++)
			{
				for (unsigned int l = 0; l < backProcess->getPaintGame().getPlayersCardsPos().at(k).size(); l += 2)
				{
					set_Res_Pos(k, l);
					pDC->BitBlt(50 + w + l * cardSpaceWidth, cardSpaceHeight / 100, w, h, &memdc, 20 + i * w, 18 + j * h, SRCCOPY);
				}
				k++;
				for (unsigned int l = 0; l < backProcess->getPaintGame().getPlayersCardsPos().at(k).size(); l += 2)
				{
					set_Res_Pos(k, l);
					pDC->BitBlt(cardSpaceWidth * 25, 10 + l * cardSpaceHeight, w, h, &memdc, 20 + i * w, 18 + j * h, SRCCOPY);
				}
				k++;
				for (unsigned int l = 0; l < backProcess->getPaintGame().getPlayersCardsPos().at(k).size(); l += 2)
				{
					set_Res_Pos(k, l);
					pDC->BitBlt(50 + w + l * cardSpaceWidth, cardSpaceHeight * 12, w, h, &memdc, 20 + i * w, 18 + j * h, SRCCOPY);
				}
				k++;
				for (unsigned int l = 0; l < backProcess->getPaintGame().getPlayersCardsPos().at(k).size(); l += 2)
				{
					set_Res_Pos(k, l);
					pDC->BitBlt(cardSpaceWidth / 100, 10 + l * cardSpaceHeight, w, h, &memdc, 20 + i * w, 18 + j * h, SRCCOPY);
				}
				k++;
				for (unsigned int l = 0; l < backProcess->getPaintGame().getPlayersCardsPos().at(k).size(); l += 2)
				{
					set_Res_Pos(k, l);
					pDC->BitBlt(150 + w + l * cardSpaceWidth, cardSpaceHeight * 6, w, h, &memdc, 20 + i * w, 18 + j * h, SRCCOPY);
				}
			}


			memdc.SelectObject(oldbmp);
			bmp.DeleteObject();
			memdc.DeleteDC();

			backProcess->getPaintGame().getPlayersCardsPos();
			//std::cout << "..." << backProcess->getPaintGame().getPlayersCardsPos().size() << std::endl;
			int playersCardCount = backProcess->getPaintGame().getPlayersCardsPos().size() / 4;

			//std::cout << backProcess->playerCardsWString() << std::endl;

			CString sToken = _T("");
			int tokenI = 0;
			/*while (AfxExtractSubString(sToken, backProcess->playerCardsWString(), tokenI, ';'))
			{
				listBox.AddString(sToken);
				tokenI++;
			}*/


		}
	}
	catch (...)
	{
		Invalidate();
	}

}


// CVersion3View printing

BOOL CVersion3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVersion3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVersion3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CVersion3View diagnostics

#ifdef _DEBUG
void CVersion3View::AssertValid() const
{
	CView::AssertValid();
}

void CVersion3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVersion3Doc* CVersion3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVersion3Doc)));
	return (CVersion3Doc*)m_pDocument;
}
#endif //_DEBUG


// CVersion3View message handlers

// listBox that can be used here and from player
//CListBox listBox;
int CVersion3View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	listBox.Create(WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0, 0, 0, 0), this, 1);
	font.CreatePointFont(180, L"Times New Roman");
	listBox.SetFont(&font);
	return 0;
}


void CVersion3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	listBox.MoveWindow(cx - 300, 0, cx, cy);
}


void CVersion3View::OnPlayGo()
{
	// TODO: Add your command handler code here
	if (!initializeGame && gameEnds)
	{
		gameEnds = false;

		//std::thread backProccessStart(&CVersion3View::startBackProcess, this);
		backProccessStart = std::thread(&CVersion3View::startBackProcess, this);
		backProccessStart.detach();
		Invalidate();
	}
	initializeGame = true;
	gameEnds = true;
}


void CVersion3View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	font.DeleteObject();
}

void CVersion3View::startBackProcess()
{
	try
	{
		delete backProcess;
		backProcess = new BackProcess;
		backProcess->startProcess();
	}
	catch (...)
	{
		delete backProcess;
		backProcess = new BackProcess;
		backProcess->startProcess();
	}
}


void CVersion3View::OnFileInitializegame()
{

}



void CVersion3View::OnFileReplaygame()
{
	// TODO: Add your command handler code here
	//if (initializeGame && !gameEnds)
	//{
	//	gameEnds = false;
	//	backProccessStart = std::thread(&CVersion3View::startBackProcess, this);
	//	backProccessStart.detach();
	//	Invalidate();
	//}
	//gameEnds = true;
}
