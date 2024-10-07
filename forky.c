#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void pattern1(int things, FILE *fp);
void pattern2(int things, FILE *fp);
int random_sleep_time();

int main(int argc, char *argv[]) {
    int things = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    srand(time(NULL));

    FILE *fp = fopen("results.txt", "a");

    if (pattern == 1){
        fprintf(fp, "\nRunning Pattern 1:\n");
        fflush(fp);  // attempt to flush to avoid repetition after forking
        pattern1(things, fp);
    } else if (pattern == 2){
        fprintf(fp, "\nRunning Pattern 2:\n");
        fflush(fp);  
        pattern2(things, fp);
    }

    fclose(fp);

    return 0;
}

void pattern1(int things, FILE *fp) {
    fprintf(fp, "** Pattern 1: Creating %d processes\n", things);
    fflush(fp);  

    pid_t pids[things]; 

    for (int i = 0; i < things; i++){
        pids[i] = fork();

        if (pids[i] == 0) { 
            int sleep_time = random_sleep_time();
            fprintf(fp, "Process %d (pid %d) created: will sleep for %d seconds\n", i, getpid(), sleep_time);
            fflush(fp);  

            sleep(sleep_time);

            fprintf(fp, "Process %d (pid %d): exiting\n", i, getpid());
            fflush(fp);  

            exit(0); 
        } else if (pids[i] > 0) { 
            fprintf(fp, "Parent: created child %d (pid %d)\n", i, pids[i]);
            fflush(fp);
        } 
        else {
            perror("Fork failed");

            exit(1); 
        }
    }
    fprintf(fp, "** Pattern 1: All processes created\n");
    
    fflush(fp);  

    for (int i = 0; i < things; i++){
        waitpid(pids[i], NULL, 0);
    }
    fprintf(fp, "** Pattern 1: All children have exited\n");

    fflush(fp); 
}

void pattern2(int things, FILE *fp) {
    fprintf(fp, "** Pattern 2: Creating %d processes\n", things);
    fflush(fp);  

    pid_t pids[things];

    for (int i = 0; i < things; i++){
        pids[i] = fork(); 

        if (pids[i] == 0) { 
            int sleep_time = random_sleep_time(); 

            fprintf(fp, "Child %d (pid %d): starting\n", i, getpid());
            fflush(fp); 
            sleep(sleep_time); 

            fprintf(fp, "Child %d (pid %d): exiting after sleeping for %d seconds.\n", i, getpid(), sleep_time);
            fflush(fp); 

            exit(0);
        } 
        else { 
            fprintf(fp, "Parent: created child %d (pid %d)\n", i, pids[i]);

            fflush(fp); 
        }
    }

    for (int i = 0; i < things; i++){
        waitpid(pids[i], NULL, 0);  
    }

    fprintf(fp, "** Pattern 2: All children have exited\n");

    fflush(fp); 
}

int random_sleep_time() {
    return rand() % 8 + 1; 
}
