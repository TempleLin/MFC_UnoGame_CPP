
// Version3View.h : interface of the CVersion3View class
//

#pragma once
//my own include
#include "backProcess.h"
#include <thread>


class CVersion3View : public CView
{
	friend class Players;
protected: // create from serialization only
	CVersion3View() noexcept;
	DECLARE_DYNCREATE(CVersion3View)

	CListBox listBox;

	CFont font;

	bool initializeGame;
	bool gameEnds;

	std::thread backProccessStart;

	void startBackProcess();

// Attributes
public:
	CVersion3Doc* GetDocument() const;
	//BackProcess* backProcess;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CVersion3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPlayGo();
	afx_msg void OnDestroy();
	afx_msg void OnFileInitializegame();
	afx_msg void OnFileReplaygame();
};

#ifndef _DEBUG  // debug version in Version3View.cpp
inline CVersion3Doc* CVersion3View::GetDocument() const
   { return reinterpret_cast<CVersion3Doc*>(m_pDocument); }
#endif

