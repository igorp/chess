Chess
======

This is a command line implementation of chess, playable by two human players.

        A B C D E F G H
        ---------------
    8 | r   b q k b n r | 8
    7 | p p p p   p p p | 7
    6 |   - n -   -   - | 6
    5 | -   -   n   -   | 5
    4 |   - b - P -   - | 4
    3 | -   -   - N -   | 3
    2 | P P P P   P P P | 2
    1 | R N B Q K B N R | 1
        ---------------
        A B C D E F G H
    
    Black enter your move: 

Moves are entered by first specifying the piece ('p' for pawn, 'n' for knight, 'b' for bishop, 'q' for queen and 'k' for king), then the square you wish to move the piece to. For example 'pe4' moves the pawn to e4. If there are more than one of the same piece that can move to one particular square, you can specify the piece by giving its rank or file as the second character. For instance, 'r4a1' means we wish to move the rook that is on the fourth rank to a1.

The next step is to add a chess engine which uses alpha-beta pruning, so that playing alone is more fun.
