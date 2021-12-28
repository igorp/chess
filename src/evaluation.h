/*
 * Source code for evaluation.h by Igor Podsechin
 *
 * This file contains the header for the evaluation class.
 *
 */

#include "board.h"

class Evaluation
{
private:
    static int PAWN_TABLE_WHITE[8][8];
    static int KNIGHT_TABLE_WHITE[8][8];
    static int BISHOP_TABLE_WHITE[8][8];
    static int ROOK_TABLE_WHITE[8][8];
    static int QUEEN_TABLE_WHITE[8][8];
    static int KING_MIDDLE_TABLE_WHITE[8][8];
    static int KING_END_TABLE_WHITE[8][8];

    static int PAWN_TABLE_BLACK[8][8];
    static int KNIGHT_TABLE_BLACK[8][8];
    static int BISHOP_TABLE_BLACK[8][8];
    static int ROOK_TABLE_BLACK[8][8];
    static int QUEEN_TABLE_BLACK[8][8];
    static int KING_MIDDLE_TABLE_BLACK[8][8];
    static int KING_END_TABLE_BLACK[8][8];

    static void copy_table(int source[8][8], int destination[8][8]);
public:
    static void initialize_piece_square_tables();
    static int evaluate(board_state &position, int d);
    static int moves;
};
