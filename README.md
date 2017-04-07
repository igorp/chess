Chess
======

This is a command line implementation of chess, playable by two human players.

        A B C D E F G H
        ---------------
    8 | r n b q k b n r | 8
    7 | p p p p p p p p | 7
    6 |   -   -   -   - | 6
    5 | -   -   -   -   | 5
    4 |   -   -   -   - | 4
    3 | -   -   -   -   | 3
    2 | P P P P P P P P | 2
    1 | R N B Q K B N R | 1
        ---------------
        A B C D E F G H
    
    White enter your move: 

Moves are entered by first specifying the piece ('p' for pawn, 'n' for knight, 'b' for bishop, 'q' for queen and 'k' for king), then the square you wish to move the piece to. For example 'pe4' moves the pawn to e4. If there are more than one of the same piece that can make a move, you can specify which piece by giving its rank or file as the second character. For instance, 'r4a1' means we wish to move the rook that is on the fourth rank to a1.