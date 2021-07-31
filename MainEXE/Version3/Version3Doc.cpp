
// Version3Doc.cpp : implementation of the CVersion3Doc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Version3.h"
#endif

#include "Version3Doc.h"

#include <propkey.h>

// my own class
#include "backProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVersion3Doc

IMPLEMENT_DYNCREATE(CVersion3Doc, CDocument)

BEGIN_MESSAGE_MAP(CVersion3Doc, CDocument)
//	ON_COMMAND(ID_FILE_INITIALIZEGAME, &CVersion3Doc::OnFileInitializegame)
//	ON_COMMAND(ID_PLAY_GO, &CVersion3Doc::OnPlayGo)
END_MESSAGE_MAP()


// CVersion3Doc construction/destruction

CVersion3Doc::CVersion3Doc() noexcept
{
	// TODO: add one-time construction code here
}

CVersion3Doc::~CVersion3Doc()
{
}

BOOL CVersion3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CVersion3Doc serialization

void CVersion3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CVersion3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CVersion3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CVersion3Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CVersion3Doc diagnostics

#ifdef _DEBUG
void CVersion3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVersion3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVersion3Doc commands



//dynamic_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd)->Invalidate();
