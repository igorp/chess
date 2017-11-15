Chess
======

This is a command line implementation of chess, playable by two human players.

        A B C D E F G H
        ---------------
    8 | r   b k       r | 8
    7 | p     p B p N p | 7
    6 | n -   -   n   - | 6
    5 | - p - N P   - P | 5
    4 |   -   -   - P - | 4
    3 | -   - P -   -   | 3
    2 | P   P   K       | 2
    1 | q           b   | 1
        ---------------
        A B C D E F G H
    
    White won!

Moves are entered by first specifying the piece ('p' for pawn, 'n' for knight, 'b' for bishop, 'q' for queen and 'k' for king), then the square you wish to move the piece to. For example 'pe4' moves the pawn to e4. If there are more than one of the same piece that can move to one particular square, you can specify the which piece by giving its rank or file as the second character. For instance, 'r4a1' means we wish to move the rook that is on the fourth rank to a1.

Currently working on adding a chess engine using alpha-beta pruning, so that playing alone is more fun.
