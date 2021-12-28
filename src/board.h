/*
 * Source code for board.h by Igor Podsechin
 *
 * This file contains the header for the board class.
 *
 */

enum side { WHITE, BLACK };

const char EMPTY = 0;
const char WHITE_PAWN = 1;
const char WHITE_KNIGHT = 2;
const char WHITE_BISHOP = 3;
const char WHITE_ROOK = 4;
const char WHITE_QUEEN = 5;
const char WHITE_KING = 6;
const char BLACK_PAWN = -1;
const char BLACK_KNIGHT = -2;
const char BLACK_BISHOP = -3;
const char BLACK_ROOK = -4;
const char BLACK_QUEEN = -5;
const char BLACK_KING = -6;

const int WHITE_PAWN_STARTING_ROWN = 6;
const int BLACK_PAWN_STARTING_ROWN = 1;

const int knight_move[8][2]= { {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, {2, 1} };
const int bishop_direction[4][2] = { {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };
const int rook_direction[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
const int every_direction[8][2] = { {1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {1, 0}, {-1, 0}, {0, -1} };

// holds the state of the board including castling and en passant statuses
struct board_state
{
    char board[8][8];
    bool pawn_two_squares_black[8];
    bool pawn_two_squares_white[8];
    bool can_castle_white[2];
    bool can_castle_black[2];
};

class Board
{
private:
    board_state position = {
        { {-4, -2, -3, -5, -6, -3, -2, -4},
          {-1, -1, -1, -1, -1, -1, -1, -1},
          { 0,  0,  0,  0,  0,  0,  0,  0},
          { 0,  0,  0,  0,  0,  0,  0,  0},
          { 0,  0,  0,  0,  0,  0,  0,  0},
          { 0,  0,  0,  0,  0,  0,  0,  0},
          { 1,  1,  1,  1,  1,  1,  1,  1},
          { 4,  2,  3,  5,  6,  3,  2,  4} },

        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1},
        {1, 1} 
    };

    int en_passant_index;
    bool promote_pawn;
public:
    // basic chess move functions
    bool square_occupied_by_white(int i, int j);
    bool square_occupied_by_black(int i, int j);
    bool square_occupied(int i, int j);
    static bool square_occupied_by_opponent(char board[8][8], int i, int j, side s);
    bool handle_player_move(int start_i, int start_j, int target_i, int target_j);

    // pawn moves differ from other pieces' moves so much that they need a separate function
    bool pawn_move(int start_i, int start_j, int target_i, int target_j);

    // under_control function and its helper functions check if a certain square
    // is under control by an opponent's piece
    static bool under_control(char board[8][8], int i, int j, side);
    static bool under_pawn_control(char board[8][8], int start_i, int start_j, int target_i, int target_j);
    static bool under_knight_control(int start_i, int start_j, int target_i, int target_j);
    static bool under_bishop_control(char board[8][8], int start_i, int start_j, int target_i, int target_j);
    static bool under_rook_control(char board[8][8], int start_i, int start_j, int target_i, int target_j);
    static bool under_queen_control(char board[8][8], int start_i, int start_j, int target_i, int target_j);
    static bool under_king_control(int start_i, int start_j, int target_i, int target_j);

    static bool king_is_in_check(char board[8][8], side s);
    bool king_is_in_check(side s);
    static bool is_checkmate(board_state p, side s);
    bool is_checkmate(side s);
    static bool is_stalemate(board_state &p, side s);

    char get_piece(int i, int j);
    void move_piece(board_state &pos, int start_i, int start_j, int destination_i, int destination_j);

    static void copy_board(char source[8][8], char destination[8][8]);
    static void reset_en_passant(board_state &pos, side s);
    void reset_en_passant(side s);

    board_state& get_position();
    static board_state copy_position(board_state position);
};
