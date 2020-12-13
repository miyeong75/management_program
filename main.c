#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user {
    int period;
    int num_people;
} user_t;

user_t u1;

typedef struct schedule {
    int day;
    char place[40];
} schedule_t;

typedef struct node_1 {
    schedule_t s;
    struct node_1* next;
} node_s;

typedef struct info {
    char place[40];
    int admission;
} info_t;

typedef struct node_2 {
    info_t i;
    struct node_2* next;
} node_i;

void userInfo(void);
void printSchedule(void);
void makeSchedule(void);
void chPlace(void);
//void chDay(void);
void reset(void);


void main(void) {

    while (1) {
        int n;
        printf("=====================================\n");
        printf("1. 사용자 정보 입력 / 변경\n");
        printf("2. 여행 일정 보기\n");
        printf("3. 관광 일정 짜기\n");
        printf("4. 관광지 변경\n");
        printf("5. 일정 변경\n");
        printf("6. 리셋\n");
        printf("=====================================\n");
        printf("번호 입력 : ");
        scanf("%d", &n);

        switch (n) {
        case 1: userInfo(); break;
        case 2: printSchedule(); break;
        case 3: makeSchedule(); break;
        case 4: chPlace(); break;
            //case 5: chDay(); break;
        case 6: reset(); break;
        default:
            printf("잘못된 입력입니다.\n");
        }
    }
}

void userInfo(void) {

    printf("여행 기간 : ");
    scanf("%d", &u1.period);
    printf("인원 수 : ");
    scanf("%d", &u1.num_people);

    printf("여행 기간 : %d, 인원 수 : %d\n", u1.period, u1.num_people);
}


void printSchedule(void) {

    struct node_1* new_node;
    struct node_1* tmp_node;
    struct node_1* list_head = NULL;
    char place[40];

    FILE* fp = NULL;
    fp = fopen("travel.txt", "r+");
    if (fp == NULL) {
        printf("Cannot open file\n");
        return;
    }


    while (!feof(fp)) {
        new_node = (struct node_1*)malloc(sizeof(struct node_1));
        fscanf(fp, "%d %s\n", &(new_node->s.day), new_node->s.place);
        printf("Day %d : %s\n", new_node->s.day, new_node->s.place);

        new_node->next = list_head;
        list_head = new_node;

    }
    return;
}


void printList(struct node_2* list_head) {

    while (list_head != NULL) {
        printf("%s : %d원\n", list_head->i.place, list_head->i.admission * u1.num_people);
        list_head = list_head->next;
    }
}

void savetravel(struct schedule* p) {

    FILE* fp = NULL;
    fp = fopen("travel.txt", "w");
    for (int i = 0; i < u1.period; i++) {
        fprintf(fp, "%d %s\n", p[i].day, p[i].place);
    }
    fclose(fp);
}

void savefile(node_s* head) {

    FILE* fp = NULL;
    fp = fopen("travel.txt", "w");

    node_s* cur = head->next;
    while (cur != NULL) {
        schedule_t* s = &cur->s;
        fprintf(fp, "%d %s\n", s->day, s->place);
        cur = cur->next;
    }
    fclose(fp);
}


void chNode(node_s* head_ptr, char* rmplace, char* newplace, struct schedule* p) {

    node_s* newNode = (node_s*)malloc(sizeof(node_s));
    node_s* target_ptr = head_ptr->next;
    node_s* prev = head_ptr;
    node_s* tmp = (node_s*)malloc(sizeof(node_s));
    schedule_t s5;
    int cnt = u1.period - 1;

    while (target_ptr != NULL) {
        if (strcmp(target_ptr->s.place, rmplace) == 0) break;
        else {
            prev = target_ptr;
            target_ptr = target_ptr->next;
        }
    }

    if (target_ptr == NULL) return;

    s5.day = target_ptr->s.day;
    strcpy(s5.place, newplace);
    newNode->s = s5;

    tmp = target_ptr;
    prev->next = target_ptr->next;
    free(target_ptr);
    prev->next = newNode;
    newNode->next = tmp->next;

    while (head_ptr != NULL) {
        p[cnt] = head_ptr->s;
        cnt--;
        head_ptr = head_ptr->next;

    }
}


void removeNode(struct node_2* head_ptr, char* place) {

    struct node_2* target_ptr = head_ptr->next;
    struct node_2* prev = head_ptr;

    while (target_ptr != NULL) {
        if (strcmp(target_ptr->i.place, place) == 0) break;
        else {
            prev = target_ptr;
            target_ptr = target_ptr->next;
        }
    }
    if (target_ptr == NULL) return;

    prev->next = target_ptr->next;
    free(target_ptr);
}


void addNode(node_s* head_ptr, schedule_t s) {

    node_s* newNode = (node_s*)malloc(sizeof(node_s));

    node_s* last = head_ptr;

    while (last->next != NULL)
        last = last->next;

    last->next = newNode;

    newNode->s = s;
    newNode->next = NULL;
}


void addSchedule(node_s* head_ptr, void(*add)(node_s* head, schedule_t s), char* place, int cnt) {

    schedule_t s;
    s.day = cnt;
    strcpy(s.place, place);
    add(head_ptr, s);
}

void makeSchedule(void) {
    
    struct node_2* new_node;
    struct node_2* tmp_node;
    struct node_2* list_head = NULL;
    node_s head_1;
    head_1.next = NULL;
    char place[40];
    char input = '0';
    int cnt = 1;

    FILE* fp = NULL;
    fp = fopen("place.txt", "r+");
    if (fp == NULL) {
        printf("Cannot open file\n");
        return;
    }

    while (!feof(fp)) {
        new_node = (struct node_2*)malloc(sizeof(struct node_2));
        fscanf(fp, "%s %d\n", new_node->i.place, &(new_node->i.admission));

        new_node->next = list_head;
        list_head = new_node;

    }

    printList(list_head);
    for (int i = 0; i < u1.period; i++) {
        printf("====================================\n");
        printf("관광지 입력 : ");
        scanf("%s", place);
        printf("====================================\n");
        removeNode(list_head, place);
        printList(list_head);
        addSchedule(&head_1, addNode, place, cnt);
        cnt++;
    }
    savefile(&head_1);
}


void chPlace(void) {

    struct node_1* new_node;
    struct node_1* tmp_node;
    struct node_1* list_head = NULL;
    char place[40];
    char rmplace[40];
    char input = '0';
    struct schedule* p;
    p = (struct schedule*)malloc(u1.period * sizeof(struct schedule));
    int cnt = 1;

    FILE* fp = NULL;
    fp = fopen("travel.txt", "r+");
    if (fp == NULL) {
        printf("Cannot open file\n");
        return;
    }

    while (!feof(fp)) {
        new_node = (struct node_1*)malloc(sizeof(struct node_1));
        fscanf(fp, "%d %s\n", &(new_node->s.day), new_node->s.place);
        printf("Day %d : %s\n", new_node->s.day, new_node->s.place);
        p[cnt - 1].day = cnt;
        strcpy(p[cnt - 1].place, new_node->s.place);
        cnt++;

        new_node->next = list_head;
        list_head = new_node;

    }

    printf("=====================================\n");
    printf("변경하고 싶은 관광지 : ");
    scanf("%s", rmplace);
    printf("=====================================\n");

    struct node_2* new_n;
    struct node_2* tmp_n;
    struct node_2* list_h = NULL;
    node_s head_2;
    head_2.next = NULL;

    FILE* fp_p = NULL;
    fp_p = fopen("place.txt", "r+");
    if (fp_p == NULL) {
        printf("Cannot open file\n");
        return;
    }

    while (!feof(fp_p)) {
        new_n = (struct node_2*)malloc(sizeof(struct node_2));
        fscanf(fp_p, "%s %d\n", new_n->i.place, &(new_n->i.admission));

        new_n->next = list_h;
        list_h = new_n;

    }

    for (int i = 0; i < u1.period; i++) {
        removeNode(list_h, p[i].place);
    }
    printList(list_h);

	printf("=====================================\n");
    printf("가고 싶은 관광지 : ");
    scanf("%s", place);

    chNode(list_head, rmplace, place, p);
    savetravel(p);

}

void reset(void) {

    FILE* fp = NULL;
    fp = fopen("travel.txt", "w");
    if (fp == NULL) {
        printf("Failed to open file %s\n", "travel.txt");
    }

    fprintf(fp, " ");
    fclose(fp);
}

