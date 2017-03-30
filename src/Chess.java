/*
 * Chess.java by Igor P.
 */

import java.util.*;

public class Chess {

    static Scanner reader = new Scanner(System.in);
    static boolean quit;

    static char[][] board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    static int[][] knightMoves
            = {{-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, {2, 1}};

    public static void main(String args[]) {
        System.out.println("Two-player chess game by Igor P.\n");
        while (!quit) {
            drawBoard();
            getPlayerOneInput();
        }
    }

    private static void drawBoard() {
        System.out.println("    A B C D E F G H");
        System.out.println("    ---------------");

        for (int i = 0; i < 8; i++) {
            System.out.print((8 - i) + " | ");
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != ' ') {
                    System.out.print(board[i][j] + " ");
                }
                else if ((i + j) % 2 == 1) {
                    System.out.print("- ");
                }
                else {
                    System.out.print("  ");
                }
            }
            System.out.println("| " + (8 - i));
        }

        System.out.println("    ---------------");
        System.out.println("    A B C D E F G H\n");
    }

    // Process a players move that he inputs
    private static void getPlayerOneInput() {

        // make a backup copy of the old position, so that if the new move appears to put king in 
        // check we can revert back to original position
        char[][] oldBoard = board;
        boolean movedSuccessfully = false;

        while (!movedSuccessfully) {
            System.out.print("White enter your move: ");
            String move = reader.next();

            if (move.equalsIgnoreCase("quit")) {
                quit = true;
                return;
            }

            if (validSyntax(move)) {
                // if there is no piece letter, then it is a non-ambigous pawn move
                if (move.length() == 2) {
                    // convert chess notation to array location numbers
                    int i = 8 - Character.getNumericValue(move.charAt(1));
                    int j = (int) move.charAt(0) - 97;

                    // one move forward by a pawn
                    if (board[i][j] == ' ' && board[i + 1][j] == 'P') {
                        board[i + 1][j] = ' ';
                        board[i][j] = 'P';
                        movedSuccessfully = true;
                    }
                    // two moves forward from starting position by a pawn
                    else if (i == 4 && board[i][j] == ' ' && board[5][j] == ' ' && board[6][j] == 'P') {
                        board[6][j] = ' ';
                        board[4][j] = 'P';
                        movedSuccessfully = true;
                    }
                    // capture a black piece by a pawn to the left (make sure its the only pawn that can
                    // make that capture and make sure not to go out of bounds of the board array)
                    else if ("pnbrqk".contains(String.valueOf(board[i][j])) && j != 7
                            && ((j == 0 && board[i + 1][j + 1] == 'P') || (board[i + 1][j + 1] == 'P'
                            && board[i + 1][j - 1] != 'P'))) {
                        board[i + 1][j + 1] = ' ';
                        board[i][j] = 'P';
                        movedSuccessfully = true;
                    }
                    // capture a black piece by a pawn to the right (make sure part same as last block)
                    else if ("pnbrqk".contains(String.valueOf(board[i][j])) && j != 0
                            && ((j == 7 && board[i + 1][j - 1] == 'P') || (board[i + 1][j - 1] == 'P'
                            && board[i + 1][j + 1] != 'P'))) {
                        board[i + 1][j - 1] = ' ';
                        board[i][j] = 'P';
                        movedSuccessfully = true;
                    }
                }

                // a specified pawn capture, if there are two possible options
                if (move.length() == 3 && "abcdefgh".contains(String.valueOf(move.charAt(0)))) {
                    // convert chess notation to array location numbers
                    int current_file = (int) move.charAt(0) - 97;
                    int i = 8 - Character.getNumericValue(move.charAt(2));
                    int j = (int) move.charAt(1) - 97;

                    // the two files must be next to each other and target can't be on the side borders
                    if (Math.abs(current_file - j) == 1
                            && "pnbrqk".contains(String.valueOf(board[i][j]))
                            && j != 7 && j != 0
                            && board[i + 1][j - 1] == 'P' && board[i + 1][j + 1] == 'P') {
                        board[i + 1][current_file] = ' ';
                        board[i][j] = 'P';
                        movedSuccessfully = true;
                    }
                }

                // unambiguous rook move or capture
                if (move.length() == 3 && move.charAt(0) == 'r') {
                    int i = 8 - Character.getNumericValue(move.charAt(2));
                    int j = (int) move.charAt(1) - 97;

                    // Candidate rook moves, assign first possible move to these. If then we find
                    // other rooks that can also make move, we change firstRookMove to false and
                    // don't move anything. If we then find more possible moves we change  
                    // canMoveRook to false and don't make rook moves. If we don't find other rook 
                    // moves, we use the candidate moves to make the move in the end.
                    int cr_i = -1;
                    int cr_j = -1;
                    boolean firstRookMove = true;
                    boolean canMoveRook = true;

                    // make sure that target square is not own piece
                    if (!"PNBRQK".contains(String.valueOf(board[i][j]))) {
                        // first find the rook(s)
                        for (int x = 0; x < 8; x++) {
                            for (int y = 0; y < 8; y++) {
                                if (board[x][y] == 'R') {

                                    int r_i = x;
                                    int r_j = y;
                                    // rook and selected square have to be on the same horizontal or 
                                    // vertical line, but not exactly the same square
                                    if ((i == r_i || j == r_j) && !(i == r_i && j == r_j)) {

                                        boolean pathObstructed = false;
                                        // rook and target square are on the same rank
                                        if (i == r_i) {
                                            int min = Math.min(j, r_j);
                                            int max = Math.max(j, r_j);
                                            // check squares between path
                                            for (int k = min + 1; k < max; k++) {
                                                if (board[i][k] != ' ') {
                                                    pathObstructed = true;
                                                }
                                            }
                                        }
                                        // rook and target square are on the same file
                                        if (j == r_j) {
                                            int min = Math.min(i, r_i);
                                            int max = Math.max(i, r_i);

                                            // check squares between path
                                            for (int k = min + 1; k < max; k++) {
                                                if (board[k][j] != ' ') {
                                                    pathObstructed = true;
                                                }
                                            }
                                        }
                                        // if nothing in the way and ...
                                        if (!pathObstructed) {
                                            // this is first found move, then save it and mark it
                                            if (firstRookMove) {
                                                cr_i = r_i;
                                                cr_j = r_j;
                                                firstRookMove = false;

                                            }
                                            // not first found move, then change flag so that no
                                            // rook moves are made anymore
                                            else {
                                                System.out.println("More than one rook can make that move.");
                                                canMoveRook = false;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Now we have gone through the all the rooks and know if move is unique.
                        if (canMoveRook && cr_i != -1 && cr_j != -1) {
                            board[cr_i][cr_j] = ' ';
                            board[i][j] = 'R';
                            movedSuccessfully = true;
                        }
                    }
                }

                // Ambiguous rook move or capture.
                if (move.length() == 4 && move.charAt(0) == 'r') {
                    boolean firstRookMove = true;
                    boolean canMoveRook = true;

                    int i = 8 - Character.getNumericValue(move.charAt(3));
                    int j = (int) move.charAt(2) - 97;

                    int cr_i = -1;
                    int cr_j = -1;

                    // specifier tells which rook to move, either rank or file
                    int s_i = -1;
                    int s_j = -1;

                    if ("abcdefgh".contains(String.valueOf(move.charAt(1)))) {
                        s_j = (int) move.charAt(1) - 97;
                    }
                    else {
                        s_i = 8 - Character.getNumericValue(move.charAt(1));
                    }

                    if (!"PNBRQK".contains(String.valueOf(board[i][j]))) {
                        for (int x = 0; x < 8; x++) {
                            for (int y = 0; y < 8; y++) {
                                if (board[x][y] == 'R') {
                                    int r_i = x;
                                    int r_j = y;

                                    if ((i == r_i || j == r_j) && !(i == r_i && j == r_j)) {

                                        boolean pathObstructed = false;
                                        // rook and target square are on the same rank
                                        if (i == r_i) {
                                            int min = Math.min(j, r_j);
                                            int max = Math.max(j, r_j);
                                            // check squares between path
                                            for (int k = min + 1; k < max; k++) {
                                                if (board[i][k] != ' ') {
                                                    pathObstructed = true;
                                                }
                                            }
                                        }
                                        // rook and target square are on the same file
                                        if (j == r_j) {
                                            int min = Math.min(i, r_i);
                                            int max = Math.max(i, r_i);

                                            // check squares between path
                                            for (int k = min + 1; k < max; k++) {
                                                if (board[k][j] != ' ') {
                                                    pathObstructed = true;
                                                }
                                            }
                                        }
                                        // if nothing in the way and specifier matches, make it
                                        // a candidate move
                                        if (!pathObstructed) {
                                            if (s_i == r_i || s_j == r_j) {
                                                // if it is first found move, then mark it
                                                if (firstRookMove) {
                                                    cr_i = r_i;
                                                    cr_j = r_j;
                                                    firstRookMove = false;

                                                }
                                                // not first found move, then change flag so that no
                                                // rook moves are made anymore
                                                else {
                                                    canMoveRook = false;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        // Now we have gone through the all the rooks and know if move is unique.
                        if (canMoveRook && cr_i != -1 && cr_j != -1) {
                            board[cr_i][cr_j] = ' ';
                            board[i][j] = 'R';
                            movedSuccessfully = true;
                        }
                    }
                }

                // Unambiguous knight move or capture.
                if (move.length() == 3 && move.charAt(0) == 'n') {
                    boolean firstKnightMove = true;
                    boolean canMoveKnight = true;

                    int i = 8 - Character.getNumericValue(move.charAt(2));
                    int j = (int) move.charAt(1) - 97;
                    // candidate knight position
                    int cn_i = -1;
                    int cn_j = -1;


                    // make sure target square isn't occupied by own piece
                    if (!"PNBRQK".contains(String.valueOf(board[i][j]))) {
                        // go through possible squares from where knight could move to square
                        for (int m = 0; m < 8; m++) {
                            int n_i = i + knightMoves[m][0];
                            int n_j = j + knightMoves[m][1];
                            if (n_i >= 0 && n_i < 8 && n_j >= 0 && n_j < 8) {
                                if (board[n_i][n_j] == 'N') {
                                    if (firstKnightMove) {
                                        cn_i = n_i;
                                        cn_j = n_j;
                                        firstKnightMove = false;
                                    }
                                    else {
                                        System.out.println("More than one knight can make that move.");
                                        canMoveKnight = false;
                                    }
                                }
                            }
                        }
                        if (canMoveKnight && cn_i != -1 && cn_j != -1) {
                            board[cn_i][cn_j] = ' ';
                            board[i][j] = 'N';
                            movedSuccessfully = true;
                        }
                    }
                }
            }
        }
    }
    
    // Takes string as an argument and returns a boolean depending if it is correct syntax
    static boolean validSyntax(String m) {

        // valid pawn move if has two characters, first is file and second rank eg. "e4"
        if (m.length() == 2
                && "abcdefgh".contains(String.valueOf(m.charAt(0)))
                && "12345678".contains(String.valueOf(m.charAt(1)))) {
            return true;
        }
        // valid non-ambiguous pawn move if has three letters, where first two file and
        // third one is rank eg. "de5"
        if (m.length() == 3
                && "abcdefgh".contains(String.valueOf(m.charAt(0)))
                && "abcdefgh".contains(String.valueOf(m.charAt(1)))
                && "12345678".contains(String.valueOf(m.charAt(2)))) {
            return true;
        }
        // valid piece move if 3 letters, first piece symbol 2nd destination square: "nf3"
        if (m.length() == 3
                && "nbrqk".contains(String.valueOf(m.charAt(0)))
                && "abcdefgh".contains(String.valueOf(m.charAt(1)))
                && "12345678".contains(String.valueOf(m.charAt(2)))) {
            return true;
        }
        // valid non-ambiguous piece move, if first piece symbol, then starting rank/file
        // and then destination square, eg. "rfh3"  
        if (m.length() == 4
                && "nbrqk".contains(String.valueOf(m.charAt(0)))
                && "abcdefgh12345678".contains(String.valueOf(m.charAt(1)))
                && "abcdefgh".contains(String.valueOf(m.charAt(2)))
                && "12345678".contains(String.valueOf(m.charAt(3)))) {
            return true;
        }
        // otherwise not valid move syntax
        return false;
    }
}
