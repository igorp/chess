/*
 * Source code for engine.cpp by Igor Podsechin
 *
 * This class contains the minimax function that calculates the engine's best move.
 *
 */

#include <iostream>
#include <vector>
#include <stdlib.h>  
#include <time.h>
#include <algorithm>
#include "engine.h"
#include <string>

using namespace std;

int paths = 0;

// Constructor for the Engine class.
Engine::Engine()
{
    Evaluation::initialize_piece_square_tables();
}

// This is the top function that get's called in the main chess.cpp file.
// It goes through all legal moves in the position that's given as the parameter and for each 
// of them calls the minimax function, which does the actual calculation for the best move.
// Minimax gives out a score to each possible move and the best one is then selected and 
// played by the computer.
void Engine::make_computer_move(board_state & position)
{
    int best_score = INT_MAX;
    board_state possible_position;
    board_state best_position;
    string computer_move_notation;

    Board::reset_en_passant(position, BLACK);

    for (int m = 0; m < 8; m++)
    {
        for (int n = 0; n < 8; n++)
        {
            if (position.board[m][n] < 0)
            {
                if (position.board[m][n] == BLACK_PAWN)
                {
                    // one move forward by pawn 
                    if (position.board[m + 1][n] == 0)
                    {
                        // if not moving to last rank
                        if (m + 1 != 7)
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m + 1][n] = BLACK_PAWN;
                            possible_position.board[m][n] = 0;

                            if (!Board::king_is_in_check(possible_position.board, BLACK)) 
                            {
                                int score = minimax(possible_position, DEPTH, true);
                                if (score < best_score)
                                {
                                    best_position = Board::copy_position(possible_position);
                                    best_score = score;
                                    computer_move_notation = (char)(n + 97);
                                    computer_move_notation += (55 - m);
                                }
                            }
                        }
                        // if moves last rank, then cycle through promotion of all pieces
                        else 
                        {
                            // variable i denotes possible pieces from black knight to queen
                            for (char i = -5; i <= -2; i++) 
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m + 1][n] = i;
                                possible_position.board[m][n] = 0;

                                if (!Board::king_is_in_check(possible_position.board, BLACK)) 
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                    }
                                }
                            }
                        }
                    }
                    // two moves from starting position by pawn
                    if (m == 1
                        && position.board[m + 1][n] == 0
                        && position.board[m + 2][n] == 0) 
                    {
                        possible_position = Board::copy_position(position);
                        possible_position.board[m + 2][n] = BLACK_PAWN;
                        possible_position.board[m][n] = 0;

                        possible_position.pawn_two_squares_black[n] = true;

                        if (!Board::king_is_in_check(possible_position.board, BLACK))
                        {
                            int score = minimax(possible_position, DEPTH, true);
                            if (score < best_score)
                            {
                                best_position = Board::copy_position(possible_position);
                                best_score = score;
                                computer_move_notation = (char)(n + 97);
                                computer_move_notation += (54 - m);
                            }
                        }
                    }
                    // capture to the left 
                    if (n != 0 && position.board[m + 1][n - 1] > 0)
                    {
                        if (m + 1 != 7)
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m + 1][n - 1] = BLACK_PAWN;
                            possible_position.board[m][n] = 0;
                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                int score = minimax(possible_position, DEPTH, true);
                                if (score < best_score)
                                {
                                    best_position = Board::copy_position(possible_position);
                                    best_score = score;
                                    computer_move_notation = (char)(n + 97);
                                    computer_move_notation += (54 - m);
                                }
                            }
                        }
                        // if moves last rank, then cycle through promotion of all pieces
                        else
                        {
                            // variable i is possible pieces from black knight to queen
                            for (char i = -5; i <= -2; i++)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m + 1][n - 1] = i;
                                possible_position.board[m][n] = 0;

                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                    }
                                }
                            }
                        }
                    }
                    // capture to the right
                    if (n != 7 && position.board[m + 1][n + 1] > 0)
                    {
                        if (m + 1 != 7)
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m + 1][n + 1] = BLACK_PAWN;
                            possible_position.board[m][n] = 0;
                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                int score = minimax(possible_position, DEPTH, true);
                                if (score < best_score)
                                {
                                    best_position = Board::copy_position(possible_position);
                                    best_score = score;
                                }
                            }
                        }
                        // if moves last rank, then cycle through promotion of all pieces
                        else 
                        {
                            // variable i is possible pieces from black knight to queen
                            for (char i = -5; i <= -2; i++)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m + 1][n + 1] = i;
                                possible_position.board[m][n] = 0;

                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                    }
                                }
                            }
                        }
                    }

                    // en passant to the right
                    if (n != 7
                        && position.board[m][n + 1] == 1
                        && position.pawn_two_squares_white[n + 1])
                    {
                        possible_position = Board::copy_position(position);
                        possible_position.board[m + 1][n + 1] = BLACK_PAWN;
                        possible_position.board[m][n] = 0;
                        possible_position.board[m][n + 1] = 0;
                        if (!Board::king_is_in_check(possible_position.board, BLACK))
                        {
                            int score = minimax(possible_position, DEPTH, true);
                            if (score < best_score)
                            {
                                best_position = Board::copy_position(possible_position);
                                best_score = score;
                            }
                        }
                    }

                    // en passant to the left
                    if (n != 0
                        && position.board[m][n - 1] == 1
                        && position.pawn_two_squares_white[n - 1])
                    {
                        possible_position = Board::copy_position(position);
                        possible_position.board[m + 1][n - 1] = BLACK_PAWN;
                        possible_position.board[m][n] = 0;
                        possible_position.board[m][n - 1] = 0;
                        if (!Board::king_is_in_check(possible_position.board, BLACK))
                        {
                            int score = minimax(possible_position, DEPTH, true);
                            if (score < best_score)
                            {
                                best_position = Board::copy_position(possible_position);
                                best_score = score;
                            }
                        }
                    }
                }

                if (position.board[m][n] == BLACK_KNIGHT)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        int k_i = m + knight_move[k][0];
                        int k_j = n + knight_move[k][1];
                        if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8)
                        {
                            if (Board::under_knight_control(m, n, k_i, k_j)
                                && position.board[k_i][k_j] >= 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[k_i][k_j] = BLACK_KNIGHT;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                        computer_move_notation = 'N';
                                        computer_move_notation += (char)(k_j + 97);
                                        computer_move_notation += (56 - k_i);
                                    }
                                }
                            }
                        }
                    }
                }

                if (position.board[m][n] == BLACK_BISHOP)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        int path_i = m + bishop_direction[k][0];
                        int path_j = n + bishop_direction[k][1];
                        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                        {
                            if (Board::under_bishop_control(position.board, m, n, path_i, path_j)
                                && position.board[path_i][path_j] >= 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[path_i][path_j] = BLACK_BISHOP;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                        computer_move_notation = 'B';
                                        computer_move_notation += (char)(path_j + 97);
                                        computer_move_notation += (56 - path_i);
                                    }
                                }
                            }
                            // if next square is not empty, we stop
                            if (position.board[path_i][path_j] != 0)
                            {
                                break;
                            }
                            path_i += bishop_direction[k][0];
                            path_j += bishop_direction[k][1];
                        }
                    }
                }

                if (position.board[m][n] == BLACK_ROOK)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        int path_i = m + rook_direction[k][0];
                        int path_j = n + rook_direction[k][1];
                        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                        {
                            if (Board::under_rook_control(position.board, m, n, path_i, path_j)
                                && position.board[path_i][path_j] >= 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[path_i][path_j] = BLACK_ROOK;
                                possible_position.board[m][n] = 0;
                                if (m == 0 && n == 0)
                                {
                                    possible_position.can_castle_black[0] = false;
                                }
                                if (m == 0 && n == 7)
                                {
                                    possible_position.can_castle_black[1] = false;
                                }
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                        computer_move_notation = 'R';
                                        computer_move_notation += (char)(path_j + 97);
                                        computer_move_notation += (56 - path_i);
                                    }
                                }
                            }
                            // if next square is not empty, we stop
                            if (position.board[path_i][path_j] != 0)
                            {
                                break;
                            }
                            path_i += rook_direction[k][0];
                            path_j += rook_direction[k][1];
                        }
                    }
                }

                if (position.board[m][n] == BLACK_QUEEN)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        int path_i = m + every_direction[k][0];
                        int path_j = n + every_direction[k][1];
                        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                        {
                            if (Board::under_queen_control(position.board, m, n, path_i, path_j)
                                && position.board[path_i][path_j] >= 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[path_i][path_j] = BLACK_QUEEN;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                        computer_move_notation = 'Q';
                                        computer_move_notation += (char)(path_j + 97);
                                        computer_move_notation += (56 - path_i);
                                    }
                                }
                            }
                            // if next square is not empty, we stop
                            if (position.board[path_i][path_j] != 0)
                            {
                                break;
                            }
                            path_i += every_direction[k][0];
                            path_j += every_direction[k][1];
                        }
                    }
                }

                if (position.board[m][n] == BLACK_KING)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        int k_i = m + every_direction[k][0];
                        int k_j = n + every_direction[k][1];
                        if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8)
                        {
                            if (Board::under_king_control(m, n, k_i, k_j)
                                && position.board[k_i][k_j] >= 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[k_i][k_j] = BLACK_KING;
                                possible_position.board[m][n] = 0;
                                possible_position.can_castle_black[0] = false;
                                possible_position.can_castle_black[1] = false;
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    int score = minimax(possible_position, DEPTH, true);
                                    if (score < best_score)
                                    {
                                        best_position = Board::copy_position(possible_position);
                                        best_score = score;
                                        computer_move_notation = 'K';
                                        computer_move_notation += (char)(k_j + 97);
                                        computer_move_notation += (56 - k_i);
                                    }
                                }
                            }
                        }
                    }
                    // castling kingside
                    if (m == 0 && n == 4
                        && position.board[0][5] == 0 && position.board[0][6] == 0
                        && position.board[0][7] == BLACK_ROOK
                        && !Board::under_control(position.board, 0, 4, WHITE)
                        && !Board::under_control(position.board, 0, 5, WHITE)
                        && !Board::under_control(position.board, 0, 6, WHITE)
                        && position.can_castle_black[1])
                    {
                        possible_position = Board::copy_position(position);
                        possible_position.board[0][6] = BLACK_KING;
                        possible_position.board[0][4] = 0;
                        possible_position.board[0][7] = 0;
                        possible_position.board[0][5] = BLACK_ROOK;
                        possible_position.can_castle_black[0] = false;
                        possible_position.can_castle_black[1] = false;
                        if (!Board::king_is_in_check(possible_position.board, BLACK))
                        {
                            int score = minimax(possible_position, DEPTH, true);
                            if (score < best_score)
                            {
                                best_position = Board::copy_position(possible_position);
                                best_score = score;
                            }
                        }
                    }
                    // castling queenside
                    if (m == 0 && n == 4
                        && position.board[0][1] == 0 && position.board[0][2] == 0 && position.board[0][3] == 0
                        && position.board[0][0] == BLACK_ROOK
                        && !Board::under_control(position.board, 0, 2, WHITE)
                        && !Board::under_control(position.board, 0, 3, WHITE)
                        && !Board::under_control(position.board, 0, 4, WHITE)
                        && position.can_castle_black[0])
                    {
                        possible_position = Board::copy_position(position);
                        possible_position.board[0][2] = BLACK_KING;
                        possible_position.board[0][4] = 0;
                        possible_position.board[0][0] = 0;
                        possible_position.board[0][3] = BLACK_ROOK;
                        possible_position.can_castle_black[0] = false;
                        possible_position.can_castle_black[1] = false;
                        if (!Board::king_is_in_check(possible_position.board, BLACK))
                        {
                            int score = minimax(possible_position, DEPTH, true);
                            if (score < best_score)
                            {
                                best_position = Board::copy_position(possible_position);
                                best_score = score;
                            }
                        }
                    }
                }
            }
        }
    }

    // print out move information
    cout << computer_move_notation << endl;
    cout << "Nodes: " << paths << endl;
    paths = 0;
    cout << "Evaluation: " << (float)best_score / 100 << endl;

    position = Board::copy_position(best_position);
}

int Engine::minimax(board_state & position, int depth, bool maximizingPlayer)
{
    paths++;
    board_state possible_position;

    int score = Evaluation::evaluate(position, depth);
    if (depth == 0 || game_is_over(position))
    {
        return score;
    }

    // this is maximizer's move (choose best move for white)
    if (maximizingPlayer)
    {
        Board::reset_en_passant(position, WHITE);
        score = INT_MIN;
        for (int m = 0; m < 8; m++)
        {
            for (int n = 0; n < 8; n++)
            {
                if (position.board[m][n] > 0)
                {

                    if (position.board[m][n] == WHITE_PAWN)
                    {
                        // one move forward by pawn 
                        if (position.board[m - 1][n] == 0)
                        {
                            // if not move to last rank
                            if (m - 1 != 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m - 1][n] = WHITE_PAWN;
                                possible_position.board[m][n] = 0;

                                if (!Board::king_is_in_check(possible_position.board, WHITE))
                                {
                                    score = max(score, minimax(possible_position, depth - 1, false));
                                }
                            }
                            // if moves last rank, then promote to all pieces
                            else
                            {
                                // variable i is possible pieces from white knight to queen
                                for (char i = 2; i <= 5; i++)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[m - 1][n] = i;
                                    possible_position.board[m][n] = 0;

                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                            }
                        }
                        // two moves from starting position by pawn
                        if (m == 6
                            && position.board[m - 1][n] == 0
                            && position.board[m - 2][n] == 0)
                        {
                            possible_position = Board::copy_position(position); 
                            possible_position.board[m - 2][n] = WHITE_PAWN;
                            possible_position.board[m][n] = 0;

                            possible_position.pawn_two_squares_white[n] = true;

                            if (!Board::king_is_in_check(possible_position.board, WHITE))
                            {
                                score = max(score, minimax(possible_position, depth - 1, false));
                            }
                        }
                        // capture to the left 
                        if (n != 0
                            && position.board[m - 1][n - 1] < 0)
                        {
                            // not to last rank
                            if (m - 1 != 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m - 1][n - 1] = WHITE_PAWN;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, WHITE))
                                {
                                    score = max(score, minimax(possible_position, depth - 1, false));
                                }
                            }
                            else
                            {
                                // variable i is possible pieces from white knight to queen
                                for (char i = 2; i <= 5; i++)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[m - 1][n - 1] = i;
                                    possible_position.board[m][n] = 0;

                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                            }
                        }
                        // capture to the right
                        if (n != 7
                            && position.board[m - 1][n + 1] < 0)
                        {
                            // not last rank
                            if (m - 1 != 0)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m - 1][n + 1] = WHITE_PAWN;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, WHITE))
                                {
                                    score = max(score, minimax(possible_position, depth - 1, false));
                                }
                            }
                            // last rank (promote)
                            else
                            {
                                // variable i is possible pieces from white knight to queen
                                for (char i = 2; i <= 5; i++)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[m - 1][n + 1] = i;
                                    possible_position.board[m][n] = 0;

                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                            }
                        }

                        // en passant to the right
                        if (n != 7 && position.board[m][n + 1] == -1 && position.pawn_two_squares_black[n + 1])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m - 1][n + 1] = WHITE_PAWN;
                            possible_position.board[m][n] = 0;
                            possible_position.board[m][n + 1] = 0;
                            if (!Board::king_is_in_check(possible_position.board, WHITE))
                            {
                                score = max(score, minimax(possible_position, depth - 1, false));
                            }
                        }

                        // en passant to the left
                        if (n != 0 && position.board[m][n - 1] == -1 && position.pawn_two_squares_black[n - 1])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m - 1][n - 1] = WHITE_PAWN;
                            possible_position.board[m][n] = 0;
                            possible_position.board[m][n - 1] = 0;
                            if (!Board::king_is_in_check(possible_position.board, WHITE))
                            {
                                score = max(score, minimax(possible_position, depth - 1, false));
                            }
                        }
                    }

                    if (position.board[m][n] == WHITE_KNIGHT)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int k_i = m + knight_move[k][0];
                            int k_j = n + knight_move[k][1];
                            if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8)
                            {
                                if (Board::under_knight_control(m, n, k_i, k_j)
                                    && position.board[k_i][k_j] <= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[k_i][k_j] = WHITE_KNIGHT;
                                    possible_position.board[m][n] = 0;
                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                            }
                        }
                    }

                    if (position.board[m][n] == WHITE_BISHOP)
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            int path_i = m + bishop_direction[k][0];
                            int path_j = n + bishop_direction[k][1];
                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                            {
                                if (Board::under_bishop_control(position.board, m, n, path_i, path_j)
                                    && position.board[path_i][path_j] <= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[path_i][path_j] = WHITE_BISHOP;
                                    possible_position.board[m][n] = 0;
                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                                // if next square is not empty, we stop
                                if (position.board[path_i][path_j] != 0)
                                {
                                    break;
                                }
                                path_i += bishop_direction[k][0];
                                path_j += bishop_direction[k][1];
                            }
                        }
                    }

                    if (position.board[m][n] == WHITE_ROOK)
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            int path_i = m + rook_direction[k][0];
                            int path_j = n + rook_direction[k][1];
                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                            {
                                if (Board::under_rook_control(position.board, m, n, path_i, path_j)
                                    && position.board[path_i][path_j] <= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[path_i][path_j] = WHITE_ROOK;
                                    possible_position.board[m][n] = 0;
                                    if (m == 7 && n == 0)
                                    {
                                        possible_position.can_castle_white[0] = false;
                                    }
                                    if (m == 7 && n == 7)
                                    {
                                        possible_position.can_castle_white[1] = false;
                                    }
                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                                // if next square is not empty, we stop
                                if (position.board[path_i][path_j] != 0)
                                {
                                    break;
                                }
                                path_i += rook_direction[k][0];
                                path_j += rook_direction[k][1];
                            }
                        }
                    }

                    if (position.board[m][n] == WHITE_QUEEN)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int path_i = m + every_direction[k][0];
                            int path_j = n + every_direction[k][1];
                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                            {
                                if (Board::under_queen_control(position.board, m, n, path_i, path_j)
                                    && position.board[path_i][path_j] <= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[path_i][path_j] = WHITE_QUEEN;
                                    possible_position.board[m][n] = 0;
                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                                // if next square is not empty, we stop
                                if (position.board[path_i][path_j] != 0)
                                {
                                    break;
                                }
                                path_i += every_direction[k][0];
                                path_j += every_direction[k][1];
                            }
                        }
                    }

                    if (position.board[m][n] == WHITE_KING)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int k_i = m + every_direction[k][0];
                            int k_j = n + every_direction[k][1];
                            if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8)
                            {
                                if (Board::under_king_control(m, n, k_i, k_j)
                                    && position.board[k_i][k_j] <= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[k_i][k_j] = WHITE_KING;
                                    possible_position.board[m][n] = 0;
                                    possible_position.can_castle_white[0] = false;
                                    possible_position.can_castle_white[1] = false;
                                    if (!Board::king_is_in_check(possible_position.board, WHITE))
                                    {
                                        score = max(score, minimax(possible_position, depth - 1, false));
                                    }
                                }
                            }
                        }
                        // castling kingside
                        if (m == 7 && n == 4
                            && position.board[7][5] == 0 && position.board[7][6] == 0
                            && position.board[7][7] == WHITE_ROOK
                            && !Board::under_control(position.board, 7, 4, BLACK)
                            && !Board::under_control(position.board, 7, 5, BLACK)
                            && !Board::under_control(position.board, 7, 6, BLACK)
                            && position.can_castle_white[1])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[7][6] = WHITE_KING;
                            possible_position.board[7][4] = 0;
                            possible_position.board[7][7] = 0;
                            possible_position.board[7][5] = WHITE_ROOK;
                            possible_position.can_castle_white[0] = false;
                            possible_position.can_castle_white[1] = false;
                            if (!Board::king_is_in_check(possible_position.board, WHITE))
                            {
                                score = max(score, minimax(possible_position, depth - 1, false));
                            }
                        }
                        // castling queenside
                        if (m == 7 && n == 4
                            && position.board[7][1] == 0 && position.board[7][2] == 0 && position.board[7][3] == 0
                            && position.board[7][0] == WHITE_ROOK
                            && !Board::under_control(position.board, 7, 1, BLACK)
                            && !Board::under_control(position.board, 7, 2, BLACK)
                            && !Board::under_control(position.board, 7, 3, BLACK)
                            && position.can_castle_white[0])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[7][2] = WHITE_KING;
                            possible_position.board[7][4] = 0;
                            possible_position.board[7][0] = 0;
                            possible_position.board[7][3] = WHITE_ROOK;
                            possible_position.can_castle_white[0] = false;
                            possible_position.can_castle_white[1] = false;
                            if (!Board::king_is_in_check(possible_position.board, WHITE))
                            {
                                score = max(score, minimax(possible_position, depth - 1, false));
                            }
                        }
                    }
                }
            }
        }
        return score;
    }
    // this is the minimizer's move (choose best move for black)
    else
    {
        Board::reset_en_passant(position, BLACK);
        score = INT_MAX;
        for (int m = 0; m < 8; m++)
        {
            for (int n = 0; n < 8; n++)
            {
                if (position.board[m][n] < 0)
                {

                    if (position.board[m][n] == BLACK_PAWN)
                    {
                        // one move forward by pawn 
                        if (position.board[m + 1][n] == 0)
                        {
                            // if not moving to last rank
                            if (m + 1 != 7)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m + 1][n] = BLACK_PAWN;
                                possible_position.board[m][n] = 0;

                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    score = min(score, minimax(possible_position, depth - 1, true));
                                }
                            }
                            // if moves last rank, then promote to all pieces
                            else
                            {
                                // variable i is possible pieces from black knight to queen
                                for (char i = -5; i <= -2; i++)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[m + 1][n] = i;
                                    possible_position.board[m][n] = 0;

                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                            }
                        }
                        // two moves from starting position by pawn
                        if (m == 1
                            && position.board[m + 1][n] == 0
                            && position.board[m + 2][n] == 0)
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m + 2][n] = BLACK_PAWN;
                            possible_position.board[m][n] = 0;

                            possible_position.pawn_two_squares_black[n] = true;

                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                score = min(score, minimax(possible_position, depth - 1, true));
                            }
                        }
                        // capture to the left 
                        if (n != 0
                            && position.board[m + 1][n - 1] > 0)
                        {
                            if (m + 1 != 7)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m + 1][n - 1] = BLACK_PAWN;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    score = min(score, minimax(possible_position, depth - 1, true));
                                }
                            }
                            // if moves last rank, then promote to all pieces
                            else
                            {
                                // variable i is possible pieces from black knight to queen
                                for (char i = -5; i <= -2; i++)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[m + 1][n - 1] = i;
                                    possible_position.board[m][n] = 0;

                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                            }
                        }
                        // capture to the right
                        if (n != 7
                            && position.board[m + 1][n + 1] > 0)
                        {
                            if (m + 1 != 7)
                            {
                                possible_position = Board::copy_position(position);
                                possible_position.board[m + 1][n + 1] = BLACK_PAWN;
                                possible_position.board[m][n] = 0;
                                if (!Board::king_is_in_check(possible_position.board, BLACK))
                                {
                                    score = min(score, minimax(possible_position, depth - 1, true));
                                }
                            }
                            // if moves last rank, then promote to all pieces
                            else
                            {
                                // variable i is possible pieces from black knight to queen
                                for (char i = -5; i <= -2; i++)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[m + 1][n + 1] = i;
                                    possible_position.board[m][n] = 0;

                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                            }
                        }

                        // en passant to the right
                        if (n != 7 && position.board[m][n + 1] == 1 && position.pawn_two_squares_white[n + 1])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m + 1][n + 1] = BLACK_PAWN;
                            possible_position.board[m][n] = 0;
                            possible_position.board[m][n + 1] = 0;
                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                score = min(score, minimax(possible_position, depth - 1, true));
                            }
                        }
                        
                        // en passant to the left
                        if (n != 0 && position.board[m][n - 1] == 1 && position.pawn_two_squares_white[n - 1])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[m + 1][n - 1] = BLACK_PAWN;
                            possible_position.board[m][n] = 0;
                            possible_position.board[m][n - 1] = 0;
                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                score = min(score, minimax(possible_position, depth - 1, true));
                            }
                        }
                    }

                    if (position.board[m][n] == BLACK_KNIGHT)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int k_i = m + knight_move[k][0];
                            int k_j = n + knight_move[k][1];
                            if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8)
                            {
                                if (Board::under_knight_control(m, n, k_i, k_j)
                                    && position.board[k_i][k_j] >= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[k_i][k_j] = BLACK_KNIGHT;
                                    possible_position.board[m][n] = 0;
                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                            }
                        }
                    }

                    if (position.board[m][n] == BLACK_BISHOP)
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            int path_i = m + bishop_direction[k][0];
                            int path_j = n + bishop_direction[k][1];
                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                            {
                                if (Board::under_bishop_control(position.board, m, n, path_i, path_j)
                                    && position.board[path_i][path_j] >= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[path_i][path_j] = BLACK_BISHOP;
                                    possible_position.board[m][n] = 0;
                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                                // if next square is not empty, we stop
                                if (position.board[path_i][path_j] != 0)
                                {
                                    break;
                                }
                                path_i += bishop_direction[k][0];
                                path_j += bishop_direction[k][1];
                            }
                        }
                    }

                    if (position.board[m][n] == BLACK_ROOK)
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            int path_i = m + rook_direction[k][0];
                            int path_j = n + rook_direction[k][1];
                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                            {
                                if (Board::under_rook_control(position.board, m, n, path_i, path_j)
                                    && position.board[path_i][path_j] >= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[path_i][path_j] = BLACK_ROOK;
                                    possible_position.board[m][n] = 0;
                                    if (m == 0 && n == 0)
                                    {
                                        possible_position.can_castle_black[0] = false;
                                    }
                                    if (m == 0 && n == 7)
                                    {
                                        possible_position.can_castle_black[1] = false;
                                    }
                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                                // if next square is not empty, we stop
                                if (position.board[path_i][path_j] != 0)
                                {
                                    break;
                                }
                                path_i += rook_direction[k][0];
                                path_j += rook_direction[k][1];
                            }
                        }
                    }

                    if (position.board[m][n] == BLACK_QUEEN)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int path_i = m + every_direction[k][0];
                            int path_j = n + every_direction[k][1];
                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
                            {
                                if (Board::under_queen_control(position.board, m, n, path_i, path_j)
                                    && position.board[path_i][path_j] >= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[path_i][path_j] = BLACK_QUEEN;
                                    possible_position.board[m][n] = 0;
                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                                // if next square is not empty, we stop
                                if (position.board[path_i][path_j] != 0)
                                {
                                    break;
                                }
                                path_i += every_direction[k][0];
                                path_j += every_direction[k][1];
                            }
                        }
                    }

                    if (position.board[m][n] == BLACK_KING)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            int k_i = m + every_direction[k][0];
                            int k_j = n + every_direction[k][1];
                            if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8)
                            {
                                if (Board::under_king_control(m, n, k_i, k_j)
                                    && position.board[k_i][k_j] >= 0)
                                {
                                    possible_position = Board::copy_position(position);
                                    possible_position.board[k_i][k_j] = BLACK_KING;
                                    possible_position.board[m][n] = 0;
                                    possible_position.can_castle_black[0] = false;
                                    possible_position.can_castle_black[1] = false;
                                    if (!Board::king_is_in_check(possible_position.board, BLACK))
                                    {
                                        score = min(score, minimax(possible_position, depth - 1, true));
                                    }
                                }
                            }
                        }
                        // castling kingside
                        if (m == 0 && n == 4
                            && position.board[0][5] == 0 && position.board[0][6] == 0
                            && position.board[0][7] == BLACK_ROOK
                            && !Board::under_control(position.board, 0, 4, WHITE)
                            && !Board::under_control(position.board, 0, 5, WHITE)
                            && !Board::under_control(position.board, 0, 6, WHITE)
                            && position.can_castle_black[1])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[0][6] = BLACK_KING;
                            possible_position.board[0][4] = 0;
                            possible_position.board[0][7] = 0;
                            possible_position.board[0][5] = BLACK_ROOK;
                            possible_position.can_castle_black[0] = false;
                            possible_position.can_castle_black[1] = false;
                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                score = min(score, minimax(possible_position, depth - 1, true));
                            }
                        }
                        // castling queenside
                        if (m == 0 && n == 4
                            && position.board[0][1] == 0 && position.board[0][2] == 0 && position.board[0][3] == 0
                            && position.board[0][0] == BLACK_ROOK
                            && !Board::under_control(position.board, 0, 2, WHITE)
                            && !Board::under_control(position.board, 0, 3, WHITE)
                            && !Board::under_control(position.board, 0, 4, WHITE)
                            && position.can_castle_black[0])
                        {
                            possible_position = Board::copy_position(position);
                            possible_position.board[0][2] = BLACK_KING;
                            possible_position.board[0][4] = 0;
                            possible_position.board[0][0] = 0;
                            possible_position.board[0][3] = BLACK_ROOK;
                            possible_position.can_castle_black[0] = false;
                            possible_position.can_castle_black[1] = false;
                            if (!Board::king_is_in_check(possible_position.board, BLACK))
                            {
                                score = min(score, minimax(possible_position, depth - 1, true));
                            }
                        }
                    }
                }
            }
        }
        return score;
    }
}

bool Engine::game_is_over(board_state & position)
{
    if (Board::is_checkmate(position, WHITE))
    {
        return true;
    }
    if (Board::is_checkmate(position, BLACK))
    {
        return true;
    }
    if (Board::is_stalemate(position, WHITE))
    {
        return true;
    }
    if (Board::is_stalemate(position, BLACK))
    {
        return true;
    }
    return false;
}
