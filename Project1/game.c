#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h> // system("cls")
#include <conio.h>  // _getch()
#include <string.h> // strcpy, sprintf

#define MAP_HEIGHT 10
#define MAP_WIDTH 20

#define MAX_MONSTERS 3
#define MAX_ITEMS 3     // 아이템 개수

// 플레이어 구조체
struct Player {
    int x;
    int y;
    int hp;
    int max_hp; // 최대 체력 (과회복 방지용)
    int atk;
};

// 몬스터 구조체
struct Monster {
    int x;
    int y;
    int hp;
    int max_hp;
    int atk;
    int alive;
};

// 아이템 구조체
struct Item {
    int x;
    int y;
    char type; // 'P':물약, 'S':검
    int value; // 회복량 또는 공격력 증가량
    int active; // 1:존재함, 0:획득함
};

// 맵 데이터
char map[MAP_HEIGHT][MAP_WIDTH] = {
    "###################",
    "#.................#",
    "#.###############.#",
    "#.#...............#",
    "#.#.#####.#####.#.#",
    "#.#.#...#.#...#.#.#",
    "#.#.#...#.#...#.#.#",
    "#.###############.#",
    "#...............>#",
    "###################"
};

char game_message[100] = "";
int current_stage = 1; // 현재 스테이지

// 화면 출력 함수
void display_map(struct Player* player, struct Monster monsters[], struct Item items[]) {
    system("cls");

    printf("=== [ STAGE %d ] ===\n", current_stage); // 스테이지 표시

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH - 1; j++) {

            // 1. 플레이어
            if (i == player->y && j == player->x) {
                printf("@");
                continue;
            }

            // 2. 몬스터
            int monster_here = 0;
            for (int k = 0; k < MAX_MONSTERS; k++) {
                if (monsters[k].alive && monsters[k].y == i && monsters[k].x == j) {
                    printf("m");
                    monster_here = 1;
                    break;
                }
            }
            if (monster_here) continue;

            // 3. 아이템 (3주차 추가)
            int item_here = 0;
            for (int k = 0; k < MAX_ITEMS; k++) {
                if (items[k].active && items[k].y == i && items[k].x == j) {
                    printf("%c", items[k].type); // P 또는 S 출력
                    item_here = 1;
                    break;
                }
            }
            if (item_here) continue;

            // 4. 기본 맵
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    printf("\n[STATUS] HP: %d/%d | ATK: %d\n", player->hp, player->max_hp, player->atk);
    printf("----------------------------------\n");
    printf("%s\n", game_message);
}

// 다음 스테이지 세팅 함수
void next_stage(struct Player* player, struct Monster monsters[], struct Item items[]) {
    current_stage++; // 층수 증가

    // 플레이어 위치 초기화
    player->x = 1;
    player->y = 1;

    // 몬스터 부활 및 강화 (난이도 상승)
    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].alive = 1;
        monsters[i].max_hp += 10;       // 체력 증가
        monsters[i].hp = monsters[i].max_hp;
        monsters[i].atk += 2;           // 공격력 증가
    }

    // 아이템 리셋 (다시 먹을 수 있게)
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 1;
    }

    sprintf(game_message, "지하 %d층으로 내려왔습니다! 몬스터가 강해졌습니다!", current_stage);
}

int main() {

    // 플레이어 초기화
    struct Player player = { 1, 1, 100, 100, 10 }; // x, y, hp, max_hp, atk

    // 몬스터 초기화
    struct Monster monsters[MAX_MONSTERS] = {
        {3, 3, 30, 30, 5, 1},
        {7, 6, 30, 30, 5, 1}, 
        {15, 8, 30, 30, 5, 1}
    };

    // 아이템 초기화 (좌표 수정됨)
    struct Item items[MAX_ITEMS] = {
        {5, 5, 'P', 30, 1},
        {10, 1, 'S', 3, 1}, 
        {15, 3, 'P', 30, 1} 
    };

    char input_key;
    int game_running = 1;

    strcpy(game_message, "던전 탐험을 시작합니다! (P:물약, S:검)");

    while (game_running) {

        display_map(&player, monsters, items);

        input_key = _getch();
        strcpy(game_message, "");

        int next_x = player.x;
        int next_y = player.y;

        // 키 입력 처리 (대소문자 모두 허용)
        if (input_key == 'w' || input_key == 'W') next_y--;
        else if (input_key == 's' || input_key == 'S') next_y++;
        else if (input_key == 'a' || input_key == 'A') next_x--;
        else if (input_key == 'd' || input_key == 'D') next_x++;
        else if (input_key == 'q' || input_key == 'Q') {
            game_running = 0;
            strcpy(game_message, "게임을 종료했습니다.");
            continue;
        }

        // 1. 다음 스테이지 이동
        if (map[next_y][next_x] == '>') {
            next_stage(&player, monsters, items);
            continue;
        }

        //몬스터 충돌 및 전투
        int monster_index = -1;
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (monsters[i].alive && monsters[i].y == next_y && monsters[i].x == next_x) {
                monster_index = i;
                break;
            }
        }

        if (monster_index != -1) {
            struct Monster* target = &monsters[monster_index];

            // 공격
            target->hp -= player.atk;

            if (target->hp <= 0) {
                target->alive = 0;
                sprintf(game_message, "몬스터 처치! (경험치 획득)");
            }
            else {
                // 반격
                player.hp -= target->atk;
                sprintf(game_message, "전투! 몬스터 HP:%d | 플레이어 HP:%d", target->hp, player.hp);

                if (player.hp <= 0) {
                    game_running = 0;
                    strcpy(game_message, "GAME OVER... 당신은 차가운 바닥에 쓰러졌습니다.");
                }
            }
        }
        else if (map[next_y][next_x] != '#') {
            //아이템 획득 로직
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (items[i].active && items[i].y == next_y && items[i].x == next_x) {
                    items[i].active = 0; // 아이템 획득 처리

                    if (items[i].type == 'P') { // 물약
                        player.hp += items[i].value;
                        if (player.hp > player.max_hp) player.hp = player.max_hp; // 최대 체력 제한
                        sprintf(game_message, "물약 획득! HP가 회복되었습니다.");
                    }
                    else if (items[i].type == 'S') { // 검
                        player.atk += items[i].value;
                        sprintf(game_message, "낡은 검 획득! 공격력이 %d 증가했습니다.", items[i].value);
                    }
                }
            }

            // 이동 반영
            player.x = next_x;
            player.y = next_y;

            // 메시지가 비어있다면 이동 메시지
            if (strlen(game_message) == 0)
                sprintf(game_message, "이동 중... (Stage %d)", current_stage);

        }
        else {
            sprintf(game_message, "벽입니다. 지나갈 수 없습니다.");
        }
    }

    display_map(&player, monsters, items);
    printf("\n%s\n", game_message);

    return 0;
}