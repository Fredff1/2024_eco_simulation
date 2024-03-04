#include <stdio.h>
#include <pthread.h>
#include "conway.h"
#include <unistd.h>

static int count = 0;

void clear_screen()
{
    // Use ANSI escape codes to clear the screen
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
}

void print_help()
{
    printf("-----------------------------------------------------------------------\n"
           "|i<row><col> initialize an empty grid with <row> rows and <col>columns|\n"
           "|r                        randomly set alive/dead states for all grids|\n"
           "|n                                         evolve into next generation|\n"
           "|c                    automatically evolve,until receiving 'b' command|\n"
           "|b                                                     pause evolution|\n"
           "|s<path>                               save grid states to file <path>|\n"
           "|l<path>                             Load grid staees from file <path>|\n"
           "|q                                                                quit|\n"
           "|h                                                                help|\n"
           "-----------------------------------------------------------------------\n");
}

void print_game(const Conway *c)
{
    show_conway(c);
}

void *listen_break(void *flag)
{
    char c = 0;
    while (c != 'b')
    {
        scanf(" %c", &c);
        scanf("%*[^\n]"); // 清除未读内容
    }

    *(int *)flag = 1;
    return NULL;
}

void automatic_evolve(Conway *c, int ch)
{
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break, &flag);
    while (flag != 1)
    {
        clear_screen();
        if (ch == 1)
        {
            next_generation(c);
        }
        else if (ch == 2)
        {
            next_generation_food(c);
            delete_food(c);
            food_initialize(c);
        }
        print_game(c);
        printf("round: %d\n", ++count);
        printf("automatically evolving...\n");
        sleep(1); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    print_game(c);
}

int main()
{
    int row, col;
    Conway c;
    char running = 1;
    int ch;
    printf("1 for the no food rule\n"
           "2 for the food rule\n");
    scanf("%d", &ch);
    print_help();
    while (running)
    {
        char cmd;
        char path[40];
        scanf(" %c", &cmd);
        switch (cmd)
        {
        case 'i':
            scanf("%d%d", &row, &col);
            c = new_conway(row, col);
            print_game(&c);
            break;
        case 'r':
            clear_screen();
            init_random(&c);
            if (ch == 2)
            {
                food_initialize(&c);
            }
            print_game(&c);
            break;
        case 'n':
            clear_screen();
            if (ch == 1)
            {
                next_generation(&c);
            }
            else if (ch == 2)
            {
                next_generation_food(&c);
                delete_food(&c);
                food_initialize(&c);
            }
            print_game(&c);
            printf("round: %d\n", ++count);
            break;
        case 'c':
            automatic_evolve(&c, ch);
            break;
        case 's':
            scanf("%s", path);
            save_conway(&c, path);
            break;
        case 'l':
            delete_grids(&c);
            printf("Please enter a file name: ");
            scanf("%s", path);
            c = new_conway_from_file(path);
            print_game(&c);
            break;
        case 'q':
            running = 0;
            break;
        case 'h':
            print_help();
            break;
        default:
            break;
        }
    }
    return 0;
}