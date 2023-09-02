#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Player
{
    int player_number;
    int current_stage_number;
    struct Player *next;
    int *beaten_players;
};

struct Stack
{
    struct Player *base;
};

void pushPlayer(struct Stack *stack, struct Player *player)
{
    if (stack->base == NULL)
        stack->base = player;
    else
    {
        struct Player *current_player = stack->base;
        for (; current_player->next != NULL; current_player = current_player->next)
            ;
        current_player->next = player;
    }
}

void pushBeatenPlayers(struct Player *player, int beaten_player_number)
{
    int i = 0;
    for (; *(player->beaten_players + i) != 0; i++)
        ;
    *(player->beaten_players + i) = beaten_player_number;
}

void displayPlayersAtAnyGivenStage(int stage_number, struct Player *base)
{
    printf("Displaying %d. stage's players\n", stage_number);
    for (struct Player *player = base; player != NULL; player = player->next)
    {
        if (player->current_stage_number == stage_number)
            printf("%d  ", player->player_number);
    }
    printf("\n");
}

void displayBeatenPlayers(int given_player_number, int n, struct Player *base)
{
    printf("Displaying beaten players by player number %d\n", given_player_number);
    struct Player *player = base;
    for (; player->player_number != given_player_number; player = player->next)
        ;

    if (*(player->beaten_players) == 0)
        printf("This player has no win !\n");
    else
    {
        for (int j = 0; *(player->beaten_players + j) != 0 && j != n; j++)
        {
            printf("%d  ", *(player->beaten_players + j));
        }
        printf("\n");
    }
}

const char *returnSpaceString(int number_of_space)
{
    char *str = (char *)malloc(number_of_space * sizeof(char));
    for (int i = 0; i < number_of_space; i++)
    {
        str[i] = ' ';
    }
    return str;
}

void displayFixture(struct Player *base)
{
    int space = 2;
    int temp_current_stage_number = base->current_stage_number;
    printf("Displaying fixture\n");
    for (struct Player *player = base; player != NULL; player = player->next)
    {
        if (player->current_stage_number != temp_current_stage_number)
        {
            temp_current_stage_number = player->current_stage_number;
            space *= 2;
            printf("\n");
        }
        printf("%s%d%s", returnSpaceString(space), player->player_number, returnSpaceString(space - 1));
    }
}

int main()
{
    struct Stack stack;
    stack.base = NULL;

    struct Player *temp_base;

    int number_of_players, temp, n, compare;
    int main_choice, menu_choice;

    do
    {
        printf("Please enter number of players (must be power of 2) : ");
        scanf("%d", &number_of_players);
    } while (number_of_players % 2 != 0);

    temp = number_of_players;
    n = 0;
    while (temp != 1)
    {
        temp /= 2;
        n++;
    }

    for (int i = 1; i <= number_of_players; i++)
    {
        struct Player *player = (struct Player *)malloc(sizeof(struct Player));
        player->player_number = i;
        player->current_stage_number = 1;
        player->next = NULL;
        player->beaten_players = (int *)calloc(n, sizeof(int));
        pushPlayer(&stack, player);
    }

    temp_base = stack.base;
    compare = pow(2, n);
    for (int j = 2; j <= n + 1; j++)
    {
        compare /= 2;

        for (int c = 0; c < compare; c++)
        {
            struct Player *player = (struct Player *)malloc(sizeof(struct Player));

            printf("Choose the winner\n1) player number %d\n2) player number %d\n", temp_base->player_number, temp_base->next->player_number);
            do
            {
                printf("Your choice (must be 1 or 2) : ");
                scanf("%d", &main_choice);
            } while (!(main_choice == 1 || main_choice == 2));

            player->current_stage_number = j;
            player->next = NULL;

            if (main_choice == 1)
            {
                player->player_number = temp_base->player_number;
                player->beaten_players = temp_base->beaten_players;

                pushBeatenPlayers(player, temp_base->next->player_number);
                pushPlayer(&stack, player);
            }
            else if (main_choice == 2)
            {
                player->player_number = temp_base->next->player_number;
                player->beaten_players = temp_base->next->beaten_players;

                pushBeatenPlayers(player, temp_base->player_number);
                pushPlayer(&stack, player);
            }

            temp_base = temp_base->next->next;
        }
    }

    do
    {
        printf("Welcome to our tennis tournament application !\nMenu\n");
        printf("1. Display the players at any given stage\n");
        printf("2. Display the players he/she has beaten for any given player number\n");
        printf("3. Display the fixture\n");
        printf("4. Exit\nYour choice (must be 1-4) : ");
        scanf("%d", &main_choice);

        if (main_choice == 1)
        {
            do
            {
                printf("Please enter stage number that you want to learn players at it\nYour choice (must be 1-%d) : ", n + 1);
                scanf("%d", &menu_choice);
            } while (!(menu_choice >= 1 && menu_choice <= n + 1));

            displayPlayersAtAnyGivenStage(menu_choice, stack.base);
        }
        else if (main_choice == 2)
        {
            do
            {
                printf("Please enter player number that you want to learn beaten players by him/her\nYour choice (must be 1-%d) : ", number_of_players);
                scanf("%d", &menu_choice);
            } while (!(menu_choice >= 1 && menu_choice <= number_of_players));

            displayBeatenPlayers(menu_choice, n, stack.base);
        }
        else if (main_choice == 3)
            displayFixture(stack.base);
    } while (!(main_choice >= 1 && main_choice <= 4));

    return 0;
}