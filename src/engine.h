/*
 * Source code for engine.h by Igor Podsechin
 *
 * This file contains the header for the engine class.
 *
 */

#include "evaluation.h"

class Engine
{
private:
    // tree search depth
    const int DEPTH = 3; 
public:
    Engine();

    void make_computer_move(board_state &position);
    int minimax(board_state &position, int depth, bool maximizingPlayer);
    bool game_is_over(board_state &position);
};
