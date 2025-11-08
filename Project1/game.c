#include <stdio.h>
#include <stdlib.h> // system("cls") 함수를 위해 필요
#include <conio.h>  // _getch() 함수를 위해 필요 (Windows)

// 맵 크기를 상수로 정의
#define MAP_HEIGHT 10
#define MAP_WIDTH 20

// 플레이어 구조체 정의
struct Player {
    int x;
    int y;
};

// 맵 2차원 배열 선언
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "###################",
    "#..................",
    "#.###############.#",
    "#.#.............#.#",
    "#.#.#...#.#...#.#.#",
    "#.#.#...#.#...#.#.#",
    "#.#.#...#.#...#...#",
    "#.###############.#",
    "#.................#",
    "###################"
};

// 화면 출력
void display_map(struct Player* player) {
    system("cls");

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH - 1; j++) {

            if (i == player->y && j == player->x) {
                printf("@");
            }
            else {
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }

    printf("\n(x: %d, y: %d) | q: 종료\n", player->x, player->y);
}

int main() {
    // 5. 플레이어 변수 생성 및 초기 위치 설정
    struct Player player;
    player.x = 1; // 시작 x좌표
    player.y = 1; // 시작 y좌표

    char input_key;

    // 6. 메인 게임 루프 (while(1))
    while (1) {

        display_map(&player);

        // 키 입력
        input_key = _getch();

        // 7. 키 입력 처리 (switch 또는 if-else)

        int next_x = player.x;
        int next_y = player.y;

        if (input_key == 'w') { // 위
            next_y--;
        }
        else if (input_key == 's') { // 아래
            next_y++;
        }
        else if (input_key == 'a') { // 왼쪽
            next_x--;
        }
        else if (input_key == 'd') { // 오른쪽
            next_x++;
        }
        else if (input_key == 'q') { // 종료
            break;
        }

        // 8. 벽 충돌 최종 검사
        if (map[next_y][next_x] != '#') {
            player.x = next_x;
            player.y = next_y;
        }
    }

    printf("게임을 종료했습니다.\n");
    return 0;
}