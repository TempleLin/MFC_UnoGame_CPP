#pragma once
#include "paintGame.h"
#include "myDebugger.h"
#include "myFileManagement.h"

class BackProcess
{
private:
	std::unique_ptr<PaintGame> paintGame;
	std::unique_ptr<Players> players;
	std::unique_ptr<MyDebugger> myDebugger; 
	std::unique_ptr<CardEdit> cardEdit;
	std::unique_ptr<FileManagement::FileManager> fileManager;
public:
	BackProcess();
	PaintGame& getPaintGame();
	CString playerCardsWString();
	void startProcess();
	void replayGame();
	FileManagement::FileManager& getFileManager();
};

inline BackProcess *backProcess = new BackProcess;