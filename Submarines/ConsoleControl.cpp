#include "ConsoleControl.h"



void SetColour(const CONSOLECOLOUR foreground, const CONSOLECOLOUR background)
{

    int Color = foreground + (background * 16);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, Color);
}

void SetCursor(int x, int y)
{
  HANDLE hStdout;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
  csbiInfo.dwCursorPosition.X=x;
  csbiInfo.dwCursorPosition.Y=y;
  SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
}

void Clear()
{
	HANDLE hConsole=GetConsoleWindow();
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize;
	// Get the number of character cells in the current buffer. 
	if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
	{
		return;
	}
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// Fill the entire screen with blanks.
	if( !FillConsoleOutputCharacter( hConsole,        // Handle to console screen buffer 
							(TCHAR) ' ',     // Character to write to the buffer
							dwConSize,       // Number of cells to write 
							coordScreen,     // Coordinates of first cell 
							&cCharsWritten ))// Receive number of characters written
	{
		return;
	}
	// Get the current text attribute.
	if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
	{
		return;
	}
	// Set the buffer's attributes accordingly.
	if( !FillConsoleOutputAttribute( hConsole,         // Handle to console screen buffer 
							csbi.wAttributes, // Character attributes to use
							dwConSize,        // Number of cells to set attribute 
							coordScreen,      // Coordinates of first cell 
							&cCharsWritten )) // Receive number of characters written
	{
		return;
	}
	// Put the cursor at its home coordinates.
	SetConsoleCursorPosition( hConsole, coordScreen );
}