#include "myShell.h"
#include "myFunctionsShell.h"

int main(int argc, char const *argv[])
{
    welcome();

    while (1)
    {
        getLocation();

        char *input = getInputFromUser();
        if (!input) {
            continue;
        }

        char **arguments = splitArguments(input);
        if (!arguments) {
            free(input);
            continue;
        }

        if (!arguments[0]) {
            free(arguments);
            free(input);
            continue;
        }

        if (isExitCommand(arguments[0])) {
            logout(input); 
        }

        if (strcmp(arguments[0], "cd") == 0) {
            cd(arguments);
        }
        else if (strcmp(arguments[0], "echo") == 0) {
            echo(arguments);
        }
        else if (strcmp(arguments[0], "cp") == 0) {
            cp(arguments);
        }
        else if (strcmp(arguments[0], "delete") == 0) {
            delete(arguments);
        }
        else if (strcmp(arguments[0], "dir") == 0) {
            get_dir();
        }
        else if (strcmp(arguments[0], "mv") == 0) {
            move(arguments);
        }
        else if (strcmp(arguments[0], "read") == 0) {
            _read(arguments);
        }
        else if (strcmp(arguments[0], "wc") == 0) {
            wordCount(arguments);
        }
        else {
            int pipeIndex = -1;
            for (int i = 0; arguments[i]; i++) {
                if (strcmp(arguments[i], "|") == 0) {
                    pipeIndex = i;
                    break;
                }
            }
            if (pipeIndex != -1) {
                char **argv1 = malloc(sizeof(char*) * (pipeIndex + 1));
                if (!argv1) {
                    free(arguments);
                    free(input);
                    continue;
                }
                for (int k = 0; k < pipeIndex; k++) {
                    argv1[k] = arguments[k];
                }
                argv1[pipeIndex] = NULL;

                int len2 = 0;
                while (arguments[pipeIndex + 1 + len2]) {
                    len2++;
                }
                char **argv2 = malloc(sizeof(char*) * (len2 + 1));
                if (!argv2) {
                    free(argv1);
                    free(arguments);
                    free(input);
                    continue;
                }
                for (int m = 0; m < len2; m++) {
                    argv2[m] = arguments[pipeIndex + 1 + m];
                }
                argv2[len2] = NULL;

                mypipe(argv1, argv2);

                free(argv1);
                free(argv2);
            }
            else {
                systemCall(arguments);
                wait(NULL);
            }
        }

        free(arguments);
        free(input);
    }

    return 0;
}

void welcome()
{
    const char *logo[] = {
        "                         | ___  o|     |\n",
        "                         |[_-_]_ |     |\n",
        "      ______________     |[_____]|     |     Kfir Moscovich\n",
        "     |.------------.|    |[====o]|     | \n",
        "     ||            ||    |[_.--_]|     |     ./myShell\n",
        "     ||            ||    |[_____]|     |\n",
        "     ||            ||    |      :|     |\n",
        "     ||            ||    |      :|     |\n",
        "     ||____________||    |      :|     |\n",
        " .==.|\"\"  ......    |.==.|      :|     |\n",
        " |::| '-.________.-' |::||      :|     |\n",
        " |''|  (__________)-.|''||______:|     |\n",
        " `\"\"`_.............._\\\"\"`______        |\n",
        "    /:::::::::::'':::\\`;'-.-.  `\\      |\n",
        "   /::=========.:.-::\"\\ \\ \\--\\   \\     |\n",
        "   \\`\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"`/  \\ \\__)   \\    |\n",
        "___ `\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"`    '========'   |    Welcome. have fun!\n",
        NULL
    };
    

    for (int i = 0; logo[i] != NULL; i++) {
        printf("%s", logo[i]);
    }
    puts("");
}
