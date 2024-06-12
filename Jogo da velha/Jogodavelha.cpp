#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#define Computador 1 
#define HUMANO 2 
#define SIDE 3 
#define ComputadorMOVE 'O' 
#define PlayerMove 'X' 

enum GameState { MENU, PLAYING, PAUSED, EXIT };
enum Dificuldade { FACIL, MEDIO, DIFICIL };

struct Move { 
	int row, col; 
}; 

char player = 'x', oponente = 'o'; 
enum Dificuldade dificuldade = FACIL;

bool MovesLeft(char (*board)[3]) 
{ 
	for (int i = 0; i < 3; i++) 
		for (int j = 0; j < 3; j++) 
			if (board[i][j] == '_') 
				return true; 
	return false; 
} 

int avalie(char (*b)[3]) 
{ 
	for (int row = 0; row < 3; row++) { 
		if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) { 
			if (b[row][0] == player) 
				return +10; 
			else if (b[row][0] == oponente) 
				return -10; 
		} 
	} 

	for (int col = 0; col < 3; col++) { 
		if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) { 
			if (b[0][col] == player) 
				return +10; 
			else if (b[0][col] == oponente) 
				return -10; 
		} 
	} 

	if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) { 
		if (b[0][0] == player) 
			return +10; 
		else if (b[0][0] == oponente) 
			return -10; 
	} 

	if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) { 
		if (b[0][2] == player) 
			return +10; 
		else if (b[0][2] == oponente) 
			return -10; 
	} 

	return 0; 
} 

int minimax(char (*board)[3], int profundidade, bool isMax) 
{ 
	int score = avalie(board); 

	if (score == 10) 
		return score; 

	if (score == -10) 
		return score; 

	if (MovesLeft(board) == false) 
		return 0; 

	if (isMax) { 
		int best = -1000; 

		for (int i = 0; i < 3; i++) { 
			for (int j = 0; j < 3; j++) { 
				if (board[i][j] == '_') { 
					board[i][j] = player; 
					int val = minimax(board, profundidade + 1, !isMax); 
					if (val > best) { 
						best = val; 
					} 
					board[i][j] = '_'; 
				} 
			} 
		} 
		return best; 
	} 
	else { 
		int best = 1000; 

		for (int i = 0; i < 3; i++) { 
			for (int j = 0; j < 3; j++) { 
				if (board[i][j] == '_') { 
					board[i][j] = oponente; 
					int val = minimax(board, profundidade + 1, !isMax); 
					if (val < best) { 
						best = val; 
					} 
					board[i][j] = '_'; 
				} 
			} 
		} 
		return best; 
	} 
} 

int minimaxDificuldade(char (*board)[3], int profundidade, bool isMax, enum Dificuldade dificuldade) 
{ 
	if (dificuldade == FACIL && profundidade >= 1) return 0; 
	if (dificuldade == MEDIO && profundidade >= 2) return 0; 
	return minimax(board, profundidade, isMax); 
} 

struct Move FindMelhorMovimento(char (*board)[3]) 
{ 
	int bestVal = -1000; 
	struct Move MelhorMove; 
	MelhorMove.row = -1; 
	MelhorMove.col = -1; 

	for (int i = 0; i < 3; i++) { 
		for (int j = 0; j < 3; j++) { 
			if (board[i][j] == '_') { 
				board[i][j] = player; 
				int moveVal = minimaxDificuldade(board, 0, false, dificuldade); 
				board[i][j] = '_'; 

				if (moveVal > bestVal) { 
					MelhorMove.row = i; 
					MelhorMove.col = j; 
					bestVal = moveVal; 
				} 
			} 
		} 
	} 

	return MelhorMove; 
} 

void showBoard(char (*board)[SIDE]) 
{ 
	printf("\n\n"); 
	printf("\t\t\t %c | %c | %c \n", board[0][0], 
		board[0][1], board[0][2]); 
	printf("\t\t\t--------------\n"); 
	printf("\t\t\t %c | %c | %c \n", board[1][0], 
		board[1][1], board[1][2]); 
	printf("\t\t\t--------------\n"); 
	printf("\t\t\t %c | %c | %c \n\n", board[2][0], 
		board[2][1], board[2][2]); 
} 

void Instrucoes() 
{ 
	printf("\t\t\t Jogo da Velha\n\n"); 
	printf("Escolha um numero de 1 a 9 abaixo "
		"e jogue.\n\n"); 

	printf("\t\t\t 1 | 2 | 3 \n"); 
	printf("\t\t\t--------------\n"); 
	printf("\t\t\t 4 | 5 | 6 \n"); 
	printf("\t\t\t--------------\n"); 
	printf("\t\t\t 7 | 8 | 9 \n\n"); 

	printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n\n"); 
} 

void inicializar(char (*board)[SIDE], int *moves) 
{ 
	srand(time(NULL)); 

	for (int i = 0; i < SIDE; i++) { 
		for (int j = 0; j < SIDE; j++) 
			board[i][j] = ' '; 
	} 

	for (int i = 0; i < SIDE * SIDE; i++) 
		moves[i] = i; 

	for (int i = 0; i < SIDE * SIDE; i++) { 
		int randIndex = rand() % (SIDE * SIDE); 
		int temp = moves[i]; 
		moves[i] = moves[randIndex]; 
		moves[randIndex] = temp; 
	} 
} 

void Ganhador(int VezTurno, int score) 
{ 
	if (VezTurno == Computador) 
		printf("Computador Ganhou :("); 
	else
		printf("Jogador ganhou com pontuacao: %d\n", score); 
} 

int rowCrossed(char (*board)[SIDE]) 
{ 
	for (int i = 0; i < SIDE; i++) { 
		if (board[i][0] == board[i][1] 
			&& board[i][1] == board[i][2] 
			&& board[i][0] != ' ') 
			return 1; 
	} 
	return 0; 
} 

int columnCrossed(char (*board)[SIDE]) 
{ 
	for (int i = 0; i < SIDE; i++) { 
		if (board[0][i] == board[1][i] 
			&& board[1][i] == board[2][i] 
			&& board[0][i] != ' ') 
			return 1; 
	} 
	return 0; 
} 

int diagonalCrossed(char (*board)[SIDE]) 
{ 
	if ((board[0][0] == board[1][1] 
		&& board[1][1] == board[2][2] 
		&& board[0][0] != ' ') 
		|| (board[0][2] == board[1][1] 
			&& board[1][1] == board[2][0] 
			&& board[0][2] != ' ')) 
		return 1; 

	return 0; 
} 

int gameOver(char (*board)[SIDE]) 
{ 
	return (rowCrossed(board) || columnCrossed(board) 
			|| diagonalCrossed(board)); 
} 

void playJogodaVelha(int VezTurno) 
{ 
	char board[SIDE][SIDE]; 
	int moves[SIDE * SIDE]; 
	int score = 9999; // pontuacao inicial

	inicializar(board, moves); 

	Instrucoes(); 

	int moveIndex = 0, x, y; 

	clock_t start_time = clock(); // Inicio do Cronometro

	while (!gameOver(board) && moveIndex != SIDE * SIDE) { 
		clock_t current_time = clock();
		double elapsed_time = ((double) (current_time - start_time)) / CLOCKS_PER_SEC;

		// Diminui 50 pts a cada 0,2 seg
		if (((int)(elapsed_time * 5)) % 1 == 0) {
			score = 9999 - (int)(elapsed_time / 0.2) * 50;
			if (score < 0) score = 0; // Evitar pontuacao negativa
		}

		if (VezTurno == Computador) { 
			char tempBoard[3][3]; 
			for (int i = 0; i < 3; i++) { 
				for (int j = 0; j < 3; j++) { 
					if (board[i][j] == 'X') { 
						tempBoard[i][j] = 'x'; 
					} 
					else if (board[i][j] == 'O') { 
						tempBoard[i][j] = 'o'; 
					} 
					else { 
						tempBoard[i][j] = '_'; 
					} 
				} 
			} 
			struct Move thisMove = FindMelhorMovimento(tempBoard); 
			x = thisMove.row; 
			y = thisMove.col; 

			board[x][y] = ComputadorMOVE; 
			printf("Computador colocou %c no slot %d %d\n", 
				ComputadorMOVE, x, y); 
			showBoard(board); 
			moveIndex++; 
			VezTurno = HUMANO; 
		} 
		else if (VezTurno == HUMANO) { 
			int move; 
			printf("Digite seu movimento (1-9): "); 
			scanf("%d", &move); 
			if (move < 1 || move > 9) { 
				printf("Movimente invalido! Por favor, selecione "
					" numero de 1 a 9.\n"); 
				continue; 
			} 
			x = (move - 1) / SIDE; 
			y = (move - 1) % SIDE; 
			if (board[x][y] == ' ') { 
				board[x][y] = PlayerMove; 
				showBoard(board); 
				moveIndex++; 
				if (gameOver(board)) { 
					Ganhador(HUMANO, score); 
					return; 
				} 
				VezTurno = Computador; 
			} 
			else { 
				printf("Slot %d Ja Esta ocupado. Tente "
					"novamente.\n", 
					move); 
			} 
		} 
	} 

	if (!gameOver(board) && moveIndex == SIDE * SIDE) 
		printf("EMPATE!\n"); 
	else { 
		if (VezTurno == Computador) 
			VezTurno = HUMANO; 
		else if (VezTurno == HUMANO) 
			VezTurno = Computador; 

		Ganhador(VezTurno, score); 
	} 
} 

void displayMenuDificuldade() 
{ 
	int escolha; 
	printf("\n--- Selecione a Dificuldade ---\n"); 
	printf("1. Facil\n"); 
	printf("2. Medio\n"); 
	printf("3. Dificil\n"); 
	printf("Digite sua escolha: "); 
	scanf("%d", &escolha); 

	switch (escolha) { 
	case 1: 
		dificuldade = FACIL; 
		break; 
	case 2: 
		dificuldade = MEDIO; 
		break; 
	case 3: 
		dificuldade = DIFICIL; 
		break; 
	default: 
		printf("Escolha invalida! Selecionando dificuldade fácil por padrão.\n"); 
		dificuldade = FACIL; 
		break; 
	} 
} 

enum GameState displayMenu() 
{ 
	int escolha; 
	printf("\n--- Menu ---\n"); 
	printf("1. Start Game\n"); 
	printf("2. Pause Game\n"); 
	printf("3. Resume Game\n"); 
	printf("4. Restart Game\n"); 
	printf("5. Exit\n"); 
	printf("Digite sua escolha: "); 
	scanf("%d", &escolha); 

	switch (escolha) { 
	case 1: 
		displayMenuDificuldade(); 
		return PLAYING; 
	case 2: 
		return PAUSED; 
	case 3: 
		return PLAYING; 
	case 4: 
		return MENU; 
	case 5: 
		return EXIT; 
	default: 
		printf("Escolha invalida! Por favor tente novamente."); 
		return MENU; 
	} 
}

int main() 
{ 
	enum GameState gameState = MENU;
	int VezTurno = Computador; 

	while (gameState != EXIT) { 
		switch (gameState) { 
		case MENU: 
			gameState = displayMenu(); 
			break; 
		case PLAYING: 
			playJogodaVelha(VezTurno); 
			gameState = MENU; 
			break; 
		case PAUSED: 
			printf("Jogo Pausado. Selecione uma opcao do menu para continuar.\n"); 
			gameState = MENU; 
			break; 
		case EXIT: 
			printf("Saiu do jogo.\n"); 
			break; 
		} 
	}

	return 0; 
}

