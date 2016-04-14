#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tavern.h"

/* Function used in menus that waits until the user presses Enter */
void enterToContinue() {
    char *userInput = '0';
    fflush(stdin);
    while (userInput != '\r' && userInput != '\n') {
        userInput = getchar();
    }
}

/* Displays all the information of a potion */
void printPotion(struct Player *player, int n) {
    /* Previous is a boolean which depends on whether or not the previous stat was displayed */
    int previous = 0;
    struct UsableItem *potion = returnListElementUsable(player->playerPotions, n);
    system("cls");
    printf("Potion %d/%d \n\nName: %s\n\n", n, dlistUsable_length(player->playerPotions), potion->name);
    printf("This potion gives");
    if (potion->hp != 0) {
        printf(" %d hp ", potion->hp);
        previous = 1;
    } else {
        previous = 0;
    }
    if (potion->attack != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d attack ", potion->attack);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->relativeDefense != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d relative defense ", potion->relativeDefense);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->absoluteDefense != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d absolute defense ", potion->absoluteDefense);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->dodge != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d dodge ", potion->dodge);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->totalDuration != 0) {
        printf("for %d turns.\n\n", potion->totalDuration);
    }
    printf("Cost : %d gold\n", potion->goldValue);
    printf("(Press (ENTER) to check next potion) ");

}

/* First menu to display player's potions */
void managePotions(struct Player *player) {
    int i = 0;
    system("cls");
    printf("You have %d total potions.\n", dlistUsable_length(player->playerPotions));
    if (dlistUsable_length(player->playerPotions) == 0) {
        printf("(Press (ENTER) to go back to the previous menu) ");
        enterToContinue();
        manageInventory(player);
    }
    printf("(Press (ENTER) to start checking potions) ");
    enterToContinue();
    for (i = 0; i < dlistUsable_length(player->playerPotions); i++) {
        printPotion(player, i + 1);
        enterToContinue();
    }
    manageInventory(player);
}

/* Displays the stats of an item in player's inventory */
void printItem(struct Player *player, int n) {
    struct StuffItem *item = returnListElementItem(player->playerInventory, n);
    system("cls");
    printf("Item %d/%d \n\nName: %s\n\n", n, dlistItems_length(player->playerInventory), item->name);
    printf("Attack: %d \t\tAbsolute Defense: %d\nHealth points: %d \t Relative Defense: %d\n\n",
           item->attack, item->absoluteDefense, item->hp, item->relativeDefense);
    printf("Cost : %d gold\n\n", item->goldValue);
    printf("(Press (E) to equip or (ENTER) to check next item) ");
}

/* Manages the display of all the player's inventory */
void manageItems(struct Player *player) {
    int i = 0;
    char *userInput = '0';

    system("cls");
    printf("You have %d total items in your inventory.\n", dlistItems_length(player->playerInventory));
    if (dlistItems_length(player->playerInventory) == 0) {
        printf("(Press (ENTER) to go back to the previous menu) ");
        enterToContinue();
        manageInventory(player);
    }
    printf("(Press (ENTER) to start checking items) ");
    enterToContinue();
    for (i = 0; i < dlistItems_length(player->playerInventory); i++) {
        printItem(player, i + 1);
        scanf("%c", &userInput);
        if (userInput == 'E' || userInput == 'e') {
            replaceItemEquip(player, i + 1);
            printf("(Press (ENTER) to continue) ");
            enterToContinue();
            manageInventory(player);
        }
    }
    manageInventory(player);
}

/* Displays most of the player's general information */
void printProfile(struct Player *player) {
    system("cls");
    printf("%s's profile\n\n", player->mob->name);
    printf("Attack: %d \t\tRelative Defense: %d\n", calcAttack(player), calcRelDef(player));
    printf("Health Points: %d\tAbsolute Defense: %d\n", calcHp(player), calcAbsDef(player));
    printf("Dodge score: %d \n\n", player->mob->dodge);
    printf("You have %d lives left, and own %d gold.\n", player->lives, player->gold);
    printf("(Press (ENTER) to go back to the previous menu) ");
    enterToContinue();
    manageInventory(player);
}

/* Menu to choose which one of his inventory, potions or profile the players wants to see */
int manageInventory(struct Player *player) {
    char *userInput = '0';

    system("cls");
    printf("Do you want to see your character's profile (C)\n");
    printf("or manage your items' inventory (I)\n");
    printf("or manage your potions (P) ? \n");
    printf("(Press (Q) to go back to the tavern menu) ");
    fflush(stdin);
    scanf("%c", &userInput);
    if (userInput == 'C' || userInput == 'c') {
        printProfile(player);
    } else if (userInput == 'I' || userInput == 'i') {
        manageItems(player);
    } else if (userInput == 'P' || userInput == 'p') {
        managePotions(player);
    } else if (userInput == 'Q' || userInput == 'q') {
        enterTavern(player);
    }
}

/* prints the caracteristics of a given weapon
 * if there are no more weapons, returns 0 */
int printWeaponBuy(int n) {
    n = max(7, n);
    if (n == dlistItems_length(getItems()) + 1) {
        return 0;
    }
    system("cls");
    struct StuffItem *item = returnListElementItem(getItems(), n);

    while (item->typeId != 4 && item->typeId != 5) {
        n++;
        if (n == dlistItems_length(getItems()) + 1) {
            return 0;
        } else {
            item = returnListElementItem(getItems(), n);
        }
    }

    printf("Name: %s\n\n", item->name);
    printf("Attack: %d \t\t Absolute Defense: %d\nHealth points: %d \t Relative Defense: %d\n\n",
           item->attack, item->absoluteDefense, item->hp, item->relativeDefense);
    printf("Cost : %d gold\n", item->goldValue);
    printf("Buy (B) or go onto the next item (N) ? ");
    return n;
}

/* prints the caracteristics of a given armor
 * if there are no more armors, returns 0 */
int printArmorBuy(int n) {
    n = max(7, n);
    if (n == dlistItems_length(getItems()) + 1) {
        return 0;
    }
    system("cls");
    struct StuffItem *item = returnListElementItem(getItems(), n);

    while (item->typeId == 4 || item->typeId == 5) {
        n++;
        if (n == dlistItems_length(getItems()) + 1) {
            return 0;
        } else {
            item = returnListElementItem(getItems(), n);
        }
    }
    printf("Name: %s\n\n", item->name);
    printf("Attack: %d \t\t Absolute Defense: %d\nHealth points: %d \t Relative Defense: %d\n\n",
           item->attack, item->absoluteDefense, item->hp, item->relativeDefense);
    printf("Cost : %d gold\n", item->goldValue);

    printf("Buy (B) or go onto the next item (N) ? ");
    return n;
}

/* prints the caracteristics of a given potion
 * if there are no more potions, returns 0 */
int printPotionBuy(int n) {
    int previous = 0;
    if (n == dlistUsable_length(getUsable()) + 1) {
        return 0;
    }
    struct UsableItem *potion = returnListElementUsable(getUsable(), n);
    system("cls");
    printf("Name: %s\n\n", potion->name);
    printf("This potion gives");
    if (potion->hp != 0) {
        printf(" %d hp ", potion->hp);
        previous = 1;
    } else {
        previous = 0;
    }
    if (potion->attack != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d attack ", potion->attack);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->relativeDefense != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d relative defense ", potion->relativeDefense);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->absoluteDefense != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d absolute defense ", potion->absoluteDefense);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->dodge != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d dodge ", potion->dodge);
        previous = 1;
    } else {
        previous = 0;
    }
    if (potion->totalDuration != 0) {
        printf("for %d turns", potion->totalDuration);
    }
    printf(".\n");
    printf("Cost : %d gold\n", potion->goldValue);

    printf("Buy (B) or go onto the next potion (N) ? ");
    return n;
}

/* manages a buying interface for a certain type of item:
 * weapons for 'W'
 * armors for 'A'
 * potions for 'P'
 */
void tavernBuyStuff(struct Player *player, char C) {
    int i = 1;
    int n = 0;
    int flag = 1;
    char *userInput = '0';

    while (flag) {
        switch (C) {
            case 'W':
                i = printWeaponBuy(i);
                break;
            case 'A':
                i = printArmorBuy(i);
                break;
            case 'P':
                i = printPotionBuy(i);
                break;
        }
        /* if the print function returned 0, there are no more items of the given type
         * skips to the end */
        if (i == 0) {
            goto noMoreLabel;
        }

        fflush(stdin);
        scanf("%c", &userInput);
        if (userInput == 'B' || userInput == 'b') {
            if (C == 'W' || C == 'A') {
                struct StuffItem *item = returnListElementItem(getItems(), i);
                dlistItems_append(player->playerInventory, *item);
                player->gold -= item->goldValue;
                printf("%s has been added to your inventory!\n", returnListElementItem(getItems(), i)->name);
                printf("You have %d gold left.\n", player->gold);
            } else {
                struct UsableItem *potion = returnListElementUsable(getUsable(), i);
                dlistUsable_append(player->playerPotions, *potion);
                player->gold -= potion->goldValue;
                printf("%s has been added to your potions!\n", potion->name);
                printf("You have %d gold left.\n", player->gold);
            }
            printf("(Press (ENTER) to continue)");
            fflush(stdin);
            enterToContinue();
            i++;
        } else if (userInput == 'N' || userInput == 'n') {
            i++;
        } else if (userInput == 'Q' || userInput == 'q') {
            return 0;
        }
    }
    noMoreLabel:
    system("cls");
    printf("I dont have any more items to sell, young man.\n");
    printf("(Press (ENTER) to go back to the Buy menu) ");
    fflush(stdin);
    enterToContinue();
    tavernBuy(player);
}

/* menu for the player to choose what he wants to buy */
void tavernBuy(struct Player *player) {
    char *userChoice = '0';

    system("cls");
    printf("I own many items I have been collecting over the years,  travelling around the world.\n");
    printf("Weapons (W),  armors (A), potions (P), what would you like to buy ?\n");
    printf("(Press (Q) to go back to the previous menu) ");
    fflush(stdin);
    scanf("%c", &userChoice);
    if (userChoice == 'W' || userChoice == 'w') {
        tavernBuyStuff(player, 'W');
    } else if (userChoice == 'A' || userChoice == 'a') {
        tavernBuyStuff(player, 'A');
    } else if (userChoice == 'P' || userChoice == 'p') {
        tavernBuyStuff(player, 'P');
    } else if (userChoice == 'Q' || userChoice == 'q') {
        tavernTrade(player);
    }
}

/* Displays a item's information for a sale */
void printItemSell(struct Player *player, int n) {
    if (n == dlistItems_length(player->playerInventory) + 1) {
        return 0;
    }
    system("cls");
    struct StuffItem *item = returnListElementItem(player->playerInventory, n);
    printf("Name: %s\n\n", item->name);
    printf("Attack: %d \t\t Absolute Defense: %d\nHealth points: %d \t Relative Defense: %d\n\n",
           item->attack, item->absoluteDefense, item->hp, item->relativeDefense);
    printf("Sell value: %d gold\n", (int) (item->goldValue * 0.7));

    printf("Sell (S) or go onto the next item (N) ? ");
}

/* Displays a potion's information for a sale */
void printPotionSell(struct Player *player, int n) {
    int previous = 0;
    if (n == dlistUsable_length(player->playerPotions) + 1) {
        return 0;
    }
    struct UsableItem *potion = returnListElementUsable(player->playerPotions, n);
    system("cls");
    printf("Name: %s\n\n", potion->name);
    printf("This potion gives");
    if (potion->hp != 0) {
        printf(" %d hp ", potion->hp);
        previous = 1;
    } else {
        previous = 0;
    }
    if (potion->attack != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d attack ", potion->attack);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->relativeDefense != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d relative defense ", potion->relativeDefense);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->absoluteDefense != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d absolute defense ", potion->absoluteDefense);
        previous = 1;
    } else {
        previous = 0;
    }

    if (potion->dodge != 0) {
        if (previous) {
            printf("and");
        }
        printf(" %d dodge ", potion->dodge);
        previous = 1;
    } else {
        previous = 0;
    }
    if (potion->totalDuration != 0) {
        printf("for %d turns", potion->totalDuration);
    }
    printf(".\n");
    printf("Sell value: %d gold\n", (int) (potion->goldValue * 0.7));

    printf("Sell (S) or go onto the next potion (N) ? ");
}

/* Manages the sales */
void tavernSellStuff(struct Player *player, int n) {
    char *userInput = '0';
    int i = 0;
    int flag = 1;

    while (flag) {
        switch (n) {
            case 0:
                if (i == dlistItems_length(player->playerInventory)) {
                    goto noMoreLabel;
                }
                printItemSell(player, i);
                break;
            case 1:
                if (i == dlistUsable_length(player->playerPotions)) {
                    goto noMoreLabel;
                }
                printPotionSell(player, i);
                break;
        }
        fflush(stdin);
        scanf("%c", &userInput);
        if (userInput == 'S' || userInput == 's') {
            if (n == 0) {
                struct StuffItem *item = returnListElementItem(player->playerInventory, i);
                dlistItems_remove_id(player->playerInventory, i);
                player->gold += (int) (item->goldValue * 0.7);
                printf("%s has been removed from your inventory!\n", item->name);
                printf("You now have %d gold.\n", player->gold);
            } else {
                struct UsableItem *potion = returnListElementItem(player->playerPotions, i);
                dlistUsable_remove_id(player->playerPotions, i);
                player->gold += (int) (potion->goldValue * 0.7);
                printf("%s has been removed from your potions!\n", potion->name);
                printf("You now have %d gold.\n", player->gold);
            }
            printf("(Press (ENTER) to continue)");
            fflush(stdin);
            enterToContinue();
            i++;
        } else if (userInput == 'N' || userInput == 'n') {
            i++;
        } else if (userInput == 'Q' || userInput == 'q') {
            return 0;
        }

    }
    noMoreLabel:
    system("cls");
    printf("You don't have any more items top sell, young man.\n");
    printf("(Press (ENTER) to go back to the Buy menu) ");
    fflush(stdin);
    enterToContinue();
    tavernSell(player);
}

/* Main sell menu where the player chooses what to sell */
void tavernSell(struct Player *player) {
    char *userChoice = '0';

    system("cls");
    printf("What exactly would you like to sell ?\n");
    printf("Items from your inventory (I) or  your potions (P) ?\n");
    printf("(Press (Q) to go back to the previous menu) ");
    fflush(stdin);
    scanf("%c", &userChoice);
    if (userChoice == 'I' || userChoice == 'i') {
        tavernSellStuff(player, 0);
    } else if (userChoice == 'P' || userChoice == 'p') {
        tavernSellStuff(player, 1);
    } else if (userChoice == 'Q' || userChoice == 'q') {
        tavernTrade(player);
    }
}

/* manages the trading at the tavern */
int tavernTrade(struct Player *player) {
    char *userChoice = '0';

    system("cls");
    printf("Good day, aventurer, \n");
    printf("would you like to see my collection of weapons,  armors and potions (B)\n");
    printf("or do you have some items you would like to sell (S) ?\n");
    printf("(Press (Q) to go back to the tavern menu) ");
    fflush(stdin);
    scanf("%c", &userChoice);
    if (userChoice == 'S' || userChoice == 's') {
        tavernSell(player);
    } else if (userChoice == 'B' || userChoice == 'b') {
        tavernBuy(player);
    } else if (userChoice == 'Q' || userChoice == 'q') {
        return 0;
    }
}

/* prints some cool pictures when the player chooses
 * to rest at the tavern, and sets his hp to full */
void tavernRest(struct Player *player) {
    system("cls");
    int startTimer = clock();

    printf("                           +\n");
    printf("             ---====D                        @\n");
    printf("    o                 ,,*:*::,.__     *\n");
    printf("                 * ,)))*)\))* __.=-.             o\n");
    printf("         |       ,((*(/a a\)-'\n");
    printf("        -O-    ,))))))( = )                     =( =     +\n");
    printf("   +     |    ,(((*(((()-(_              *\n");
    printf("              *))))))/ `*` \ \n");
    printf("              (((*}(//(_|_)\\                        .\n");
    printf("              )*))))\\*).( //            +  .\n");
    printf("     *        (((((*(// , )/                   |\n");
    printf("               ))))))/ / /`                   -O-     @\n");
    printf("   +           `(*((((/ /                      |\n");
    printf("             o  `)))*))`\            *\n");
    printf("                  `((((\ `-.__     ,      .\n");
    printf("        .            *)-._    `--~   `\n");
    printf("    @                +    `~---~~`                *\n");
    printf("                *       .            o         +\n");


    while (clock() < startTimer + 3000) {
    }
    system("cls");
    startTimer = clock();

    printf("                             | \n");
    printf("                     .   |\n");
    printf("                         |\n");
    printf("           \    *        |     *    .  /\n");
    printf("             \        *  |  .        /\n");
    printf("          .    \     ___---___     /    .\n");
    printf("                 \.--         --./\n");
    printf("      ~-_    *  ./               \.   *   _-~\n");
    printf("         ~-_   /                   \   _-~     *\n");
    printf("    *       ~-/                     \-~\n");
    printf("      .      |                       |      .\n");
    printf("          * |                         | *\n");
    printf(" -----------|                         |-----------\n");
    printf("   .        |                         |        .\n");
    printf("         *   |                       | *\n");
    printf("            _-\                     /-_    *\n");
    printf("      .  _-~ . \                   /   ~-_\n");
    printf("      _-~       `\               /'*      ~-_\n");
    printf("     ~           /`--___   ___--'\           ~ \n");
    printf("            *  /        ---     .  \       \n");
    printf("             /     *     |           \ \n");
    printf("           /             |   *         \ \n");
    printf("                      .  |        .\n");
    printf("                         |\n");
    printf("                         | \n");

    while (clock() < startTimer + 3000) {
    }
    system("cls");
    printf("After a long night in a sweet bed and a decent breakfast, \nyou feel rested and full of energy. You are now full hp.\n");
    while (clock() < startTimer + 7000) {
    }
    player->mob->hp = player->mob->mobRace->hp;
}

/* Menu for the player to choose if he wants to leave the game */
void quitGame(struct Player *player) {
    char userChoice = '0';

    system("cls");
    printf("Do you really wish to leave the game ? \nThis will save your current progression and you can come back later to continue your adventure.\n");
    printf("Press (S) to save and exit the game, \n(Q) to exit the game withtout saving \nor (M) to get back the tavern menu. ");
    fflush(stdin);
    scanf("%c", &userChoice);
    if (userChoice == 'S' || userChoice == 's') {
        system("cls");
        printf("Pursuing will delete any game save already existing, are you sure you want to save your progression and leave the game? \n");
        printf("Press (S) to save or (B) to go back to the previous menu. ");
        userChoice = '0';
        fflush(stdin);
        scanf("%c", &userChoice);
        if (userChoice == 'S' || userChoice == 's') {
            int startTimer = clock();
            printf("Saving game");
            while (clock() < startTimer + 1000) {
            }
            printf(".");
            while (clock() < startTimer + 2000) {
            }
            printf(".");
            while (clock() < startTimer + 3000) {
            }
            printf(".\n");
            saveGame(player);
            printf("Your game has been saved.\n");
            printf("(Press enter to exit the console) ");
            enterToContinue();
            exit(0);
        } else if (userChoice == 'B' || userChoice == 'b') {
            quitGame(player);
        }

    } else if (userChoice == 'Q' || userChoice == 'q') {
        userChoice = '0';
        system("cls");
        printf("Are you sure you want to leave without saving your progression ? \n");
        printf("Press (Q) to quit the game or (B) to go back to the previous menu. ");
        fflush(stdin);
        scanf("%c", &userChoice);
        if (userChoice == 'Q' || userChoice == 'q') {
            exit(0);
        } else if (userChoice == 'B' || userChoice == 'b') {
            quitGame(player);
        }
    }
}

/* Manages the choices the player has when he enters the tavern */
void enterTavern(struct Player *player) {
    char *userChoice = '0';
    int flag = 1;

    system("cls");
    printf("As you enter the tavern, a trader approaches.\n");
    while (flag) {
        printf("Do you wish to trade the merchant (T) \ngo upstairs and rest for the night (R) \n");
        printf("manage your inventory (I) \nor leave the tavern and travel to the next dungeon (L) ? \n");
        printf("(Press (Q) to quit the game) ");
        fflush(stdin);
        scanf("%c", &userChoice);
        if (userChoice == 'T' || userChoice == 't') {
            tavernTrade(player);
        } else if (userChoice == 'R' || userChoice == 'r') {
            tavernRest(player);
        } else if (userChoice == 'I' || userChoice == 'i') {
            manageInventory(player);
        } else if (userChoice == 'L' || userChoice == 'l') {
            startDungeon(player);
        } else if (userChoice == 'Q' || userChoice == 'q') {
            quitGame(player);
        }
        system("cls");
    }
}
