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
        {'p', ' ', ' ', ' ', ' ', ' ', ' ', 'p'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

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
                } else if ((i + j) % 2 == 1) {
                    System.out.print("- ");
                } else {
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
                    } // two moves forward from starting position by a pawn
                    else if (i == 4 && board[i][j] == ' ' && board[5][j] == ' ' && board[6][j] == 'P') {
                        board[6][j] = ' ';
                        board[4][j] = 'P';
                        movedSuccessfully = true;
                    } // capture a black piece by a pawn to the left (make sure its the only pawn that can
                    // make that capture) and make sure not to go out of bounds of the board array
                    else if ("pnbrqk".contains(String.valueOf(board[i][j])) && j != 7
                            && ((j == 0 && board[i + 1][j + 1] == 'P') || (board[i + 1][j + 1] == 'P' && board[i + 1][j - 1] != 'P'))) {
                        board[i + 1][j + 1] = ' ';
                        board[i][j] = 'P';
                        movedSuccessfully = true;
                    } // capture a black piece by a pawn to the right (make sure part same as last block)
                    else if ("pnbrqk".contains(String.valueOf(board[i][j])) && j != 0
                            && ((j == 7 && board[i + 1][j - 1] == 'P') || (board[i + 1][j - 1] == 'P' && board[i + 1][j + 1] != 'P'))) {
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

                    if (Math.abs(current_file - j) == 1
                            && "pnbrqk".contains(String.valueOf(board[i][j]))
                            && board[i + 1][j - 1] == 'P' && board[i + 1][j + 1] == 'P') {
                        board[i + 1][current_file] = ' ';
                        board[i][j] = 'P';
                        movedSuccessfully = true;
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
        // valid piece move if 3 letters, first piece syrxmbol 2nd destination square: "nf3"
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
