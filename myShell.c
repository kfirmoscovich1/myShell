#include "myShell.h"
#include "myFunctionsShell.h"

int main(int argc, char const *argv[])
{

    welcome();
    while (1)
    {
        int isPipe = 2;
        int isEchoWrith = 0;
        int isEchoPpend = 0;

        getLocation();

        char *input = getInputFromUser();
        puts(input);
        
        if (strncmp(input, "exit",4) == 0)
        {
            puts("Exit");
            logout(input);
        }

        char **arguments = splitArguments(input);

        if (strcmp(input, "cd") == 0)
        {
            cd(arguments);
        }
        else if (strncmp(input, "echo",4) == 0)
        {
            if (isEchoWrith)
                echowrite(arguments);
            else if (isEchoPpend)
                echoppend(arguments);
            else
                echo(arguments);
        }
        else if (strcmp(input, "cp") == 0)
        {
            cp(arguments);
        }
        else if (strcmp(input, "delete") == 0)
        {
            delete(arguments);
        }
        else if (strcmp(input, "dir") == 0)
        {
            get_dir();
        }
        else if (isPipe)
        {
            mypipe(arguments, arguments + isPipe + 1);
        }
        else
        {
            systemCall(arguments);
            wait(NULL);
        }

        free(input);
    }

    return 0;
}
void welcome()
{
    char *logo[] = {"                     aRTI//BARK             ____\n",
                    "             simpleSH//////////Shl         |B//S|", "    |\n",
                    "          SH//////YS           shell//Sh   |A@@I|", "    | Welcome to simple shell\n",
                    " Sim aSystemSH//Sh              sys//B     |R$$M|", "    | Version 1.0\n",
                    " ArtABBBaraKk///Sh               sM//E     |A^^P|", "    |\n",
                    "         pCCCCY//h          eSS@@ y//E     |K**L|", "    | https://github.com/BSharabi\n",
                    "         SPPPP///a          pP///AC//E     |&&&E|", "    |\n",
                    "              A//A            smP////S     |A**S|", "    | Have fun!\n",
                    "              p///Ac            sE///a     |R##H|", "    |\n",
                    "              P////YCpc           L//L     |T!!E|", "    | Wanna support simple shell?\n",
                    "       scccccp///pSP///p          p//l     |I%%L|", "    |\n",
                    "      sY/////////y  caa           S//h     |U--L|", "    |\n",
                    "       shsSellH//Ya              pY/Sh     |MIGT|", "    |\n",
                    "        sH/ShS////YCc          aC//Yp      |____|", "    |\n",
                    "         Si  shlll//SHsimpleSH//LSs\n",
                    "                  shell//////IPSHs\n",
                    "                       artium\n", '\0'};
    int i = 0;

    do
    {
        printf("%s", logo[i++]);
        // if (i % 2 != 0 || i > 26)
        //     green();
        // else
        //     blue();
    } while (logo[i]);
    // reset();
    puts("\n");
}