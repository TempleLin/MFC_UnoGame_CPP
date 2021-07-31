#include "pch.h"
#include "players.h"
#include "threadProcess.h"
#include "backProcess.h"

Player::Player(int& giveCardsCount, std::vector<CCard>& cards)
{
	drawFromTable(giveCardsCount, cards);
}

int Player::getCardCount() { cardCount = holdCards.size(); return cardCount; }
std::vector<CCard>& Player::getHoldCards() { return holdCards; }

void Player::drawFromTable(int& drawCount, std::vector<CCard>& cards)
{
	for (int i = 0; i < drawCount; i++)
	{
		CCard temp = cards[cards.size() - 1];
		holdCards.push_back(temp);
		cards.pop_back();
	}
}

Players::Players(int& drawCount, std::vector<CCard>& cards)
{

	callPlay = nullptr;
	pThread = nullptr;

	for (int i = 0; i < 4; i++)
	{
		players.push_back(Player(drawCount, cards));
	}

	for (int i = 0; i < players.size(); i++)
		ranking.push_back(0);
}

Players::~Players()
{
}

void Players::storePlayersID(std::string pathname, int playersCount)
{
	for (const auto& entry : std::filesystem::directory_iterator(pathname))
	{
		playersCount--;
		playersYZUID.push_back(entry.path().filename().string().substr(0, 10));
		if (!playersCount)
			break;
	}
}



// start playing by calling each player's Play function one by one
void Players::callDLLPlay(std::string pathname, std::vector<CCard>& deck)
{
#ifdef CALL_DLL

	MyDebugger myDebugger;
	CardEdit cardEdit;

	bool endGame = false;

	auto errorEndGame = [&](int _playerIndex, int type) {
		endGame = true;
		ranking.at(_playerIndex) = type;
	};

	// turns true when player throws reverse card
	bool reverseOrder{ false };
	// if reverseOrder is true, the game will skip players for selected times
	int coroutineWait{ 0 };
	// if player wants to draw card
	int pickCardCoroutine{ 0 };


	while (!endGame)
	{


		try
		{

			//// 1 = start throw card, 2 = tell player to throw card, 
			//// 3 = response to player asking to draw one more card
			//static int state{ 1 };

			//// the card to give if player asks to draw card
			//static card one{ 0,0,0 };

			//// last four cards thrown on table
			//static std::vector<card> others{ {0,0,0} };

			//// player's response through passing by reference
			//// 1 = throw card, 2 = asks for draw card, 3 = throw the drawn card
			//static int command{ 0 };

			//// player's response through passing by reference
			//// reply is the card thrown by player
			//static card reply{ 0,0,0 };

			//// player's response through passing by reference
			//// only responses when the player throws color changing card
			//// the number is the card the player wants to switch color to
			//static int extra{ 0 };


			// find all files in a directory
			for (const auto& entry : std::filesystem::directory_iterator(pathname))
			{
				try
				{
					if (coroutineWait || pickCardCoroutine)
					{
						if (coroutineWait)
							coroutineWait--;
						if (pickCardCoroutine)
							pickCardCoroutine--;
						playerIndex++;
						if (playerIndex == players.size())
							playerIndex = 0;
						continue;
					}


					myDebugger.writeLine("----------------------------------------------------------");
					myDebugger.writeLine(entry.path().u8string());


					HINSTANCE hInstance = LoadLibrary(entry.path().wstring().c_str());
					if (!hInstance)
					{
						errorEndGame(playerIndex, PLAYERROR);
						myDebugger.writeLine("File Error:" + entry.path().string());
					}
					callPlay = (CallPlay)GetProcAddress(hInstance, "Play");
					if(!callPlay)
						callPlay = (CallPlay)GetProcAddress(hInstance, "play");
					myDebugger.writeLine(callPlay ?
						"Succeessfully linked to player." : "Failed to link to player.");
					if (!callPlay)
						errorEndGame(playerIndex, PLAYERROR);


					/*TODO: write the first time give player seven cards.
					* add cards to others.
					*/
					if (endGame)
						break;
					std::cout << players[0].getHoldCards().size() << std::endl;


					// set current player's cards to others
					PlayerCallParams::cards = players[playerIndex].getHoldCards();


					pThread = AfxBeginThread(ThreadProcess::runThread, this);
					DWORD dwRet = WaitForSingleObject(pThread->m_hThread, 2000);
					switch (dwRet)
					{
					case WAIT_TIMEOUT:
						myDebugger.writeLine("DLL Thread Time Out");
						errorEndGame(playerIndex, TIMEOUT);
						break;
					case WAIT_ABANDONED:
						myDebugger.writeLine("DLL Thread Abnormal");
						errorEndGame(playerIndex, ABANDONED);
						break;
					default:
						myDebugger.writeLine("DLL Thread OK");
					}

					myDebugger.writeLine("----------------------------------------------------------");


					//start judging and give calculate ranks if game needs to end
					gameJudge(PlayerCallParams::command, PlayerCallParams::reply, PlayerCallParams::cards, pickCardCoroutine,
						PlayerCallParams::others, PlayerCallParams::extra, deck, reverseOrder, PlayerCallParams::state, PlayerCallParams::one, coroutineWait, endGame);

					// if winner appears, end game

					for (int i = 0; i < players.size(); i++)
					{
						cardEdit.sortCards(players.at(i).getHoldCards());
					}

					backProcess->getPaintGame().setPaintGame(*this);
					CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
					CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
					pChild->Invalidate();

					for (int i = 0; i < players.size(); i++)
					{
						if (players.at(i).getHoldCards().size() == 0)
							endGame = true;
					}

					if (endGame)
						break;

					 
					// if reverseOrder is true, the game will skip two players every time
					// so that it works like reverse
					if (reverseOrder)
					{
						coroutineWait = players.size() - 2;
					}
					playerIndex++;
					if (playerIndex == players.size())
					{
						playerIndex = 0;
						//break;
					}

					FreeLibrary(hInstance);
				}
				catch(...)
				{
					myDebugger.writeLine("Error happened in while");
					errorEndGame(playerIndex, PLAYERROR);
				}

			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			myDebugger.writeLine(e.what());
		}
	}
	backProcess->getFileManager().outputMoves("GAME END");
#endif //CALL_DLL
}

void Players::gameJudge(int& command, CCard& reply, std::vector<CCard>& cards, int& pickCardCoroutine,
	std::vector<CCard>& others, int& extra, std::vector<CCard>& deck, bool& reverseOrder, int& state, CCard& one, int& coroutineWait, bool &endGame)
{
#ifdef JUDGE_GAME

	MyDebugger myDebugger;

	LPCTSTR listBoxPrint;

	// lambda for finding if the card is in the player's hand
	auto ifPlayerHasCard = [&](CCard& _card) {
		bool playerHasCard{ false };
		for (auto it = players[playerIndex].getHoldCards().begin();
			it != players[playerIndex].getHoldCards().end();
			++it)
		{
			if (*it == _card) playerHasCard = true;
		}
		return playerHasCard;
	};

	// lambda for two purposes
	// 1.if deck is out of cards
	// 2.for getting called by "foulGetRanks" lambda
	auto getRanks = [&]() {

		int lastMaxCount{ 0 };

		std::vector<int> indexesTemp(players.size());
		std::vector<Player> tempPlayers = players;

		for (int i = 0; i < indexesTemp.size(); i++)
		{
			indexesTemp.at(i) = i + 1;
		}

		for (int i = tempPlayers.size() - 1; i >= 0; i--)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (tempPlayers.at(j).getCardCount() > tempPlayers.at(i).getCardCount())
				{
					Player temp = tempPlayers.at(j);
					tempPlayers.at(j) = tempPlayers.at(i);
					tempPlayers.at(i) = temp;

					int temp2 = indexesTemp.at(j);
					indexesTemp.at(j) = indexesTemp.at(i);
					indexesTemp.at(i) = temp2;
				}
			}
		}

		for (int i = 0; i < indexesTemp.size(); i++)
		{
			for (int j = 0; j < indexesTemp.size(); j++)
			{
				if (indexesTemp.at(j) == i + 1)
					ranking.at(i) = j + 1;
			}
		}
		myDebugger.writeLine("Get ranks");
		endGame = true;
		return;
		//players.clear();
	};

	// lambda for ending the game when loser found
	auto foulGetRanks = [&]() {
		// foul player gets last, remaining three gets determined
		myDebugger.writeLine("foul");
		getRanks();
		for (std::vector<int>::iterator it = ranking.begin(); it != ranking.end(); ++it)
		{
			// set the foul player (which is this player) to -1
			if (it - ranking.begin() == playerIndex) *it = FOULRANK;
		}
		// sort ranking after foul player gets removed from rank
		int rankFromLowest = ranking.size() - 1;
		int sortedCount = 0;
		for (int i = 0; i < ranking.size() && sortedCount != ranking.size() - 1; i++)
		{
			for (int j = 0; j < ranking.size() && sortedCount != ranking.size() - 1; j++)
			{
				if (ranking.at(j) >= rankFromLowest)
				{
					ranking.at(j) = rankFromLowest;
					rankFromLowest--;
					sortedCount++;
				}
			}
		}
	};

	// lambda for getting the next player index
	auto nextPlayerIndex = [&] {
		if (!coroutineWait)
		{
			if (playerIndex == players.size() - 1)
				return 0;
			return playerIndex + 1;
		}
		else
		{
			if (!playerIndex)
				return static_cast<int>(players.size()) - 1;
			return playerIndex - 1;
		}
	}();

	auto cardDetermine = [&] (CCard& _card){

		if (command == 1 && !ifPlayerHasCard(_card))
		{
			foulGetRanks(); myDebugger.writeLine("PLAYER DOESN'T HAVE THROWN CARD");
			return;
		}

		if (!static_cast<int>(_card.type) && !_card.id && !static_cast<int>(_card.color))
		{
			foulGetRanks(); myDebugger.writeLine("PLAYER WANTS TO THROW CARD BUT DIDN'T ASSIGN");
			return;
		}

		myDebugger.playerAction(playerIndex, "has card");

		if (static_cast<int>(_card.type) == TYPE_CHANGE || static_cast<int>(_card.type) == TYPE_ADDFOUR)
		{
#define OTHERSLAST others.at(others.size() - 1)

							//TODO: add card effects
			if (!_card.id)
			{
				switch (static_cast<int>(_card.type))
				{
				case TYPE_CHANGE:
					if (extra && extra > 0 && extra < 5)
						_card.color = static_cast<Color>(extra);
					else
					{
						foulGetRanks(); backProcess->getFileManager().outputMoves("CHANGE COLOR CARD BUT EXTRA NOT RIGHT");
					}
					break;
				case TYPE_ADDFOUR:
					if (extra && extra > 0 && extra < 5)
					{
						if (deck.size() > 3)
						{
							int drawFourTimes{ 4 };
							players[nextPlayerIndex].drawFromTable(drawFourTimes, deck);
							coroutineWait = 1;
							_card.color = static_cast<Color>(extra);
						}
						else if (deck.size() > 0 && deck.size() < 4)
						{
							int drawTimes{ static_cast<int>(deck.size()) };
							players[nextPlayerIndex].drawFromTable(drawTimes, deck);
							coroutineWait = 1;
							_card.color = static_cast<Color>(extra);
						}
						else
						{
							getRanks();
						}
					}
					else
					{
						foulGetRanks(); backProcess->getFileManager().outputMoves("ADD FOUR CARD BUT EXTRA NOT RIGHT");
					}
					break;
				}
			}
			else
			{
				foulGetRanks(); backProcess->getFileManager().outputMoves("TYPE CHANGE OR TYPE ADDFOUR BUT HAS ID");
			}
		}
		else if (static_cast<int>(_card.type) == TYPE_REVERSE)
		{
			if (_card.color == OTHERSLAST.color)
			{
				reverseOrder = !reverseOrder;
				myDebugger.writeLine("REVERSED ORDER");
			}
			else
			{
				foulGetRanks(); 
				backProcess->getFileManager().outputMoves("TYPE REVERSE BUT COLOR DOESN'T MATCH OTHERS LAST CARD");
			}
		}
		else if (static_cast<int>(_card.type) == TYPE_ADDTWO)
		{
			if (_card.color == OTHERSLAST.color)
			{
				int drawTwoTimes{ 2 };
				if (deck.size() >= drawTwoTimes)
					players[nextPlayerIndex].drawFromTable(drawTwoTimes, deck);
				else
				{
					int drawTimes{ static_cast<int>(deck.size()) };
					players[nextPlayerIndex].drawFromTable(drawTimes, deck);
				}
				//next player can't draw card
				coroutineWait = 1;
			}
			else
			{
				foulGetRanks(); backProcess->getFileManager().outputMoves("TYPE ADDTWO BUT COLOR DOESN'T MATCH OTHERS LAST CARD");
			}
		}
		else if (static_cast<int>(_card.type) == TYPE_SKIP)
		{
			if (_card.color == OTHERSLAST.color)
			{
				coroutineWait = 1;
			}
			else
			{
				foulGetRanks(); backProcess->getFileManager().outputMoves("TYPE SKIP BUT COLOR DOESN'T MATCH OTHERS LAST CARD");
			}
		}
		else
		{
			if (OTHERSLAST.color == _card.color
				|| OTHERSLAST.id == _card.id)
			{

			}
			else if ((int)OTHERSLAST.type == TYPE_ADDFOUR || (int)OTHERSLAST.color == BLACK)
			{

			}
			else
			{
				foulGetRanks(); backProcess->getFileManager().outputMoves("TYPE NORMAL BUT NO COLOR NOR ID MATCH OTHERS LAST CARD");
			}
		}

		if (!endGame)
		{
			for (std::vector<CCard>::iterator it = cards.begin(); it != cards.end(); ++it)
			{
				if (*it == _card)
				{
					players[playerIndex].getHoldCards().erase(players[playerIndex].getHoldCards().begin() + (it - cards.begin()));
					others.push_back(_card);
					if (others.size() > 4)
						others.erase(others.begin());
					break;
				}
				else if (it->type == static_cast<Type>(TYPE_ADDFOUR) || it->type == static_cast<Type>(TYPE_CHANGE))
				{
					if (it->type == _card.type)
					{
						players[playerIndex].getHoldCards().erase(players[playerIndex].getHoldCards().begin() + (it - cards.begin()));
						others.push_back(_card);
						if (others.size() > 4)
							others.erase(others.begin());
						break;
					}
				}
			}
		}
		
		/*else
		{
			foulGetRanks(); myDebugger.writeLine("PLAY DOESN'T HAVE THROWN CARD");
		}*/
	};

	backProcess->getFileManager().outputMoves("Player: " + playersYZUID.at(playerIndex));

	std::wstring wPlayerReplies;
	auto setListBox = [&](std::string _playerReplies){
		backProcess->getFileManager().outputMoves(_playerReplies);
		wPlayerReplies = std::wstring(_playerReplies.begin(), _playerReplies.end());
		backProcess->getFileManager().setListBoxString(wPlayerReplies.c_str());
	};

	if (state == 1)
	{
		backProcess->getFileManager().outputMoves("EXE state 1");
		if (playerIndex == ranking.size() - 1)
			state = 2;
		return;
	}
	// if player throws card
	else if (state == 2 && command == 1)
	{
		backProcess->getFileManager().outputMoves("EXE state 2");

		setListBox("Player" + std::to_string(playerIndex) + " replies: [" + std::to_string(static_cast<int>(reply.type)) + ","
			+ std::to_string(reply.id) + "," + std::to_string(static_cast<int>(reply.color)) + "]");

		//std::string playerReplies{ "Player replies: [" + std::to_string(static_cast<int>(reply.type)) + ","
		//	+ std::to_string(reply.id) + "," + std::to_string(static_cast<int>(reply.color)) + "]" };
		//backProcess->getFileManager().outputMoves(playerReplies);

		//TODO:add thrown card to others,
		// check the others only has four cards
		// check if the card is legal and qualified
		// add all special cards effects
		// make a lambda to kick player (deprecated, no need to kick player)
		// calculate each player's cards amount if can't have winner
		// add ranking
		// write down player rank in a log file
		// when state == 1, player doesn't need to throw cards, ignore them
		// end game when table is empty
		// add judgement against player's command
		// add code to change state
		cardDetermine(reply);
	}
	else if ((state == 2 || state == 3) && command == 2)
	{
		backProcess->getFileManager().outputMoves("EXE state 2 or 3, command 2");
		// if player still needs to draw card, put "one" to his/her held cards
		if (state == 3)
			players[playerIndex].getHoldCards().push_back(one);

		// state wil be 3 for next player (same player will be called)
		state = 3;

		if (!deck.empty())
		{
			one = deck.at(deck.size() - 1);
			setListBox("Player" + std::to_string(playerIndex) + " draws: [" + std::to_string(static_cast<int>(one.type)) + ","
				+ std::to_string(one.id) + "," + std::to_string(static_cast<int>(one.color)) + "]");
			deck.pop_back();

			// this is for letting the game call the same player next
			//if (!reverseOrder)
			//	//playerIndex--;
			//	pickCardCoroutine = players.size() - 1;
			//else
			//	//playerIndex -= players.size() - 2;
			//	pickCardCoroutine = players.size();
			pickCardCoroutine = players.size() - 1;
		}
		else
		{
			getRanks();
		}
	}
	else if (state == 3 && command == 3)
	{
		backProcess->getFileManager().outputMoves("EXE state 3, command 3");
		
		//backProcess->getFileManager().outputMoves("Player ones: [" + std::to_string(static_cast<int>(one.type)) + ","
		//	+ std::to_string(one.id) + "," + std::to_string(static_cast<int>(one.color)) + "]");
		setListBox("Player" + std::to_string(playerIndex) + " ones: [" + std::to_string(static_cast<int>(one.type)) + ","
			+ std::to_string(one.id) + "," + std::to_string(static_cast<int>(one.color)) + "]");
		//cardDetermine(one);

		// calling next player will turn back to state 2
		state = 2;
	}
	else
	{
		myDebugger.writeLine("else");
		foulGetRanks(); myDebugger.writeLine("STATE AND COMMAND DOESN'T MATCH ANY REQUIREMENT");
	}

	std::string outputMoveInfo;

	backProcess->getFileManager().outputMoves("Deck size: " + std::to_string(deck.size()));

	backProcess->getFileManager().outputMoves("State:" + std::to_string(state));

	backProcess->getFileManager().outputMoves("Card type:" + std::to_string(static_cast<int>(reply.type)));

	backProcess->getFileManager().outputMoves("Card ID:" + std::to_string(reply.id));

	backProcess->getFileManager().outputMoves("Card color:" + std::to_string(static_cast<int>(reply.color)));

	backProcess->getFileManager().outputMoves("OTHERS LAST type:" + std::to_string(static_cast<int>(OTHERSLAST.type)));

	backProcess->getFileManager().outputMoves("OTHERS LAST ID:" + std::to_string(OTHERSLAST.id));

	backProcess->getFileManager().outputMoves("OTHERS LAST color:" + std::to_string(static_cast<int>(OTHERSLAST.color)));
	//myDebugger.writeLine("Deck size: " + std::to_string(deck.size()));
	//fileManagement.outputMoves("Deck size: " + std::to_string(deck.size()));
	//myDebugger.writeLine("State:" + std::to_string(state));
	//fileManagement.outputMoves("State:" + std::to_string(state));
	//myDebugger.writeLine("Card type:" + std::to_string(static_cast<int>(reply.type)));
	//fileManagement.outputMoves("Card type:" + std::to_string(static_cast<int>(reply.type)));
	//myDebugger.writeLine("Card ID:" + std::to_string(reply.id));
	//fileManagement.outputMoves("Card ID:" + std::to_string(reply.id));
	//myDebugger.writeLine("Card color:" + std::to_string(static_cast<int>(reply.color)));
	//fileManagement.outputMoves("Card color:" + std::to_string(static_cast<int>(reply.color)));
	//myDebugger.writeLine("OTHERS LAST type:" + std::to_string(static_cast<int>(OTHERSLAST.type)));
	//fileManagement.outputMoves("OTHERS LAST type:" + std::to_string(static_cast<int>(OTHERSLAST.type)));
	//myDebugger.writeLine("OTHERS LAST ID:" + std::to_string(OTHERSLAST.id));
	//myDebugger.writeLine("OTHERS LAST color:" + std::to_string(static_cast<int>(OTHERSLAST.color)));

	//backProcess->getPaintGame().setPaintGame(*this);
	//CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//CMDIChildWnd* pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	//pChild->Invalidate();

	// prints player actions if rank is not set (rank is set when player gets empty)
	if (players.at(playerIndex).getCardCount() == 0)
	{
		getRanks();
	}

#endif // JUDGE_GAME
}


std::string Players::playersCardsString()
{
	std::string toReturn;
	for (int i = 0; i < players.size(); i++)
	{
		toReturn += ("Player" + std::to_string(i + 1) + " : ");
		for (int j = 0; j < players.at(i).getCardCount(); j++)
		{
			toReturn += ("[" + std::to_string(static_cast<int>(players.at(i).getHoldCards().at(j).type)) + ",");
			toReturn += (std::to_string(players.at(i).getHoldCards().at(j).id) + ",");
			toReturn += (std::to_string(static_cast<int>(players.at(i).getHoldCards().at(j).color)) + "]");
			if (!(j % 3))
				toReturn += ";";
		}
	}
	return toReturn;
}