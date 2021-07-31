#include "pch.h"
#include "threadProcess.h"
#include <iostream>
#include "backProcess.h"

UINT ThreadProcess::runThread(LPVOID param)
{
    Players* players = (Players*)param;
    typedef Players::PlayerCallParams CALLPARAMS;
    players->callPlay(CALLPARAMS::state, players->players[Players::playerIndex].getHoldCards(), CALLPARAMS::one,
        CALLPARAMS::others, CALLPARAMS::command, CALLPARAMS::reply, CALLPARAMS::extra);
    if (CALLPARAMS::state != 1)
        Sleep(1000);

    //backProcess->getPaintGame().setPaintGame(*this);
    //CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
    //pChild->Invalidate();

    return 0;
}