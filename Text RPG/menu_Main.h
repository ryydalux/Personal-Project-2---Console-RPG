#pragma once


void manage_MainMenu(Player& player);
void print_mainMenuHeader();
void openingMenu();
int mainMenu(int& back_rememberOption);































//-Use a for loop where player can press W or S to navigate and hit ENTER to select option. when player hits S, i-- if player hits W i++ . its basically an infinite loop that only breaks if player hits ENTER
//if user enters anything else than W, S or ENTER, decrement i and continue; this makes it so loop stays on the same iteration.
//if iteration goes out of bounds (loop condition not met), reset i to the opposite option.
