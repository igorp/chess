/*
 * board.cpp by Igor Podsechin
 *
 * This class defines the rules of the game eg. how the pieces move, saves
 * the board position and also handles the human player's moves.
 *
 */

#include "board.h" 
#include <string>
#include <iostream>

using namespace std;

// counts have many moves have been played
int move_number = 1;
// string variable used for printing out move information to the console
string move_notation = "";

// Returns true if square given by the parameters is occupied by a white piece.
bool Board::square_occupied_by_white(int i, int j)
{
    if (position.board[i][j] > 0)
    {
        return true;
    }
    return false;
}

// Returns true if square given by the parameters is occupied by a black piece.
bool Board::square_occupied_by_black(int i, int j)
{
    if (position.board[i][j] < 0)
    {
        return true;
    }
    return false;
}

// Returns true if square given by the parameters is occupied by any piece.
bool Board::square_occupied(int i, int j)
{
    if (position.board[i][j] != 0)
    {
        return true;
    }
    return false;
}

// Returns true if the square given by the parameters is occupied by side s's opponent
bool Board::square_occupied_by_opponent(char board[8][8], int i, int j, side s)
{
    if (s == WHITE) 
    {
        if (board[i][j] < 0) 
        {
            return true;
        }
    }
    else 
    {
        if (board[i][j] > 0) 
        {
            return true;
        }
    }
    return false;
}

// Returns true if selected piece can move to the given parameter coordinate.
bool Board::handle_player_move(int start_i, int start_j, int target_i, int target_j)
{
    // first save a copy of the current position to a new board state called possible_position
    board_state possible_position;
    copy_board(position.board, possible_position.board);

    possible_position.can_castle_black[0] = position.can_castle_black[0];
    possible_position.can_castle_black[1] = position.can_castle_black[1];
    possible_position.can_castle_white[0] = position.can_castle_white[0];
    possible_position.can_castle_white[1] = position.can_castle_white[1];

    for (int n = 0; n < 8; n++)
    {
        possible_position.pawn_two_squares_black[n] = position.pawn_two_squares_black[n];
        possible_position.pawn_two_squares_white[n] = position.pawn_two_squares_white[n];
    }

    bool move_is_legal = false;

    promote_pawn = false;
    en_passant_index = -1;

    // check the the legality of the move according to the type of the piece
    switch (position.board[start_i][start_j])
    {
    case WHITE_PAWN:
        if (pawn_move(start_i, start_j, target_i, target_j))
        {
            move_piece(possible_position, start_i, start_j, target_i, target_j);
            
            int direction = 1;

            for (int n = 0; n < 2; n++)
            {
                if (start_i == 3 && start_i - 1 == target_i && start_j == target_j + direction &&
                    position.pawn_two_squares_black[target_j])
                {
                    possible_position.board[target_i + 1][target_j] = 0;
                }
            }
            move_is_legal = true;
        }
        break;
    case WHITE_KNIGHT:
        if (under_knight_control(start_i, start_j, target_i, target_j))
        {
            move_piece(possible_position, start_i, start_j, target_i, target_j);
            move_is_legal = true;
        }
        break;
    case WHITE_BISHOP:
        if (under_bishop_control(position.board, start_i, start_j, target_i, target_j))
        {
            move_piece(possible_position, start_i, start_j, target_i, target_j);
            move_is_legal = true;
        }
        break;
    case WHITE_ROOK:
        if (under_rook_control(position.board, start_i, start_j, target_i, target_j))
        {
            move_piece(possible_position, start_i, start_j, target_i, target_j);

            // if rook moves from its starting square, then castling to that side becomes illegal
            if (start_i == 7 && start_j == 0)
            {
                possible_position.can_castle_white[0] = false;
            }
            if (start_i == 7 && start_j == 7)
            {
                possible_position.can_castle_white[1] = false;
            }
            move_is_legal = true;
        }
        break;
    case WHITE_QUEEN:
        if (under_queen_control(position.board, start_i, start_j, target_i, target_j))
        {
            move_piece(possible_position, start_i, start_j, target_i, target_j);
            move_is_legal = true;
        }
        break;
    case WHITE_KING:
        if (under_king_control(start_i, start_j, target_i, target_j))
        {
            move_piece(possible_position, start_i, start_j, target_i, target_j);
            // after king moves, castling becomes illegal
            possible_position.can_castle_white[0] = false;
            possible_position.can_castle_white[1] = false;
            move_is_legal = true;
        }
        // castling kingside
        if (start_i == 7 && start_j == 4 && target_i == 7 && target_j == 6
            && position.board[7][5] == 0 && position.board[7][6] == 0
            && !under_control(position.board, 7, 4, BLACK)
            && !under_control(position.board, 7, 5, BLACK)
            && !under_control(position.board, 7, 6, BLACK)
            && position.can_castle_white[1])
        {
            possible_position.board[target_i][target_j] = possible_position.board[start_i][start_j];
            possible_position.board[start_i][start_j] = 0;
            possible_position.board[7][7] = 0;
            possible_position.board[7][5] = WHITE_ROOK;
            possible_position.can_castle_white[0] = false;
            possible_position.can_castle_white[1] = false;
            move_is_legal = true;
        }
        // castling queenside
        if (start_i == 7 && start_j == 4 && target_i == 7 && target_j == 2
            && position.board[7][1] == 0 && position.board[7][2] == 0 && position.board[7][3] == 0
            && !under_control(position.board, 7, 2, BLACK)
            && !under_control(position.board, 7, 3, BLACK)
            && !under_control(position.board, 7, 4, BLACK)
            && position.can_castle_white[0])
        {
            possible_position.board[target_i][target_j] = possible_position.board[start_i][start_j];
            possible_position.board[start_i][start_j] = 0;
            possible_position.board[7][0] = 0;
            possible_position.board[7][3] = WHITE_ROOK;
            possible_position.can_castle_white[0] = false;
            possible_position.can_castle_white[1] = false;
            move_is_legal = true;
        }
        break;
    default:
        break;
    }

    if (move_is_legal && !king_is_in_check(possible_position.board, WHITE))
    {        
        copy_board(possible_position.board, position.board);
        position.can_castle_white[0] = possible_position.can_castle_white[0];
        position.can_castle_white[1] = possible_position.can_castle_white[1];
        position.can_castle_black[0] = possible_position.can_castle_black[0];
        position.can_castle_black[1] = possible_position.can_castle_black[1];

        for (int i = 0; i < 8; i++)
        {
            position.pawn_two_squares_white[i] = possible_position.pawn_two_squares_white[i];
            position.pawn_two_squares_black[i] = possible_position.pawn_two_squares_black[i];
        }

        if (en_passant_index != -1)
        {
            position.pawn_two_squares_white[en_passant_index] = true;
        }

        if (promote_pawn)
        {
            char user_input;
            cout << "Promote your pawn. Enter (k)night, (b)ishop, (r)ook or (q)ueen: ";
            cin >> user_input;

            switch (user_input)
            {
            case 'k':
                position.board[target_i][target_j] = WHITE_KNIGHT;
                break;
            case 'b':
                position.board[target_i][target_j] = WHITE_BISHOP;
                break;
            case 'r':
                position.board[target_i][target_j] = WHITE_ROOK;
                break;
            case 'q':
                position.board[target_i][target_j] = WHITE_QUEEN;
                break;
            default:
                position.board[target_i][target_j] = WHITE_QUEEN;
                break;
            }
        }
        cout << move_number++ << ".";

        switch (position.board[target_i][target_j])
        {
        case WHITE_KNIGHT:
            cout << 'N';
            break;
        case WHITE_BISHOP:
            cout << 'B';
            break;
        case WHITE_ROOK:
            cout << 'R';
            break;
        case WHITE_QUEEN:
            cout << 'Q';
            break;
        case WHITE_KING:
            cout << 'K';
            break;
        }
        // print out the move that was made to console
        cout << ((char) (target_j+97) ) << (8-target_i) << " ";
        return true;
    }
    return false;
}

// Returns true if selected pawn can move to the given parameter coordinate.
bool Board::pawn_move(int start_i, int start_j,
    int target_i, int target_j)
{
    char starting_row = WHITE_PAWN_STARTING_ROWN;

    // one square forward
    if (target_i == start_i - 1 && start_j == target_j)
    {
        // check that square in front is not occupied
        if (!square_occupied(target_i, target_j))
        {
            // if the square is on the last row, set promotion flag to true
            if (target_i == 0)
            {
                promote_pawn = true;
            }
            return true;
        }
    }
    // two squares forward from starting row
    if (start_i == starting_row && start_i - 2 == target_i
        && start_j == target_j)
    {
        // check that two squares in front are not occupied
        if (!square_occupied(target_i + 1, target_j) && !square_occupied(target_i, target_j))
        {
            en_passant_index = target_j;
            return true;
        }
    }

    // Loop through captures twice, once to the right and once to the left
    int direction = 1;

    for (int n = 0; n < 2; n++)
    {
        if (n == 1)
        {
            direction = -1;
        }
        // normal capture
        if (start_i - 1 == target_i && target_j + direction == start_j
            && square_occupied_by_black(target_i, target_j))
        {
            // if the square is on the last row, set promotion flag to true
            if (target_i == 0)
            {
                promote_pawn = true;
            }
            return true;
        }

        // en passant capture
        if (start_i == 3 && start_i - 1 == target_i && start_j == target_j + direction &&
            position.pawn_two_squares_black[target_j])
        {
            char possible_board[8][8];
            possible_board[target_i + 1][target_j] = 0;
            return true;
        }
    }
    return false;
}

// Returns true if target square is under the pawn's control
bool Board::under_pawn_control(char board[8][8], int start_i, int start_j, int target_i, int target_j)
{
    int dir = 1;
    if (board[start_i][start_j] < 0)
    {
        dir = -1;
    }
    if (target_i + dir == start_i && start_j == target_j + 1)
    {
        return true;
    }
    if (target_i + dir == start_i && start_j == target_j - 1)
    {
        return true;
    }
    return false;
}

// Returns true if target square is under the knight's control
bool Board::under_knight_control(int start_i, int start_j, int target_i, int target_j)
{
    for (int m = 0; m < 8; m++)
    {
        if (target_i == start_i + knight_move[m][0] && target_j == start_j + knight_move[m][1])
        {
            return true;
        }
    }
    return false;
}

// Returns true if target square is under the bishop's control
bool Board::under_bishop_control(char board[8][8], int start_i, int start_j, int target_i, int target_j)
{
    for (int f = 0; f < 4; f++)
    {
        int path_i = start_i + bishop_direction[f][0];
        int path_j = start_j + bishop_direction[f][1];

        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
        {
            if (path_i == target_i && path_j == target_j)
            {
                return true;
            }
            // if next square is not empty, we stop
            if (board[path_i][path_j] != 0)
            {
                break;
            }
            path_i += bishop_direction[f][0];
            path_j += bishop_direction[f][1];
        }
    }
    return false;
}

// Returns true if target square is under the rook's control
bool Board::under_rook_control(char board[8][8], int start_i, int start_j, int target_i, int target_j)
{
    for (int f = 0; f < 4; f++)
    {
        int path_i = start_i + rook_direction[f][0];
        int path_j = start_j + rook_direction[f][1];

        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
        {
            if (path_i == target_i && path_j == target_j)
            {
                return true;
            }
            // if next square is not empty, we stop
            if (board[path_i][path_j] != 0)
            {
                break;
            }
            path_i += rook_direction[f][0];
            path_j += rook_direction[f][1];
        }
    }
    return false;
}

// Returns true if target square is under the queen's control
bool Board::under_queen_control(char board[8][8], int start_i, int start_j, int target_i, int target_j)
{
    for (int f = 0; f < 8; f++)
    {
        int path_i = start_i + every_direction[f][0];
        int path_j = start_j + every_direction[f][1];

        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8)
        {
            if (path_i == target_i && path_j == target_j)
            {
                return true;
            }
            // if next square is not empty, we stop
            if (board[path_i][path_j] != 0)
            {
                break;
            }
            path_i += every_direction[f][0];
            path_j += every_direction[f][1];
        }
    }
    return false;
}

// Returns true if target square is under the king's control
bool Board::under_king_control(int start_i, int start_j, int target_i, int target_j)
{
    for (int f = 0; f < 8; f++) 
    {
        if (target_i == start_i + every_direction[f][0] && target_j == start_j + every_direction[f][1])
        {
            return true;
        }
    }
    return false;
}

// Returns true if given square is under control by side s
bool Board::under_control(char board[8][8], int i, int j, side s)
{
    char pawn = WHITE_PAWN;
    char knight = WHITE_KNIGHT;
    char bishop = WHITE_BISHOP;
    char rook = WHITE_ROOK;
    char queen = WHITE_QUEEN;
    char king = WHITE_KING;

    if (s == BLACK)
    {
        pawn = BLACK_PAWN;
        knight = BLACK_KNIGHT;
        bishop = BLACK_BISHOP;
        rook = BLACK_ROOK;
        queen = BLACK_QUEEN;
        king = BLACK_KING;
    }
    for (int m = 0; m < 8; m++) 
    {
        for (int n = 0; n < 8; n++) 
        {
            if (board[m][n] == pawn && under_pawn_control(board, m, n, i, j))
            {
                return true;
            }
            if (board[m][n] == knight && under_knight_control(m, n, i, j))
            {
                return true;
            }
            if (board[m][n] == bishop && under_bishop_control(board, m, n, i, j))
            {
                return true;
            }
            if (board[m][n] == rook && under_rook_control(board, m, n, i, j))
            {
                return true;
            }
            if (board[m][n] == queen && under_queen_control(board, m, n, i, j))
            {
                return true;
            }
            if (board[m][n] == king && under_king_control(m, n, i, j))
            {
                return true;
            }
        }
    }
    return false;
}

// Returns true if side s king is in check
bool Board::king_is_in_check(char board[8][8], side s)
{
    char king = WHITE_KING;
    side opponent = BLACK;

    if (s == BLACK)
    {
        king = BLACK_KING;
        opponent = WHITE;
    }
    for (int m = 0; m < 8; m++)
    {
        for (int n = 0; n < 8; n++)
        {
            if (board[m][n] == king)
            {
                if (under_control(board, m, n, opponent))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// Returns true if side s king is in check
bool Board::king_is_in_check(side s)
{
    return king_is_in_check(position.board, s);
}

// If checkmate for player s (eg. s = white, white loses), then function returns true.
// Checkmate takes place when the king is under attack (in check) and there are no legal moves
// that prevent it.
bool Board::is_checkmate(board_state p, side s)
{
    char possible_board[8][8];
    // direction in which pawns move
    int dir = -1;
    // starting rank of pawns
    int startingRank = 6;
    int enPassantRank = 3;
    bool *en_passant = p.pawn_two_squares_black;

    char pawn = WHITE_PAWN;
    char knight = WHITE_KNIGHT;
    char bishop = WHITE_BISHOP;
    char rook = WHITE_ROOK;
    char queen = WHITE_QUEEN;
    char king = WHITE_KING;

    if (s == BLACK) 
    {
        dir = 1;
        startingRank = 1;
        enPassantRank = 4;
        en_passant = p.pawn_two_squares_white;

        pawn = BLACK_PAWN;
        knight = BLACK_KNIGHT;
        bishop = BLACK_BISHOP;
        rook = BLACK_ROOK;
        queen = BLACK_QUEEN;
        king = BLACK_KING;
    }

    for (int m = 0; m < 8; m++) 
    {
        for (int n = 0; n < 8; n++) 
        {
            // king moves
            if (p.board[m][n] == king) 
            {
                for (int f = 0; f < 8; f++) 
                {
                    int k_i = m + every_direction[f][0];
                    int k_j = n + every_direction[f][1];
                    if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8) 
                    {
                        if (square_occupied_by_opponent(p.board, k_i, k_j, s) || p.board[k_i][k_j] == 0) 
                        {
                            copy_board(p.board, possible_board);
                            possible_board[m][n] = 0;
                            possible_board[k_i][k_j] = king;
                            if (!king_is_in_check(possible_board, s)) 
                            {
                                return false;
                            }
                        }
                    }
                }
            }

            // pawn moves
            if (p.board[m][n] == pawn) 
            {
                // one move forward by pawn
                if (p.board[m + dir][n] == 0) 
                {
                    copy_board(p.board, possible_board);
                    possible_board[m][n] = 0;
                    possible_board[m + dir][n] = pawn;
                    if (!king_is_in_check(possible_board, s)) 
                    {
                        return false;
                    }
                }
                // two moves from starting position by pawn
                if (startingRank == m
                    && p.board[m + dir][n] == 0 && p.board[m + 2 * dir][n] == 0)
                {
                    copy_board(p.board, possible_board);
                    possible_board[m][n] = 0;
                    possible_board[m + dir][n] = 0;
                    possible_board[m + 2 * dir][n] = pawn;
                    if (!king_is_in_check(possible_board, s)) 
                    {
                        return false;
                    }
                }
                // capture to the left
                if (n != 0 && square_occupied_by_opponent(p.board, m + dir, n - 1, s))
                {
                    copy_board(p.board, possible_board);
                    possible_board[m][n] = 0;
                    possible_board[m + dir][n - 1] = pawn;
                    if (!king_is_in_check(possible_board, s)) 
                    {
                        return false;
                    }
                }
                // capture to the right
                if (n != 7 && square_occupied_by_opponent(p.board, m + dir, n + 1, s))
                {
                    copy_board(p.board, possible_board);
                    possible_board[m][n] = 0;
                    possible_board[m + dir][n + 1] = pawn;
                    if (!king_is_in_check(possible_board, s)) 
                    {
                        return false;
                    }
                }
                // en passant to the left
                if (n != 0 && m == enPassantRank && en_passant[n - 1])
                {
                    copy_board(p.board, possible_board);
                    possible_board[m][n] = 0;
                    possible_board[m][n - 1] = 0;
                    possible_board[m + dir][n - 1] = pawn;
                    if (!king_is_in_check(possible_board, s)) 
                    {
                        return false;
                    }
                }
                // en passant to the right
                if (n != 7 && m == enPassantRank && en_passant[n + 1])
                {
                    copy_board(p.board, possible_board);
                    possible_board[m][n] = 0;
                    possible_board[m][n + 1] = 0;
                    possible_board[m + dir][n + 1] = pawn;
                    if (!king_is_in_check(possible_board, s)) 
                    {
                        return false;
                    }
                }
            }

            // knight moves
            if (p.board[m][n] == knight)
            {
                for (int f = 0; f < 8; f++) 
                {
                    int n_i = m + knight_move[f][0];
                    int n_j = n + knight_move[f][1];
                    // make sure within bounds
                    if (n_i >= 0 && n_i < 8 && n_j >= 0 && n_j < 8) 
                    {
                        // make sure possible square doesn't contain own pieces
                        if (square_occupied_by_opponent(p.board, n_i, n_j, s) || p.board[n_i][n_j] == 0)
                        {
                            copy_board(p.board, possible_board);
                            possible_board[m][n] = 0;
                            possible_board[n_i][n_j] = knight;
                            if (!king_is_in_check(possible_board, s)) 
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            // rook moves
            if (p.board[m][n] == rook)
            {
                for (int f = 0; f < 4; f++) 
                {
                    int r_i = m + rook_direction[f][0];
                    int r_j = n + rook_direction[f][1];
                    while (r_i >= 0 && r_i < 8 && r_j >= 0 && r_j < 8) 
                    {
                        // check if moving to square (which isn't occupied by a 
                        // friendly piece) removes check on king
                        if (square_occupied_by_opponent(p.board, r_i, r_j, s) || p.board[r_i][r_j] == 0) 
                        {
                            copy_board(p.board, possible_board);
                            possible_board[m][n] = 0;
                            possible_board[r_i][r_j] = rook;
                            if (!king_is_in_check(possible_board, s)) 
                            {
                                return false;
                            }
                        }
                        // if current square is not empty, we stop
                        if (p.board[r_i][r_j] != 0)
                        {
                            break;
                        }
                        r_i += rook_direction[f][0];
                        r_j += rook_direction[f][1];
                    } 
                }
            }
            // bishop moves
            if (p.board[m][n] == bishop)
            {
                for (int f = 0; f < 4; f++) 
                {
                    int b_i = m + bishop_direction[f][0];
                    int b_j = n + bishop_direction[f][1];
                    while (b_i >= 0 && b_i < 8 && b_j >= 0 && b_j < 8)
                    {
                        if (square_occupied_by_opponent(p.board, b_i, b_j, s) || p.board[b_i][b_j] == 0) 
                        {
                            copy_board(p.board, possible_board);
                            possible_board[m][n] = 0;
                            possible_board[b_i][b_j] = bishop;
                            if (!king_is_in_check(possible_board, s)) 
                            {
                                return false;
                            }
                        }
                        // if current square is not empty, we stop
                        if (p.board[b_i][b_j] != 0)
                        {
                            break;
                        }
                        b_i += bishop_direction[f][0];
                        b_j += bishop_direction[f][1];
                    }
                }
            }
            // queen moves
            if (p.board[m][n] == queen)
            {
                for (int f = 0; f < 8; f++) 
                {
                    int q_i = m + every_direction[f][0];
                    int q_j = n + every_direction[f][1];
                    while (q_i >= 0 && q_i < 8 && q_j >= 0 && q_j < 8) 
                    {
                        if (square_occupied_by_opponent(p.board, q_i, q_j, s) || p.board[q_i][q_j] == 0) 
                        {
                            copy_board(p.board, possible_board);
                            possible_board[m][n] = 0;
                            possible_board[q_i][q_j] = queen;
                            if (!king_is_in_check(possible_board, s)) 
                            {
                                return false;
                            }
                        }
                        // if current square is not empty, we stop
                        if (p.board[q_i][q_j] != 0)
                        {
                            break;
                        }
                        q_i += every_direction[f][0];
                        q_j += every_direction[f][1];
                    }
                }
            }
        }
    }
    return true;
}

// Returns true if side s is checkmated.
bool Board::is_checkmate(side s)
{
    return is_checkmate(position, s);
}

// Returns true if side s is stalemated. Stalemate means that there are no legal moves in
// the position.
bool Board::is_stalemate(board_state &p, side s)
{
    // direction in which pawns move
    int dir = -1;
    // starting rank of pawns
    int enPassantRank = 3;
    bool *en_passant = p.pawn_two_squares_black;
    side opponent = BLACK;

    char pawn = WHITE_PAWN;
    char knight = WHITE_KNIGHT;
    char bishop = WHITE_BISHOP;
    char rook = WHITE_ROOK;
    char queen = WHITE_QUEEN;
    char king = WHITE_KING;

    if (s == BLACK)
    {
        dir = 1;
        enPassantRank = 4;
        en_passant = p.pawn_two_squares_white;
        opponent = WHITE;

        pawn = BLACK_PAWN;
        knight = BLACK_KNIGHT;
        bishop = BLACK_BISHOP;
        rook = BLACK_ROOK;
        queen = BLACK_QUEEN;
        king = BLACK_KING;
    }

    for (int m = 0; m < 8; m++) 
    {
        for (int n = 0; n < 8; n++) 
        {
            // king moves
            if (p.board[m][n] == king) 
            {
                for (int f = 0; f < 8; f++) 
                {
                    int k_i = m + every_direction[f][0];
                    int k_j = n + every_direction[f][1];
                    if (k_i >= 0 && k_i < 8 && k_j >= 0 && k_j < 8) 
                    {
                        if (((square_occupied_by_opponent(p.board, k_i, k_j, s) || p.board[k_i][k_j] == 0))
                            && !under_control(p.board, k_i, k_j, opponent)) 
                        {
                            return false;
                        }
                    }
                }
            }
            // pawn moves
            if (p.board[m][n] == pawn) 
            {
                // one move forward by pawn
                if (p.board[m + dir][n] == 0) 
                {
                    return false;
                }
                // captures left, right and en passant moves
                if (n != 0 && square_occupied_by_opponent(p.board, m + dir, n - 1, s)) 
                {
                    return false;
                }
                if (n != 7 && square_occupied_by_opponent(p.board, m + dir, n + 1, s)) 
                {
                    return false;
                }
                if (n != 0 && m == enPassantRank && en_passant[n - 1]) 
                {
                    return false;
                }
                if (n != 7 && m == enPassantRank && en_passant[n + 1]) 
                {
                    return false;
                }
            }
            if (p.board[m][n] == knight) 
            {
                for (int f = 0; f < 8; f++) 
                {
                    int n_i = m + knight_move[f][0];
                    int n_j = n + knight_move[f][1];
                    if (n_i >= 0 && n_i < 8 && n_j >= 0 && n_j < 8) 
                    {
                        if (square_occupied_by_opponent(p.board, n_i, n_j, s) || p.board[n_i][n_j] == 0) 
                        {
                            return false;
                        }
                    }
                }
            }
            if (p.board[m][n] == rook) 
            {
                for (int f = 0; f < 4; f++) 
                {
                    int r_i = m + rook_direction[f][0];
                    int r_j = n + rook_direction[f][1];
                    while (r_i >= 0 && r_i < 8 && r_j >= 0 && r_j < 8) 
                    {
                        // check if moving to square (which isn't occupied by a 
                        // friendly piece) removes check on king
                        if (square_occupied_by_opponent(p.board, r_i, r_j, s) || p.board[r_i][r_j] == 0) 
                        {
                            return false;
                        }
                        // if current square is not empty, we stop
                        if (p.board[r_i][r_j] != 0) 
                        {
                            break;
                        }
                        r_i += rook_direction[f][0];
                        r_j += rook_direction[f][1];
                    }
                }
            }
            // bishop moves
            if (p.board[m][n] == bishop) 
            {
                for (int f = 0; f < 4; f++) 
                {
                    int b_i = m + bishop_direction[f][0];
                    int b_j = n + bishop_direction[f][1];
                    while (b_i >= 0 && b_i < 8 && b_j >= 0 && b_j < 8) 
                    {
                        if (square_occupied_by_opponent(p.board, b_i, b_j, s) || p.board[b_i][b_j] == 0) 
                        {
                            return false;
                        }
                        // if current square is not empty, we stop
                        if (p.board[b_i][b_j] != 0) 
                        {
                            break;
                        }
                        b_i += bishop_direction[f][0];
                        b_j += bishop_direction[f][1];
                    }
                }
            }
            // queen moves
            if (p.board[m][n] == queen)
            {
                for (int f = 0; f < 8; f++)
                {
                    int q_i = m + every_direction[f][0];
                    int q_j = n + every_direction[f][1];
                    while (q_i >= 0 && q_i < 8 && q_j >= 0 && q_j < 8)
                    {
                        if (square_occupied_by_opponent(p.board, q_i, q_j, s) || p.board[q_i][q_j] == 0)
                        {
                            return false;
                        }
                        // if current square is not empty, we stop
                        if (p.board[q_i][q_j] != 0)
                        {
                            break;
                        }
                        q_i += every_direction[f][0];
                        q_j += every_direction[f][1];
                    }
                }
            }
        }
    }
    return true;
}

// Returns the piece located at the given parameter.
char Board::get_piece(int i, int j)
{
    return position.board[i][j];
}

// Moves the piece from start square to destination square.
void Board::move_piece(board_state &pos, int start_i, int start_j, int destination_i, int destination_j)
{
    // copy piece to new square
    pos.board[destination_i][destination_j] = pos.board[start_i][start_j];
    // clear old square
    pos.board[start_i][start_j] = 0;
}
// Copies souce board to destination board.
void Board::copy_board(char source[8][8], char destination[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            destination[i][j] = source[i][j];
        }
    }

}

// Clears the en passant flags in the given position for side s.
void Board::reset_en_passant(board_state &pos, side s)
{
    if (s == WHITE)
    {
        for (int n = 0; n < 8; n++)
        {
            pos.pawn_two_squares_white[n] = false;
        }
    }
    else {
        for (int n = 0; n < 8; n++)
        {
            pos.pawn_two_squares_black[n] = false;
        }
    }
}

// Clears the en passant flags in for side s.
void Board::reset_en_passant(side s)
{
    if (s == WHITE) 
    {
        for (int n = 0; n < 8; n++)
        {
            position.pawn_two_squares_white[n] = 0;
        }
    }
    else {
        for (int n = 0; n < 8; n++)
        {
            position.pawn_two_squares_black[n] = 0;
        }
    }
}

// Passes on a reference of the current position.
board_state& Board::get_position()
{
    return position;
}

// Returns a copy of the position that's given as a parameter.
board_state Board::copy_position(board_state position)
{
    return position;
}
