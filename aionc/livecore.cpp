/*
	AEON programming language standalone interpreter.
	Author: Artur Moreira
*/
#include "livecore.h"
#include <iostream>

livecore::livecore()
{
	m_state = STATE_TESTS;

	m_rootmenu.add("1", "1. Tests", []()
	{

	});
	m_rootmenu.add("2", "2. Live code", []()
	{

	});
	m_rootmenu.add("0", "0. Close", []()
	{

	});

	m_currentMenu = &m_rootmenu;
}

void livecore::prompt()
{
	switch (m_state)
	{
	case STATE_LIVECODE:
	{
						   std::cout << "> ";
						   break;
	}

		case STATE_TESTS:
		{
			std::cout << std::endl;
			for (auto& option : m_currentMenu->items)
			{
				std::cout << option.name << std::endl;
			}
			break;
		}
	}
}

void livecore::run()
{
	std::string input;
	while (input != "q")
	{
		prompt();

		std::getline(std::cin, input);

		switch (m_state)
		{
		case STATE_LIVECODE:

			break;

		case STATE_TESTS:

			break;

		case STATE_EXITING:
			input = "q";
			break;
		}
	}

	std::cout << "Good bye! Please come again!" << std::endl;
}