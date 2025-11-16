#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> // system("cls")
#include <conio.h>  // _getch()
#include <string.h> // strcpy, sprintf

//맵 크기 상수
#define MAP_HEIGHT 10
#define MAP_WIDTH 20

//몬스터 최대 수
#define MAX_MONSTERS 3

//플레이어 구조체 정의
struct Player {
    int x;
    int y;
    int hp;
    int atk;
};

//몬스터 구조체 정의
struct Monster {
    int x;
    int y;
    int hp;
    int atk;
    int alive; // 1 = 살아있음, 0 = 죽음
};

//맵 2차원 배열
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "###################",
    "#.................#",
    "#.###############.#",
    "#.#.............#.#",
    "#.#.#####.#####.#.#",
    "#.#.#...#.#...#.#.#",
    "#.#.#...#.#...#.#.#",
    "#.###############.#",
    "#...............>#", // 출구
    "###################"
};

//화면 하단에 출력될 게임 상태 메시지
char game_message[100] = "";

//화면을 다시 그리는 함수
void display_map(struct Player* player, struct Monster monsters[]) {
    system("cls"); // 화면 지우기

    //맵 그리기
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH - 1; j++) {

            //플레이어 위치인가?
            if (i == player->y && j == player->x) {
                printf("@");
                continue; // 플레이어가 최우선
            }

            //몬스터 위치인가?
            int monster_here = 0;
            for (int k = 0; k < MAX_MONSTERS; k++) {
                if (monsters[k].alive && monsters[k].y == i && monsters[k].x == j) {
                    printf("m");
                    monster_here = 1;
                    break;
                }
            }
            if (monster_here) {
                continue; //몬스터가 다음 순위
            }

            //둘 다 아니면 맵 문자 출력
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    //플레이어 스탯 및 게임 메시지 출력
    printf("\nPlayer HP: %d | ATK: %d\n", player->hp, player->atk);
    printf("------------------------------\n");
    printf("%s\n", game_message); //게임 메시지 출력
}

int main() {

    //플레이어 스탯 초기화
    struct Player player;
    player.x = 1;
    player.y = 1;
    player.hp = 100;
    player.atk = 10;

    //몬스터 초기화 (위치, hp, atk, 생존여부)
    struct Monster monsters[MAX_MONSTERS];
    monsters[0] = (struct Monster){ 3, 3, 30, 5, 1 };
    monsters[1] = (struct Monster){ 8, 6, 30, 5, 1 };
    monsters[2] = (struct Monster){ 15, 7, 30, 5, 1 };

    char input_key;
    int game_running = 1; //게임 오버/클리어 상태 변수 (1 = 진행중)

    strcpy(game_message, "던전에 오신 것을 환영합니다! (q: 종료)");

    //메인 게임 루프
    while(game_running) {

        display_map(&player, monsters);

        input_key = _getch();
        strcpy(game_message, ""); //새 입력 받기 전 메시지 초기화

        //이동할 다음 좌표 계산
        int next_x = player.x;
        int next_y = player.y;

        if (input_key == 'w') { next_y--; }
        else if (input_key == 's') { next_y++; }
        else if (input_key == 'a') { next_x--; }
        else if (input_key == 'd') { next_x++; }
        else if (input_key == 'q') {
            game_running = 0;
            strcpy(game_message, "게임을 종료했습니다.");
            continue;
        }

        //승리 조건 검사 (출구)
        if (map[next_y][next_x] == '>') {
            game_running = 0;
            strcpy(game_message, "GAME CLEAR! 던전을 탈출했습니다!");
            continue;
        }

        //몬스터 충돌 검사 (전투)
        int monster_index = -1; // -1이면 몬스터 없음
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (monsters[i].alive && monsters[i].y == next_y && monsters[i].x == next_x) {
                monster_index = i;
                break;
            }
        }

        if (monster_index != -1) {
            //몬스터와 마주침 -> 전투!
            struct Monster* target = &monsters[monster_index];

            //플레이어가 몬스터 공격
            target->hp -= player->atk;

            if (target->hp <= 0) {
                //몬스터 사망
                target->alive = 0;
                sprintf(game_message, "몬스터를 처치했습니다!");
            }
            else {
                //몬스터가 반격
                player.hp -= target->atk;
                sprintf(game_message, "몬스터를 공격! (적 HP: %d) | 반격당함! (HP: %d)", target->hp, player.hp);

                //사망 조건 검사 (플레이어)
                if (player.hp <= 0) {
                    game_running = 0;
                    strcpy(game_message, "GAME OVER... 몬스터에게 패배했습니다.");
                }
            }
        }
        //벽 충돌 검사
        else if (map[next_y][next_x] != '#') {
            //몬스터가 없고 벽도 아니면 -> 이동
            player.x = next_x;
            player.y = next_y;
            sprintf(game_message, "이동... (x: %d, y: %d)", player.x, player.y);
        }
        else {
            //벽이라서 이동 불가
            sprintf(game_message, "벽에 막혔습니다!");
        }
    }

    //게임 루프 종료 후 마지막 상태 표시
    display_map(&player, monsters);
    printf("\n%s\n", game_message); //최종 메시지 출력

    return 0;
}