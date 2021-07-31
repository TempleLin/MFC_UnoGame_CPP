#include "pch.h"
#include "backProcess.h"


//gets information for drawing
PaintGame& BackProcess::getPaintGame()
{
	return *paintGame;
}

// originally the "main" in console mode
BackProcess::BackProcess()
{
	Players::PlayerCallParams::state = 1;
	Players::PlayerCallParams::one = { (Type)0,0,(Color)0 };
	Players::PlayerCallParams::others = {};
	Players::PlayerCallParams::command = 0;
	Players::PlayerCallParams::reply = { (Type)0,0,(Color)0 };
	Players::PlayerCallParams::extra = 0;
	Players::PlayerCallParams::cards = {};
	Players::playerIndex = 0;
}

void BackProcess::startProcess()
{
	myDebugger = std::make_unique<MyDebugger>();
	cardEdit = std::make_unique<CardEdit>();
	fileManager = std::make_unique<FileManagement::FileManager>();

	//fileManager->clearFiles();

	std::vector<CCard> deck(104);

	cardEdit->initTable(deck);
	cardEdit->shuffleCards(deck);

	int firstDrawCount{ 7 };
	// each player gains seven cards and 28 cards get removed from deck when
	// players object gets instantiated
	players = std::make_unique<Players>(firstDrawCount, deck);

	cardEdit->sortCards(players->players[0].getHoldCards());
	cardEdit->sortCards(players->players[1].getHoldCards());
	cardEdit->sortCards(players->players[2].getHoldCards());
	cardEdit->sortCards(players->players[3].getHoldCards());

	paintGame = std::make_unique<PaintGame>();
	paintGame->setPaintGame(*players);

	//myDebugger->printPlayersCards(*players);

	std::string pathname = ".\\players";

	players->storePlayersID(pathname, 4);
	myDebugger->writeLine("Players ID:");
	myDebugger->writeLine(players->playersYZUID[0]);
	myDebugger->writeLine(players->playersYZUID[1]);
	myDebugger->writeLine(players->playersYZUID[2]);
	myDebugger->writeLine(players->playersYZUID[3]);

	players->callDLLPlay(pathname, deck);

	fileManager->outputRanks(*players);
}

void BackProcess::replayGame()
{
	//std::vector<CCard> deck(104);
	//cardEdit->initTable(deck);
	//cardEdit->shuffleCards(deck);
	//int firstDrawCount = 7;
	//std::vector<CCard> deck(104);
	//cardEdit->initTable(deck);
	//cardEdit->shuffleCards(deck);

	//players->players[0].getHoldCards().clear();
	//players->players[1].getHoldCards().clear();
	//players->players[2].getHoldCards().clear();
	//players->players[3].getHoldCards().clear();

	//cardEdit->sortCards(players->players[0].getHoldCards());
	//cardEdit->sortCards(players->players[1].getHoldCards());
	//cardEdit->sortCards(players->players[2].getHoldCards());
	//cardEdit->sortCards(players->players[3].getHoldCards());

	//paintGame->setPaintGame(*players);
	//std::string pathname = ".\\players";
	//players->storePlayersID(pathname, 4);
	//myDebugger->writeLine("Players ID:");
	//myDebugger->writeLine(players->playersYZUID[0]);
	//myDebugger->writeLine(players->playersYZUID[1]);
	//myDebugger->writeLine(players->playersYZUID[2]);
	//myDebugger->writeLine(players->playersYZUID[3]);

	//players->callDLLPlay(pathname, deck);

	//fileManager->outputRanks(*players);
}

CString BackProcess::playerCardsWString()
{
	CString cstr(players->playersCardsString().c_str(), players->playersCardsString().length());
	return cstr;
}


FileManagement::FileManager& BackProcess::getFileManager()
{
	return *fileManager;
}