// processlab.c

#include "libprocesslab.h"
#include "part1.h"
#include "part2.h"


int main(int argc, char*argv[])
{
    if(argc!=2)
    { perror("ligne de commande attendue : ./processlab n , n = 1, 2 ou 3\n");
      _exit(1);
    }
    int questionNum = atoi(argv[1]);
    switch (questionNum) {
        case 1:
            question1(1);
            break;
        case 2:
            question1(2);
            break;
        case 3:
            question2(1);
        default:
            break;
    }
  return 0;

}
