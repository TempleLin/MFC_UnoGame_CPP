
#include <afxwin.h>
#include "players.h"
class ThreadProcess :
    public CWinThread
{
    DECLARE_DYNCREATE(ThreadProcess)
protected:
    ThreadProcess();
    virtual ~ThreadProcess();

public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

protected:
    DECLARE_MESSAGE_MAP()
public:
    static UINT runThread(LPVOID param);
};



