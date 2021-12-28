/*
 * Source code for chess.cpp by Igor Podsechin
 *
 * This is a chess engine with a graphical user interface made with the Allegro
 * C++ library. The project is part of the author's Bachelor's Thesis at Tampere
 * University.
 *
 * This file contains the topmost main function that handles drawing the board
 * and calls the rest of the classes.
 * 
 */

#include <iostream>
#include <time.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

#include "engine.h"

using namespace std;

// timer that's used to measure the computer's move time
clock_t start_time;

// functions for handling the GUI
void initialize_game();
void load_images();
void handle_mouse_events();
void draw_screen();
void draw_piece(int i, int j);
bool left_mouse_clicked();
bool right_mouse_clicked();

// frames per second used for setting the frame rate
const float FPS = 60;
// size of a chess board square in pixels
const float SQUARE_SIZE = 80;

// allegro graphics library variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT ev;

ALLEGRO_BITMAP *background_img = NULL;
ALLEGRO_BITMAP *icon_img = NULL;
ALLEGRO_BITMAP *white_pawn_img = NULL;
ALLEGRO_BITMAP *white_knight_img = NULL;
ALLEGRO_BITMAP *white_bishop_img = NULL;
ALLEGRO_BITMAP *white_rook_img = NULL;
ALLEGRO_BITMAP *white_queen_img = NULL;
ALLEGRO_BITMAP *white_king_img = NULL;
ALLEGRO_BITMAP *black_pawn_img = NULL;
ALLEGRO_BITMAP *black_knight_img = NULL;
ALLEGRO_BITMAP *black_bishop_img = NULL;
ALLEGRO_BITMAP *black_rook_img = NULL;
ALLEGRO_BITMAP *black_queen_img = NULL;
ALLEGRO_BITMAP *black_king_img = NULL;

Board board;
Engine engine;

// flag for knowing if a piece is selected and in "focus" with the mouse
bool piece_selected = false;
bool redraw_screen = true;

// this variable tells whose turn it is to move next
side turn = WHITE;

bool game_over = false;

// coordinates of the square that is currently selected
int selected_square_i = -1;
char selected_square_j = -1;

int main()
{
    initialize_game();
    
    while (true)
    {
        al_wait_for_event(event_queue, &ev);

        // set redraw flag to true if timer reaches a tick
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw_screen = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        // if mouse button is released, handle mouse events
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            handle_mouse_events();
        }
        // redraw screen if redraw flag is up
        if (redraw_screen && al_is_event_queue_empty(event_queue))
        {
            redraw_screen = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_screen();
            al_flip_display();
        }
    }
    // clear allegro resources before exiting program
    al_destroy_display(display);
    al_destroy_bitmap(background_img);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    return 0;
}

// Initializes game data and variables.
void initialize_game()
{
    // initialize allegro library
    al_init();
    al_install_mouse();
    al_init_image_addon();

    timer = al_create_timer(1.0 / FPS);
    display = al_create_display(640, 640);
    event_queue = al_create_event_queue();

    load_images();

    al_set_window_title(display, "Chess");
    al_set_display_icon(display, icon_img);

    // add display, timer and mouse to event queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_draw_bitmap(background_img, 0, 0, 0);
    al_start_timer(timer);
    al_flip_display();
}

// Loads image files to bitmap pointers.
void load_images()
{
    background_img = al_load_bitmap("pictures/board.png");
    icon_img = al_load_bitmap("pictures/icon.png");

    white_pawn_img = al_load_bitmap("pictures/white_pawn.png");
    white_knight_img = al_load_bitmap("pictures/white_knight.png");
    white_bishop_img = al_load_bitmap("pictures/white_bishop.png");
    white_rook_img = al_load_bitmap("pictures/white_rook.png");
    white_queen_img = al_load_bitmap("pictures/white_queen.png");
    white_king_img = al_load_bitmap("pictures/white_king.png");
    black_pawn_img = al_load_bitmap("pictures/black_pawn.png");
    black_knight_img = al_load_bitmap("pictures/black_knight.png");
    black_bishop_img = al_load_bitmap("pictures/black_bishop.png");
    black_rook_img = al_load_bitmap("pictures/black_rook.png");
    black_queen_img = al_load_bitmap("pictures/black_queen.png");
    black_king_img = al_load_bitmap("pictures/black_king.png");
}

// Deals with user mouse input.
void handle_mouse_events()
{
    // calculate the coordinates of squares that were clicked
    // ie. scale mouse coordinates from 640 to 8 areas by dividing with 80
    int i = ev.mouse.y / 80;
    int j = ev.mouse.x / 80;

    // if user left-clicks on...
    if (left_mouse_clicked() && !game_over)
    {
        // selected piece itself then deselect it
        if (i == selected_square_i && j == selected_square_j)
        {
            piece_selected = false;
            selected_square_i = -1;
            selected_square_j = -1;
        }
        // own piece, then select it
        else if (board.square_occupied_by_white(i, j))
        {
            piece_selected = true;
            selected_square_i = i;
            selected_square_j = j;
        }
        // legal square whilst a piece is selected, then move it there
        else if (piece_selected)
        {
            bool moved = board.handle_player_move(selected_square_i, selected_square_j, i, j);
            if (moved)
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_screen();
                al_flip_display();

                // check if computer has lost
                if (board.king_is_in_check(BLACK) && board.is_checkmate(BLACK))
                {
                    cout << "Checkmate, white wins!" << endl;
                    game_over = true;
                }
                board.reset_en_passant(BLACK);
                if (!game_over)
                {
                    start_time = clock();
     
                    engine.make_computer_move(board.get_position());
                    cout << "Time taken: " << (float)((int)(float(clock() - start_time))) / 1000 << " seconds" << endl;
    
                    if (board.king_is_in_check(WHITE) && board.is_checkmate(WHITE))
                    {
                        cout << "Checkmate, black wins!" << endl;
                        game_over = true;
                    }
                }
                board.reset_en_passant(WHITE);
            }            
        }
        // square other than own (white) while selected, then unselect
        else if (piece_selected && !board.square_occupied_by_white(i, j))
        {
            piece_selected = false;
        }
    }
    // if player right-clicks anywhere, then unselect piece
    if (right_mouse_clicked())
    {
        piece_selected = false;
    }
}

// Draws the board and pieces onto the window.
void draw_screen()
{
    al_draw_bitmap(background_img, 0, 0, 0);

    // loop through each square on the board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // if square is not empty then draw the appropriate piece
            if (board.square_occupied(i, j))
            {
                draw_piece(i, j);
            }
        }
    }
}

void draw_piece(int i, int j)
{
    ALLEGRO_BITMAP *piece = NULL;

    switch (board.get_piece(i, j))
    {
    case WHITE_PAWN:
        piece = white_pawn_img;
        break;
    case WHITE_KNIGHT:
        piece = white_knight_img;
        break;
    case WHITE_BISHOP:
        piece = white_bishop_img;
        break;
    case WHITE_ROOK:
        piece = white_rook_img;
        break;
    case WHITE_QUEEN:
        piece = white_queen_img;
        break;
    case WHITE_KING:
        piece = white_king_img;
        break;
    case BLACK_PAWN:
        piece = black_pawn_img;
        break;
    case BLACK_KNIGHT:
        piece = black_knight_img;
        break;
    case BLACK_BISHOP:
        piece = black_bishop_img;
        break;
    case BLACK_ROOK:
        piece = black_rook_img;
        break;
    case BLACK_QUEEN:
        piece = black_queen_img;
        break;
    case BLACK_KING:
        piece = black_king_img;
        break;
    default:
        break;
    }
    al_draw_bitmap(piece, j * SQUARE_SIZE, i * SQUARE_SIZE, 0);
}

// Returns true if left mouse button is pressed.
bool left_mouse_clicked()
{
    if (ev.mouse.button & 1)
    {
        return true;
    }
    return false;
}

// Returns true if right mouse button is pressed.
bool right_mouse_clicked()
{
    if (ev.mouse.button & 2)
    {
        return true;
    }
    return false;
}
