/*
 * Chess.java by Igor P.
 */

import java.util.*;

public class ChessTest {

    public static void main(String args[]) {
        Chess game = new Chess();
        game.play();
    }
}

class Chess {

    static final int WHITE = 0;
    static final int BLACK = 1;

    Scanner reader = new Scanner(System.in);
    // these two array to tell if a certain pawn has just moved two squares, used for en passant
    boolean[] whitePawnTwoSquares = new boolean[8];
    boolean[] blackPawnTwoSquares = new boolean[8];
    // Conditions which determine if a king can castle. First array for white, second for black.
    // First boolean tells if can castle queenside, second boolean tells if can castle kingside.
    boolean[][] canCastle = {{true, true}, {true, true}};
    boolean quit;

    private char[][] board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    private char[][] board2 = {
        {' ', ' ', ' ', 'r', ' ', ' ', ' ', ' '},
        {' ', ' ', 'P', ' ', 'P', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };

    private int[][] knightMove
            = {{-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, {2, 1}};

    private int[][] rookDirection = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    private int[][] bishopDirection = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    private int[][] everyDirection = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {1, 0}, {-1, 0}, {0, -1}};
    int turn = WHITE;

    public void play() {
        System.out.println("Two-player chess game by Igor P.\n");
        while (!quit) {

            turn = WHITE;
            drawBoard();
            getPlayerInput();
//            System.out.println(Arrays.toString(canCastle[0]));
            if (quit) {
                break;
            }
            turn = BLACK;
            drawBoard();
            getPlayerInput();
//            System.out.println(Arrays.toString(canCastle[1]));
        }
    }

    private void drawBoard() {
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
    private void getPlayerInput() {

        // make a backup copy of the old position, so that if the new move appears to put king in 
        // check we can revert back to original position
        //       char[][] oldBoard = board;
        boolean movedSuccessfully = false;
        boolean[] enPassant = blackPawnTwoSquares;
        String player = "White";
        if (turn == BLACK) {
            player = "Black";
            enPassant = whitePawnTwoSquares;
        }

        while (!movedSuccessfully) {

            System.out.print(player + " enter your move: ");
            String move = reader.next();

            if (move.equalsIgnoreCase("quit")) {
                quit = true;
                return;
            }

            // Check syntax, and then depending on piece enter 'pieceMove' methods, which return 
            // true if the piece can be moved and do so.
            if (validSyntax(move)) {
                if (move.length() == 3 && move.charAt(0) == 'p') {
                    movedSuccessfully = unambiguousPawnMove(move);
                }
                else if (move.length() == 4 && move.charAt(0) == 'p') {
                    movedSuccessfully = ambiguousPawnMove(move);
                }
                else if (move.length() == 3 && move.charAt(0) == 'r') {
                    movedSuccessfully = unambiguousRookMove(move);
                }
                else if (move.length() == 4 && move.charAt(0) == 'r') {
                    movedSuccessfully = ambiguousRookMove(move);
                }
                else if (move.length() == 3 && move.charAt(0) == 'n') {
                    movedSuccessfully = unambiguousKnightMove(move);
                }
                else if (move.length() == 4 && move.charAt(0) == 'n') {
                    movedSuccessfully = ambiguousKnightMove(move);
                }
                else if (move.length() == 3 && move.charAt(0) == 'b') {
                    movedSuccessfully = unambiguousBishopMove(move);
                }
                else if (move.length() == 4 && move.charAt(0) == 'b') {
                    movedSuccessfully = ambiguousBishopMove(move);
                }
                else if (move.length() == 3 && move.charAt(0) == 'q') {
                    movedSuccessfully = unambiguousQueenMove(move);
                }
                else if (move.length() == 4 && move.charAt(0) == 'q') {
                    movedSuccessfully = ambiguousQueenMove(move);
                }
                else if (move.length() == 3 && move.charAt(0) == 'k') {
                    movedSuccessfully = kingMove(move);
                }
            }
        }
        checkForPromotion();
        // clear the en passant array
        for (int i = 0; i < enPassant.length; i++) {
            enPassant[i] = false;
        }
    }

    private boolean unambiguousPawnMove(String move) {
        // convert chess notation to array location numbers
        int i = 8 - Character.getNumericValue(move.charAt(2));
        int j = (int) move.charAt(1) - 97;

        // variables for both white and black
        char pawn = 'P';
        int dir = 1;
        int twoMoves = 4;
        boolean[] enPassantOwn = whitePawnTwoSquares;
        boolean[] enPassantOpponent = blackPawnTwoSquares;
        String opponentPieces = "pnbrqk";
        if (turn == BLACK) {
            pawn = 'p';
            dir = -1;
            twoMoves = 3;
            enPassantOwn = blackPawnTwoSquares;
            enPassantOpponent = whitePawnTwoSquares;
            opponentPieces = "PNBRQK";
        }

        // one move forward by a pawn
        if (board[i][j] == ' ' && board[i + dir][j] == pawn) {
            board[i + dir][j] = ' ';
            board[i][j] = pawn;
            return true;
        }
        // two moves forward from starting position by a pawn
        else if (i == twoMoves && board[i][j] == ' ' && board[i + dir][j] == ' ' && board[i + 2 * dir][j] == pawn) {
            board[i + 2 * dir][j] = ' ';
            board[i][j] = pawn;
            enPassantOwn[j] = true;
            return true;
        }
        // capture a piece by a pawn to the left (make sure its the only pawn that can
        // make that capture and make sure not to go out of bounds of the board array)
        else if (opponentPieces.contains(String.valueOf(board[i][j])) && j != 7
                && ((j == 0 && board[i + dir][j + 1] == pawn) || (board[i + dir][j + 1] == pawn
                && board[i + dir][j - 1] != pawn))) {
            board[i + dir][j + 1] = ' ';
            board[i][j] = pawn;
            return true;
        }
        // capture a piece by a pawn to the right (make sure part same as last block)
        else if (opponentPieces.contains(String.valueOf(board[i][j])) && j != 0
                && ((j == 7 && board[i + dir][j - 1] == pawn) || (board[i + dir][j - 1] == pawn
                && board[i + dir][j + 1] != pawn))) {
            board[i + dir][j - 1] = ' ';
            board[i][j] = pawn;
            return true;
        }
        // en passant capture to the left
        else if (board[i][j] == ' ' && j != 7 && enPassantOpponent[j]
                && ((j == 0 && board[i + dir][j + 1] == pawn) || (board[i + dir][j + 1] == pawn
                && board[i + dir][j - 1] != pawn))) {
            board[i + dir][j + 1] = ' ';
            board[i + dir][j] = ' ';
            board[i][j] = pawn;
            return true;
        }

        // en passant capture to the right
        else if (board[i][j] == ' ' && j != 7 && enPassantOpponent[j]
                && ((j == 0 && board[i + dir][j - 1] == pawn) || (board[i + dir][j - 1] == pawn
                && board[i + dir][j + 1] != pawn))) {
            board[i + dir][j - 1] = ' ';
            board[i + dir][j] = ' ';
            board[i][j] = pawn;
            return true;
        }
        return false;
    }

    private boolean ambiguousPawnMove(String move) {
        // convert chess notation to array location numbers
        int current_file = (int) move.charAt(1) - 97;
        int i = 8 - Character.getNumericValue(move.charAt(3));
        int j = (int) move.charAt(2) - 97;

        // variables for both white and black
        char pawn = 'P';
        int dir = 1;
        boolean[] enPassantOpponent = blackPawnTwoSquares;
        String opponentPieces = "pnbrqk";
        if (turn == BLACK) {
            pawn = 'p';
            dir = -1;
            enPassantOpponent = whitePawnTwoSquares;
            opponentPieces = "PNBRQK";
        }

        // the two files must be next to each other and target can't be on the side borders
        if (Math.abs(current_file - j) == 1
                && opponentPieces.contains(String.valueOf(board[i][j]))
                && j != 7 && j != 0
                && board[i + dir][j - 1] == pawn && board[i + dir][j + 1] == pawn) {
            board[i + dir][current_file] = ' ';
            board[i][j] = pawn;
            return true;
        }

        // en passant capture
        if (Math.abs(current_file - j) == 1
                && board[i][j] == ' '
                && j != 7 && j != 0
                && enPassantOpponent[j]
                && board[i + dir][j - 1] == pawn && board[i + dir][j + 1] == pawn) {
            board[i + dir][current_file] = ' ';
            board[i + dir][j] = ' ';
            board[i][j] = pawn;
            return true;
        }
        return false;
    }

    private boolean unambiguousRookMove(String move) {
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

        // variables for both white and black
        char rook = 'R';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            rook = 'r';
            opponentPieces = "pnbrqk";
        }

        // make sure that target square is not own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            // first find the rook(s)
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == rook) {

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
                board[i][j] = rook;

                updateRookCastlingFlags(cr_i, cr_j);
                return true;
            }
        }
        return false;
    }

    private boolean ambiguousRookMove(String move) {
        boolean firstRookMove = true;
        boolean canMoveRook = true;

        int i = 8 - Character.getNumericValue(move.charAt(3));
        int j = (int) move.charAt(2) - 97;

        int cr_i = -1;
        int cr_j = -1;

        // specifier tells which rook to move, either rank or file
        int s_i = -1;
        int s_j = -1;

        char rook = 'R';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            rook = 'r';
            opponentPieces = "pnbrqk";
        }

        if ("abcdefgh".contains(String.valueOf(move.charAt(1)))) {
            s_j = (int) move.charAt(1) - 97;
        }
        else {
            s_i = 8 - Character.getNumericValue(move.charAt(1));
        }

        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == rook) {
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
                board[i][j] = rook;

                updateRookCastlingFlags(cr_i, cr_j);
                return true;
            }
        }
        return false;
    }

    private boolean unambiguousKnightMove(String move) {
        boolean firstKnightMove = true;
        boolean canMoveKnight = true;

        int i = 8 - Character.getNumericValue(move.charAt(2));
        int j = (int) move.charAt(1) - 97;
        // candidate knight position
        int cn_i = -1;
        int cn_j = -1;

        char knight = 'N';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            knight = 'n';
            opponentPieces = "pnbrqk";
        }

        // make sure target square isn't occupied by own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            // go through possible squares from where knight could move to square
            for (int m = 0; m < 8; m++) {
                int n_i = i + knightMove[m][0];
                int n_j = j + knightMove[m][1];
                if (n_i >= 0 && n_i < 8 && n_j >= 0 && n_j < 8) {
                    if (board[n_i][n_j] == knight) {
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
                board[i][j] = knight;
                return true;
            }

        }
        return false;
    }

    private boolean ambiguousKnightMove(String move) {
        boolean firstKnightMove = true;
        boolean canMoveKnight = true;
        int i = 8 - Character.getNumericValue(move.charAt(3));
        int j = (int) move.charAt(2) - 97;

        int cn_i = -1;
        int cn_j = -1;

        // specifier tells which knight to move, either rank or file
        int s_i = -1;
        int s_j = -1;

        // variables for both white and black
        char knight = 'N';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            knight = 'n';
            opponentPieces = "pnbrqk";
        }

        if ("abcdefgh".contains(String.valueOf(move.charAt(1)))) {
            s_j = (int) move.charAt(1) - 97;
        }
        else {
            s_i = 8 - Character.getNumericValue(move.charAt(1));
        }
        // make sure target square isn't occupied by own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            // go through possible squares from where knight could move to square
            for (int m = 0; m < 8; m++) {
                int n_i = i + knightMove[m][0];
                int n_j = j + knightMove[m][1];
                if (n_i >= 0 && n_i < 8 && n_j >= 0 && n_j < 8) {
                    if (board[n_i][n_j] == knight) {
                        if (s_i == n_i || s_j == n_j) {
                            if (firstKnightMove) {
                                cn_i = n_i;
                                cn_j = n_j;
                                firstKnightMove = false;
                            }
                            else {
                                canMoveKnight = false;
                            }
                        }
                    }
                }
            }
            if (canMoveKnight && cn_i != -1 && cn_j != -1) {
                board[cn_i][cn_j] = ' ';
                board[i][j] = knight;
                return true;
            }
        }
        return false;
    }

    private boolean unambiguousBishopMove(String move) {
        boolean firstMove = true;
        boolean canMove = true;

        int i = 8 - Character.getNumericValue(move.charAt(2));
        int j = (int) move.charAt(1) - 97;
        // candidate bishop position
        int c_i = -1;
        int c_j = -1;

        // variables for both white and black
        char bishop = 'B';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            bishop = 'b';
            opponentPieces = "pnbrqk";
        }

        // make sure target square isn't occupied by own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == bishop) {
                        int b_i = x;
                        int b_j = y;

                        // go in all four directions
                        for (int f = 0; f < 4; f++) {
                            int path_i = x + bishopDirection[f][0];
                            int path_j = y + bishopDirection[f][1];

                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                                if (path_i == i && path_j == j) {
                                    if (firstMove) {
                                        c_i = b_i;
                                        c_j = b_j;
                                        firstMove = false;
                                    }
                                    else {
                                        System.out.println("More than one bishop can make that move.");
                                        canMove = false;
                                    }
                                }
                                // if next square is not empty, we stop
                                if (board[path_i][path_j] != ' ') {
                                    break;
                                }
                                path_i += bishopDirection[f][0];
                                path_j += bishopDirection[f][1];
                            }
                        }
                    }
                }
            }
            if (canMove && c_i != -1 && c_j != -1) {
                board[c_i][c_j] = ' ';
                board[i][j] = bishop;
                return true;
            }
        }
        return false;
    }

    private boolean ambiguousBishopMove(String move) {
        boolean firstMove = true;
        boolean canMove = true;

        int i = 8 - Character.getNumericValue(move.charAt(3));
        int j = (int) move.charAt(2) - 97;
        // candidate bishop position
        int c_i = -1;
        int c_j = -1;

        // specifier tells which bishop to move, either rank or file
        int s_i = -1;
        int s_j = -1;

        // variables for both white and black
        char bishop = 'B';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            bishop = 'b';
            opponentPieces = "pnbrqk";
        }

        if ("abcdefgh".contains(String.valueOf(move.charAt(1)))) {
            s_j = (int) move.charAt(1) - 97;
        }
        else {
            s_i = 8 - Character.getNumericValue(move.charAt(1));
        }

        // make sure target square isn't occupied by own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == bishop) {
                        int q_i = x;
                        int q_j = y;

                        // go in all four directions
                        for (int f = 0; f < 4; f++) {
                            int path_i = x + bishopDirection[f][0];
                            int path_j = y + bishopDirection[f][1];

                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                                if (path_i == i && path_j == j) {
                                    if (s_i == q_i || s_j == q_j) {
                                        if (firstMove) {
                                            c_i = q_i;
                                            c_j = q_j;
                                            firstMove = false;
                                        }
                                        else {
                                            canMove = false;
                                        }
                                    }
                                }
                                // if next square is not empty, we stop
                                if (board[path_i][path_j] != ' ') {
                                    break;
                                }
                                path_i += bishopDirection[f][0];
                                path_j += bishopDirection[f][1];
                            }
                        }
                    }
                }
            }
            if (canMove && c_i != -1 && c_j != -1) {
                board[c_i][c_j] = ' ';
                board[i][j] = bishop;
                return true;
            }
        }
        return false;
    }

    private boolean unambiguousQueenMove(String move) {
        boolean firstMove = true;
        boolean canMove = true;

        int i = 8 - Character.getNumericValue(move.charAt(2));
        int j = (int) move.charAt(1) - 97;
        // candidate queen position
        int c_i = -1;
        int c_j = -1;

        // variables for both white and black
        char queen = 'Q';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            queen = 'q';
            opponentPieces = "pnbrqk";
        }

        // make sure target square isn't occupied by own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == queen) {
                        int q_i = x;
                        int q_j = y;

                        // go in all eight directions
                        for (int f = 0; f < 8; f++) {
                            int path_i = x + everyDirection[f][0];
                            int path_j = y + everyDirection[f][1];

                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                                if (path_i == i && path_j == j) {
                                    if (firstMove) {
                                        c_i = q_i;
                                        c_j = q_j;
                                        firstMove = false;
                                    }
                                    else {
                                        System.out.println("More than one queen can make that move.");
                                        canMove = false;
                                    }
                                }
                                // if next square is not empty, we stop
                                if (board[path_i][path_j] != ' ') {
                                    break;
                                }
                                path_i += everyDirection[f][0];
                                path_j += everyDirection[f][1];
                            }
                        }
                    }
                }
            }
            if (canMove && c_i != -1 && c_j != -1) {
                board[c_i][c_j] = ' ';
                board[i][j] = queen;
                return true;
            }
        }
        return false;
    }

    private boolean ambiguousQueenMove(String move) {
        boolean firstMove = true;
        boolean canMove = true;

        int i = 8 - Character.getNumericValue(move.charAt(3));
        int j = (int) move.charAt(2) - 97;
        // candidate queen position
        int c_i = -1;
        int c_j = -1;

        // specifier tells which queen to move, either rank or file
        int s_i = -1;
        int s_j = -1;

        // variables for both white and black
        char queen = 'Q';
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            queen = 'q';
            opponentPieces = "pnbrqk";
        }

        if ("abcdefgh".contains(String.valueOf(move.charAt(1)))) {
            s_j = (int) move.charAt(1) - 97;
        }
        else {
            s_i = 8 - Character.getNumericValue(move.charAt(1));
        }

        // make sure target square isn't occupied by own piece
        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == queen) {
                        int b_i = x;
                        int b_j = y;

                        // go in all four directions
                        for (int f = 0; f < 8; f++) {
                            int path_i = x + everyDirection[f][0];
                            int path_j = y + everyDirection[f][1];

                            while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                                if (path_i == i && path_j == j) {
                                    if (s_i == b_i || s_j == b_j) {
                                        if (firstMove) {
                                            c_i = b_i;
                                            c_j = b_j;
                                            firstMove = false;
                                        }
                                        else {
                                            canMove = false;
                                        }
                                    }
                                }
                                // if next square is not empty, we stop
                                if (board[path_i][path_j] != ' ') {
                                    break;
                                }
                                path_i += everyDirection[f][0];
                                path_j += everyDirection[f][1];
                            }
                        }
                    }
                }
            }
            if (canMove && c_i != -1 && c_j != -1) {
                board[c_i][c_j] = ' ';
                board[i][j] = queen;
                return true;
            }
        }
        return false;
    }

    private boolean kingMove(String move) {
        int i = 8 - Character.getNumericValue(move.charAt(2));
        int j = (int) move.charAt(1) - 97;

        // variables for both white and black, r is rank on which the king is
        char king = 'K';
        char rook = 'R';
        int r = 7;
        String opponentPieces = "PNBRQK";
        if (turn == BLACK) {
            king = 'k';
            rook = 'r';
            r = 0;
            opponentPieces = "pnbrqk";
        }

        if (!opponentPieces.contains(String.valueOf(board[i][j]))) {
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[x][y] == king) {
                        int k_i = x;
                        int k_j = y;

                        // if king is on starting square it may want to castle
                        if (k_i == r && k_j == 4) {
                            // queenside castle can be made if conditions are met
                            if (i == r && j == 2 && canCastle[turn][0]
                                    && board[r][1] == ' ' && board[r][2] == ' ' && board[r][3] == ' ') {
                                board[r][0] = ' ';
                                board[r][4] = ' ';
                                board[r][2] = king;
                                board[r][3] = rook;
                                updateKingCastlingFlags(k_i, k_j);
                                return true;
                            }
                            // kingside castle
                            if (i == r && j == 6 && canCastle[turn][1]
                                    && board[r][5] == ' ' && board[r][6] == ' ') {
                                board[r][7] = ' ';
                                board[r][4] = ' ';
                                board[r][5] = rook;
                                board[r][6] = king;
                                updateKingCastlingFlags(k_i, k_j);
                                return true;
                            }
                        }

                        for (int f = 0; f < 8; f++) {
                            int path_i = x + everyDirection[f][0];
                            int path_j = y + everyDirection[f][1];

                            if (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                                if (path_i == i && path_j == j && !underControl(path_i, path_j)) {
                                    board[k_i][k_j] = ' ';
                                    board[i][j] = king;
                                    // update castling rights
                                    updateKingCastlingFlags(k_i, k_j);
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    // Returns true if square is under control by opponents pieces
    private boolean underControl(int i, int j) {

        // variables for both white and black
        int dir = 1;
        char pawn = 'p';
        char knight = 'n';
        char bishop = 'b';
        char rook = 'r';
        char queen = 'q';
        char king = 'k';
        if (turn == BLACK) {
            dir = -1;
            pawn = 'P';
            knight = 'N';
            bishop = 'B';
            rook = 'R';
            queen = 'Q';
            king = 'K';
        }

        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // under attack by a pawn
                if (board[x][y] == pawn) {
                    if ((x + dir == i && y + 1 == j) || (x + dir == i && y - 1 == j)) {
                        return true;
                    }
                }
                // under attack by a knight
                if (board[x][y] == knight) {
                    for (int m = 0; m < knightMove.length; m++) {
                        if (x + knightMove[m][0] == i && y + knightMove[m][1] == j) {
                            return true;
                        }
                    }
                }
                // under attack by rook
                if (board[x][y] == rook) {
                    // go in all 4 directions and see if we hit the square
                    for (int m = 0; m < rookDirection.length; m++) {
                        int path_i = x + rookDirection[m][0];
                        int path_j = y + rookDirection[m][1];
                        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                            if (path_i == i && path_j == j) {
                                return true;
                            }
                            // if next square is not empty, we stop
                            if (board[path_i][path_j] != ' ') {
                                break;
                            }
                            path_i += rookDirection[m][0];
                            path_j += rookDirection[m][1];
                        }
                    }
                }

                // under attack by bishop
                if (board[x][y] == bishop) {
                    // go in all 4 directions and see if we hit the square
                    for (int m = 0; m < bishopDirection.length; m++) {
                        int path_i = x + bishopDirection[m][0];
                        int path_j = y + bishopDirection[m][1];
                        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                            if (path_i == i && path_j == j) {
                                return true;
                            }
                            // if next square is not empty, we stop
                            if (board[path_i][path_j] != ' ') {
                                break;
                            }
                            path_i += bishopDirection[m][0];
                            path_j += bishopDirection[m][1];
                        }
                    }
                }

                // under attack by queen
                if (board[x][y] == queen) {
                    // go in all 8 directions and see if we hit the square
                    for (int m = 0; m < everyDirection.length; m++) {
                        int path_i = x + everyDirection[m][0];
                        int path_j = y + everyDirection[m][1];
                        while (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {

                            if (path_i == i && path_j == j) {
                                return true;
                            }
                            // if next square is not empty, we stop
                            if (board[path_i][path_j] != ' ') {
                                break;
                            }
                            path_i += everyDirection[m][0];
                            path_j += everyDirection[m][1];
                        }
                    }
                }

                // under attack by king
                if (board[x][y] == king) {
                    // go in all 4 directions and see if we hit the square
                    for (int m = 0; m < everyDirection.length; m++) {
                        int path_i = x + everyDirection[m][0];
                        int path_j = y + everyDirection[m][1];
                        if (path_i >= 0 && path_i < 8 && path_j >= 0 && path_j < 8) {
                            if (path_i == i && path_j == j) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    // updates the canCastle array depending on which rook has moved from it's starting square
    private void updateRookCastlingFlags(int i, int j) {
        // rook's rank is 1st or eight, depending who's turn it is
        int r = 7;
        if (turn == BLACK) {
            r = 0;
        }

        // queenside rook has moved
        if (canCastle[turn][0] && i == r && j == 0) {
            canCastle[turn][0] = false;
        }
        // kingside rook has moved
        if (canCastle[turn][1] && i == r && j == 7) {
            canCastle[turn][1] = false;
        }
    }

    // updates the canCastle array if king moves from his starting square
    private void updateKingCastlingFlags(int i, int j) {
        int r = 7;
        if (turn == BLACK) {
            r = 0;
        }

        // if king king has moved, both sides castle is prohibited
        if (i == r && j == 4) {
            canCastle[turn][0] = false;
            canCastle[turn][1] = false;
        }
    }

    // if a pawn has reached the last rank, we promote it
    private void checkForPromotion() {
        int i = 0;
        int dir = 1;
        char pawn = 'P';
        char knight = 'N';
        char bishop = 'B';
        char rook = 'R';
        char queen = 'Q';

        if (turn == BLACK) {
            i = 7;
            dir = -1;
            pawn = 'p';
            knight = 'n';
            bishop = 'b';
            rook = 'r';
            queen = 'q';
        }

        int j = -1;

        // search for any pawns on the last rank
        for (int f = 0; f < 8; f++) {
            if (board[i][f] == pawn) {
                j = f;
            }
        }

        // if such a pawn is found, ask the player to promote it
        if (j != -1) {
            String piece = "";
            while (!(piece.equals("q") || piece.equals("r") || piece.equals("b") || piece.equals("n"))) {
                System.out.print("Promote pawn to: ");
                piece = reader.next();
            }
            switch (piece) {
                case "q":
                    board[i + dir][j] = ' ';
                    board[i][j] = queen;
                    break;
                case "r":
                    board[i + dir][j] = ' ';
                    board[i][j] = rook;
                    break;
                case "b":
                    board[i + dir][j] = ' ';
                    board[i][j] = bishop;
                    break;

                case "n":
                    board[i + dir][j] = ' ';
                    board[i][j] = knight;
                    break;
                default:
                    System.out.println("Error: unknown piece");
                    break;
            }
        }
    }

    // Takes string as an argument and returns a boolean depending if it is correct syntax
    private boolean validSyntax(String m) {
        // valid piece move if 3 letters, first piece symbol 2nd destination square: "nf3"
        if (m.length() == 3
                && "pnbrqk".contains(String.valueOf(m.charAt(0)))
                && "abcdefgh".contains(String.valueOf(m.charAt(1)))
                && "12345678".contains(String.valueOf(m.charAt(2)))) {
            return true;
        }
        // valid non-ambiguous piece move, if first piece symbol, then starting rank/file
        // and then destination square, eg. "rfh3"  
        if (m.length() == 4
                && "pnbrqk".contains(String.valueOf(m.charAt(0)))
                && "abcdefgh12345678".contains(String.valueOf(m.charAt(1)))
                && "abcdefgh".contains(String.valueOf(m.charAt(2)))
                && "12345678".contains(String.valueOf(m.charAt(3)))) {
            return true;
        }
        // otherwise not valid move syntax
        return false;
    }
}
