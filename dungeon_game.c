#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INVENTORY 5
#define MAX_DESCRIPTION 50
#define MAX_FLOORS 10
#define MAX_NAME_LENGTH 50

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int health;
    int strength;
    int defense;
    int inventory_count;
    char inventory[MAX_INVENTORY][MAX_NAME_LENGTH];
} Player;

typedef struct
{
    char description[MAX_DESCRIPTION];
    int connections[2];
    char items[MAX_INVENTORY][MAX_NAME_LENGTH];
    int item_count;
    int creature_health;
    int creature_strength;
    int creature_defense;
} Floor;

Player player;
Floor floors[MAX_FLOORS];
int current_floor = -1;

void start_game(const char *name);
void display_status();
void display_help();
void display_floor();
void move_player(const char *direction);
void look();
void show_inventory();
void pickup_item(const char *item_name);
void drop_item(const char *item_name);
void use_item(const char *item_name);
void attack();
void save_game(const char *filepath);
void load_game(const char *filepath);

int main()
{
    char command[50];
    char argument[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];

    printf("Welcome to the Dungeon Adventure Game!\n");
    printf("Please enter a name for the character: ");
    scanf("%s", name);
    printf("Please type \"move down\" for start the game !\n");
    start_game(name);
    printf("Type 'help' for commands.\n");

    while (1)
    {
        printf("\n> ");
        scanf("%s", command);
        

        if (strcmp(command, "status") == 0)
        {
            display_status();
        }
        else if (strcmp(command, "move") == 0)
        {
            scanf("%s", argument);
            move_player(argument);
        }
        else if (strcmp(command, "look") == 0)
        {
            look();
        }
        else if (strcmp(command, "inventory") == 0)
        {
            show_inventory();
        }
        else if (strcmp(command, "pickup") == 0)
        {
            scanf("%s", argument);
            pickup_item(argument);
        }
        else if (strcmp(command, "drop") == 0)
        {
            scanf("%s", argument);
            drop_item(argument);
        }
        else if (strcmp(command, "use") == 0)
        {
            scanf("%s", argument);
            use_item(argument);
        }
        else if (strcmp(command, "attack") == 0)
        {
            attack();
        }
        else if (strcmp(command, "save") == 0)
        {
            scanf("%s", argument);
            save_game(argument);
        }
        else if (strcmp(command, "load") == 0)
        {
            scanf("%s", argument);
            load_game(argument);
        }
        else if (strcmp(command, "help") == 0)
        {
            display_help();
        }
        else if (strcmp(command, "exit") == 0)
        {
            printf("Exiting the game. \n");
            break;
        }
        else
        {
            printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }

    return 0;
}

void start_game(const char *name)
{
    strncpy(player.name, name, MAX_NAME_LENGTH);
    player.health = 100;
    player.strength = 15;
    player.defense = 10;
    player.inventory_count = 0;

    for (int i = 0; i < MAX_FLOORS; i++)
    {
        snprintf(floors[i].description, MAX_DESCRIPTION, "This is floor %d. What will you do?", i);
        floors[i].item_count = 0;
        floors[i].creature_health = (int)(i * 1.75 * 10);
        floors[i].creature_strength = (int)(i * 1.5 + 20);
        floors[i].connections[0] = (i == 0) ? 0 : i - 1;
        floors[i].connections[1] = (i == MAX_FLOORS - 1) ? i : i + 1;
    }

    strcpy(floors[0].items[0], "sword");
    strcpy(floors[0].items[1], "shield");
    floors[0].item_count = 2;

    strcpy(floors[1].items[0], "leatherarmor");
    floors[1].item_count = 1;

    strcpy(floors[2].items[0], "longsword");
    strcpy(floors[2].items[1], "healthpotion");
    floors[2].item_count = 2;

    strcpy(floors[3].items[0], "oldhelmet");
    floors[3].item_count = 1;

    strcpy(floors[5].items[0], "wizardhat");
    strcpy(floors[5].items[1], "amulet");
    floors[5].item_count = 2;

    strcpy(floors[6].items[0], "glasses");
    strcpy(floors[6].items[1], "pants");
    floors[6].item_count = 2;

    strcpy(floors[7].items[0], "bow");
    strcpy(floors[7].items[1], "arrow");
    floors[7].item_count = 2;

    strcpy(floors[9].items[0], "excalibur");
    floors[9].item_count = 1;
}

void display_status()
{
    printf("\nPlayer Stats:\n");
    printf("Name: %s\n", player.name);
    printf("Health: %d\n", player.health);
    printf("Strength: %d\n", player.strength);
    printf("Defense: %d\n", player.defense);
    printf("\n");
}

void display_help()
{
    printf("Available commands:\n");
    printf("  status           : Displays player status.\n");
    printf("  move <direction> : Move in a direction (up, down).\n");
    printf("  look             : Look around the current floor.\n");
    printf("  inventory        : Show your inventory.\n");
    printf("  pickup <item>    : Pick up an item in the floor.\n");
    printf("  drop <item>      : Drop an item in the floor.\n");
    printf("  use <item>       : Use an item.\n");
    printf("  attack           : Attack a creature in the floor.\n");
    printf("  save <filepath>  : Save the game state to a file.\n");
    printf("  load <filepath>  : Load the game state from a file.\n");
    printf("  exit             : Exit the game.\n");
}

void display_floor()
{
    printf("%s\n", floors[current_floor].description);
    printf("Items in the floor:\n");
    for (int i = 0; i < floors[current_floor].item_count; i++)
    {
        printf("  - %s\n", floors[current_floor].items[i]);
    }
    if (floors[current_floor].creature_health > 0)
    {
        printf("A creature is here with %d health and %d strength.\n", floors[current_floor].creature_health, floors[current_floor].creature_strength);
    }
}

void move_player(const char *direction)
{
    int dir_index = -1;
    if (strcmp(direction, "up") == 0)
    {
        dir_index = 0;
    }
    else if (strcmp(direction, "down") == 0 )
    {
        if(floors[current_floor].creature_health <= 0){
            dir_index = 1;
        }
        else{
            printf("You must destroy the creature for the move next floor \n");
            return;
        }
    }

    if (dir_index == -1)
    {
        printf("Invalid direction. Use 'up' or 'down'.\n");
        return;
    }

    current_floor = floors[current_floor].connections[dir_index];
    printf("You moved to floor %d.\n", current_floor);
    display_floor();
}

void look()
{
    display_floor();
}

void show_inventory()
{
    printf("Your inventory:\n");
    for (int i = 0; i < player.inventory_count; i++)
    {
        printf("  - %s\n", player.inventory[i]);
    }
}

void pickup_item(const char *item_name)
{
    for (int i = 0; i < floors[current_floor].item_count; i++)
    {
        if (strcmp(floors[current_floor].items[i], item_name) == 0)
        {
            if (player.inventory_count >= MAX_INVENTORY)
            {
                printf("Your inventory is full!\n");
                return;
            }

            strcpy(player.inventory[player.inventory_count++], item_name);

            for (int j = i; j < floors[current_floor].item_count - 1; j++)
            {
                strcpy(floors[current_floor].items[j], floors[current_floor].items[j + 1]);
            }
            floors[current_floor].item_count--;

            printf("You picked up %s.\n", item_name);
            return;
        }
    }

    printf("No such item in the floor.\n");
}

void drop_item(const char *item_name)
{
    int item_index = -1;
    for (int i = 0; i < player.inventory_count; i++)
    {
        if (strcmp(player.inventory[i], item_name) == 0)
        {
            item_index = i;
            break;
        }
    }

    if (item_index == -1)
    {
        printf("You don't have %s in your inventory.\n", item_name);
        return;
    }

    if (floors[current_floor].item_count >= MAX_INVENTORY)
    {
        printf("The floor is full of items. You can't drop %s here.\n", item_name);
        return;
    }

    strcpy(floors[current_floor].items[floors[current_floor].item_count], item_name);
    floors[current_floor].item_count++;

    for (int i = item_index; i < player.inventory_count - 1; i++)
    {
        strcpy(player.inventory[i], player.inventory[i + 1]);
    }
    player.inventory_count--;

    printf("You dropped %s on the floor.\n", item_name);
}

void use_item(const char *item_name)
{
    int item_index = -1;
    for (int i = 0; i < player.inventory_count; i++)
    {
        if (strcmp(player.inventory[i], item_name) == 0)
        {
            item_index = i;
            break;
        }
    }

    if (item_index == -1)
    {
        printf("You don't have %s in your inventory.\n", item_name);
        return;
    }

    if (strcmp(item_name, "healthpotion") == 0)
    {
        player.health += 25;
        printf("You used a health potion. Your health is now %d.\n", player.health);
    }
    else if (strcmp(item_name, "sword") == 0)
    {
        player.strength += 10;
        printf("You used a sword. Your strength increased temporary to %d.\n", player.strength);
        attack();
        player.strength -= 10;
    }
    else if (strcmp(item_name, "longsword") == 0)
    {
        player.strength += 20;
        printf("You used a longsword. Your strength increased temporary to %d.\n", player.strength);
        attack();
        player.strength -= 20;
    }
    else if (strcmp(item_name, "shield") == 0)
    {
        player.defense += 10;
        printf("You used a shield. Your defense increased to %d.\n", player.defense);
    }
    else if (strcmp(item_name, "leatherarmor") == 0)
    {
        player.defense += 15;
        printf("You equipped leather armor. Your defense increased to %d.\n", player.defense);
    }
    else if (strcmp(item_name, "excalibur") == 0)
    {
        player.strength += 50;
        printf("You are using excalibur. Your strength increased to %d.\n", player.strength);
    }
    else
    {
        printf("You can't use %s right now.\n", item_name);
        return;
    }

    for (int i = item_index; i < player.inventory_count - 1; i++)
    {
        strcpy(player.inventory[i], player.inventory[i + 1]);
    }
    player.inventory_count--;
}

void attack()
{
    if (floors[current_floor].creature_health <= 0)
    {
        printf("There's no creature here to attack.\n");
        return;
    }
    printf("You attacked the creature!\n");
    floors[current_floor].creature_health -= player.strength;
    if (floors[current_floor].creature_health > 0)
    {
        printf("The creature attacks back!\n");
        player.health -= (floors[current_floor].creature_strength - player.defense);
    }
    if (floors[current_floor].creature_health <= 0)
    {
        printf("You defeated the creature!\n");
    }
    if (player.health <= 0)
    {
        printf("You have been defeated. Game Over!\n");
        exit(0);
    }
}

void save_game(const char *filepath)
{
    FILE *file = fopen(filepath, "w");
    if (!file)
    {
        printf("Failed to save the game.\n");
        return;
    }

    fwrite(&player, sizeof(Player), 1, file);
    fwrite(&floors, sizeof(Floor), MAX_FLOORS, file);
    fwrite(&current_floor, sizeof(int), 1, file);

    fclose(file);
    printf("Game saved to %s.\n", filepath);
}

void load_game(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        printf("Failed to load the game.\n");
        return;
    }

    fread(&player, sizeof(Player), 1, file);
    fread(&floors, sizeof(Floor), MAX_FLOORS, file);
    fread(&current_floor, sizeof(int), 1, file);

    fclose(file);
    printf("Game loaded from %s.\n", filepath);
}
