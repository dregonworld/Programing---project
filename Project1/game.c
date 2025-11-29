#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h> //system("cls")
#include <conio.h>  //_getch()
#include <string.h> //strcpy, sprintf

// [상수 정의]
#define MAP_HEIGHT 10
#define MAP_WIDTH 20
#define MAX_MONSTERS 3
#define MAX_ITEMS 4

// [구조체 정의]
struct Player {
    int x, y;
    int hp, max_hp;
    int atk;
    int hammer; //벽 파괴 아이템 개수
};

struct Monster {
    int x, y;
    int hp, max_hp;
    int atk;
    int alive; //1:생존, 0:사망
};

struct Item {
    int x, y;
    char type; //P:물약, S:검, H:망치
    int value;
    int active; //1:배치됨, 0:획득함
};

// [전역 변수]
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
int current_stage = 1;

// [함수 정의]
//타이틀 화면
void show_title_screen() {
    system("cls");
    printf("\n\n");
    printf("   #####################################\n");
    printf("   #                                   #\n");
    printf("   #       TEXT DUNGEON CRAWLER        #\n");
    printf("   #                                   #\n");
    printf("   #####################################\n\n");
    printf("           [ PRESS ANY KEY ]\n\n");
    printf("   -----------------------------------\n");
    printf("    Move: W,A,S,D  |  Quit: Q\n");
    printf("    Items: (P)otion, (S)word, (H)ammer\n");
    printf("   -----------------------------------\n");
    _getch(); //키 입력 대기
}

//게임 오버 화면
void show_game_over_screen(int is_clear) {
    system("cls");
    printf("\n\n");
    if (is_clear) {
        printf("   *************************************\n");
        printf("   * CONGRATULATIONS!        *\n");
        printf("   *************************************\n\n");
        printf("        당신은 전설이 되었습니다!\n");
    }
    else {
        printf("   =====================================\n");
        printf("   =            GAME OVER...           =\n");
        printf("   =====================================\n\n");
        printf("        당신의 여정은 여기까지입니다.\n");
    }
    printf("\n      [ FINAL RECORD: STAGE %d ]\n", current_stage);
    printf("\n      (Press any key to exit...)\n");
    _getch();
}

//맵 및 상태 출력
void display_map(struct Player* player, struct Monster monsters[], struct Item items[]) {
    system("cls");

    printf("=== [ STAGE %d ] ===\n", current_stage);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH - 1; j++) {

            //우선순위: 플레이어 > 몬스터 > 아이템 > 맵
            if (i == player->y && j == player->x) {
                printf("@");
            }
            else {
                int drawn = 0;
                //몬스터 그리기
                for (int k = 0; k < MAX_MONSTERS; k++) {
                    if (monsters[k].alive && monsters[k].y == i && monsters[k].x == j) {
                        printf("m");
                        drawn = 1;
                        break;
                    }
                }
                if (drawn) continue;

                //아이템 그리기
                for (int k = 0; k < MAX_ITEMS; k++) {
                    if (items[k].active && items[k].y == i && items[k].x == j) {
                        printf("%c", items[k].type);
                        drawn = 1;
                        break;
                    }
                }
                if (drawn) continue;

                //기본 맵 그리기
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }

    //상태창
    printf("\n[STATUS] HP: %d/%d | ATK: %d | HAMMER: %d\n", player->hp, player->max_hp, player->atk, player->hammer);
    printf("--------------------------------------------\n");
    printf("%s\n", game_message);
}

//다음 스테이지 설정
void next_stage(struct Player* player, struct Monster monsters[], struct Item items[]) {
    current_stage++;
    player->x = 1;
    player->y = 1;

    //몬스터 재생성 및 강화
    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i].alive = 1;
        monsters[i].max_hp += 10;
        monsters[i].hp = monsters[i].max_hp;
        monsters[i].atk += 2;
    }
    // 아이템 리셋
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 1;
    }
    sprintf(game_message, "지하 %d층 진입! 적들이 강해졌습니다.", current_stage);
}

// [메인 함수]
int main() {

    //게임 시작 전 타이틀 화면
    show_title_screen();

    //초기 설정
    struct Player player = { 1, 1, 100, 100, 10, 0 };

    struct Monster monsters[MAX_MONSTERS] = {
        {3, 3, 30, 30, 5, 1},
        {7, 6, 30, 30, 5, 1},
        {15, 8, 30, 30, 5, 1}
    };

    struct Item items[MAX_ITEMS] = {
        {5, 5, 'P', 30, 1},
        {10, 1, 'S', 3, 1},
        {15, 3, 'P', 30, 1},
        {2, 1, 'H', 2, 1}
    };

    char input_key;
    int game_running = 1;
    int is_clear = 0; //0: 패배/종료, 1: 클리어(사실상 무한모드라 죽어야 끝남)

    strcpy(game_message, "던전 탐험 시작! 행운을 빕니다.");

    //메인 루프
    while (game_running) {

        display_map(&player, monsters, items);

        input_key = _getch();
        strcpy(game_message, "");

        int next_x = player.x;
        int next_y = player.y;

        //키 입력 처리
        if (input_key == 'w' || input_key == 'W') next_y--;
        else if (input_key == 's' || input_key == 'S') next_y++;
        else if (input_key == 'a' || input_key == 'A') next_x--;
        else if (input_key == 'd' || input_key == 'D') next_x++;
        else if (input_key == 'q' || input_key == 'Q') {
            game_running = 0;
            is_clear = 0;
            continue;
        }

        //스테이지 이동
        if (map[next_y][next_x] == '>') {
            next_stage(&player, monsters, items);
            continue;
        }

        //몬스터 전투
        int monster_index = -1;
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (monsters[i].alive && monsters[i].y == next_y && monsters[i].x == next_x) {
                monster_index = i;
                break;
            }
        }

        if (monster_index != -1) {
            struct Monster* target = &monsters[monster_index];
            target->hp -= player.atk; //플레이어 공격

            if (target->hp <= 0) {
                target->alive = 0;
                sprintf(game_message, "몬스터 처치! (길이 열렸습니다)");
            }
            else {
                player.hp -= target->atk; //몬스터 반격
                sprintf(game_message, "전투! 적 HP:%d | 내 HP:%d", target->hp, player.hp);

                if (player.hp <= 0) {
                    game_running = 0;
                    is_clear = 0; //패배
                }
            }
        }
        //이동 및 아이템
        else if (map[next_y][next_x] != '#') {
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (items[i].active && items[i].y == next_y && items[i].x == next_x) {
                    items[i].active = 0;
                    if (items[i].type == 'P') {
                        player.hp += items[i].value;
                        if (player.hp > player.max_hp) player.hp = player.max_hp;
                        sprintf(game_message, "물약 사용! HP 회복.");
                    }
                    else if (items[i].type == 'S') {
                        player.atk += items[i].value;
                        sprintf(game_message, "공격력 %d 증가!", items[i].value);
                    }
                    else if (items[i].type == 'H') {
                        player.hammer += items[i].value;
                        sprintf(game_message, "망치 획득! (현재: %d개)", player.hammer);
                    }
                }
            }
            player.x = next_x;
            player.y = next_y;

            if (strlen(game_message) == 0) sprintf(game_message, "이동 중... (Stage %d)", current_stage);
        }
        //벽 파괴 (망치)
        else {
            if (player.hammer > 0) {
                map[next_y][next_x] = '.';
                player.hammer--;
                player.x = next_x;
                player.y = next_y;
                sprintf(game_message, "벽 파괴! (남은 망치: %d)", player.hammer);
            }
            else {
                sprintf(game_message, "단단한 벽입니다.");
            }
        }
    }

    //게임 종료 화면
    show_game_over_screen(is_clear);

    return 0;
}