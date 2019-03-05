
/*****************************************
# Name: CS344 Program 3
# Date: March 2019
# Author: Adam Walker
# Description: smallsh assignment: Create
# a simple shell in C, handling 3 built-in
# commands (cd, exit, status), with others
# run using execvp()
*****************************************/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>

// Maxiumum of 512 arguments allowed
#define maxArgs 512

// Using a global int for foreground mode as it needs to be edited by signal handler
static int foregroundOnlyMode = 0;


// Function to set and print process exit status
void myStatus(int* exitStatus, int newExitStatus, int setStatus, int* signalTerm, int newSignalTerm, int setSignal){

  // Terminated normally
  if( setStatus ){

    // Set exit status, and set signal termination value to zero
    *exitStatus = newExitStatus;
    *signalTerm = 0;

  // Terminated by signal
  } else if ( setSignal ){

    // Set signal termination value, set exit status to zero
    *signalTerm = newSignalTerm;
    *exitStatus = 0;

  }

  // If call is not setting the exit status, then print exit status of last foreground process run
  // Using signal termination value to choose appropriate print message
  if( setStatus==0 & setSignal==0 ){

    if( *signalTerm == 0 ){

      printf("exit value %d\n",*exitStatus);
   
    } else {

      printf("terminated by signal %d\n",*signalTerm);

    }

  }

}


// Function to run non-built in commands using execvp
// Also passes additional info, including whether stdin and stdout used, int to store exit status, 
// whether process should be run in the foreground, and an int to store signal termination number
// runCommand returns pid of child process created
pid_t runCommand( char* commandArgs[], int argNum, int stdOut, int stdIn, int* exitStatus, int foreground, int* signalTerm ){

  pid_t spawnPid = -5;
  int childExitStatus = -5;

  // Fork parent
  spawnPid = fork();

  // Using switch structure specified in course notes
  switch (spawnPid) {

    case -1: { perror("Hull Breach!\n"); exit(1); break; }

    case 0: {

            // Check for stdout
            // If command is using stdout, set-up necessary redirect with dup2()
            if( stdOut>0 ){

	      int targetFD, result;

              // Open stdout loation
              targetFD = open(commandArgs[(stdOut+1)], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	      if (targetFD == -1) { perror("target open()"); exit(1); }

              // Redirecting stdout...
              result = dup2(targetFD, 1);
	      if (result == -1) { perror("target dup2()"); exit(2); }

              // Nullify args
              // This stops stdout args being included in execvp call
              commandArgs[stdOut] = NULL;
              commandArgs[(stdOut+1)] = NULL;

            }
            
            // Check for stdin
            if( stdIn>0 ){

	      int sourceFD, result;

	      // Open stdin location. Print informative error message if successful
	      sourceFD = open(commandArgs[(stdIn+1)], O_RDONLY);
              if (sourceFD == -1) { printf( "cannot open %s for input\n", commandArgs[(stdIn+1)] ); exit(1); }

              // Redirecting stdin...
              result = dup2(sourceFD, 0);
              if (result == -1) { perror("oops, didn't work..."); exit(2); }

              // Nullify args
              // This stops stdin args being included in execvp call
              commandArgs[stdIn] = NULL;
              commandArgs[(stdIn+1)] = NULL;

            }
            
            // Redirect background processes to /dev/null
            // Stops output being printed to terminal
            if( foreground==0 & stdOut==0 ){
	      
              int targetFD, result;
              
              targetFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	      if (targetFD == -1) { perror("target open()"); exit(1); }

              // Redirecting stdout...
              result = dup2(targetFD, 1);
	      if (result == -1) { perror("target dup2()"); exit(2); }

            }
 
            // Copy commandArgs over to commandArgsCopy
            // This allows adding the NULL argument at the end
            char *commandArgsCopy[maxArgs];
            
            int a;
            for (a = 0; a < (argNum+1); a++){
             commandArgsCopy[a] = commandArgs[a]; // Copying args over
            }
            commandArgsCopy[(a+1)] = NULL;
            // Add the null ending

            // Run command using execvp
            execvp(commandArgsCopy[0], commandArgsCopy); 
            perror(commandArgsCopy[0]);
            exit(2); break;
         
    }
    default: {

             if( foreground ){

               // For foreground processes, wait until completed
               pid_t actualPid = waitpid(spawnPid, &childExitStatus, 0);
      
                // Check if process exited normally
                if ( WIFEXITED(childExitStatus) ) {

        	    // Process exited normally. Retrieve exit status and update status with value
       	 	    int childExitStatusInt = WEXITSTATUS(childExitStatus);
                    // Set exit status
                    myStatus(exitStatus, childExitStatusInt, 1, signalTerm, 0, 0);

      		  } else {

        	    // Process was terminated by signal
        	    // Retreive signal number and update status with value
        	    // Also print a message informing user of signal termination 
                    int newTermSignal = WTERMSIG(childExitStatus);
                    // Set signal
                    myStatus(exitStatus, 0, 0, signalTerm, newTermSignal, 1);
        	    printf("terminated by signal %d\n", newTermSignal);

      		  }

             } else {

               // For background processes, output the pid and run in background
               printf("background pid is %d\n",spawnPid);
               pid_t hasBackgroundFinished = waitpid(spawnPid, &childExitStatus, WNOHANG);

             }

      // Set all commandArgs to empty string
      // Lets commandArgs be easily re-used for each command
      int j;
      for (j = 0; j < (argNum+1); j++){

        strcpy(commandArgs[j], "");

      }
      
    }

  }

  return spawnPid;

}


// Built-in cd command
// commandArgs used to provide destination directory
// argNum says whether user provided multiple arguments. If not, set dir to home
void myCd(char* commandArgs[], int argNum){

  // Stores return int from chdir()
  int changeDir;

  // No arguments provided, so set directory to home
  if(argNum==0){

    changeDir = chdir(getenv("HOME"));

  } else {
    
    // Reference: https://piazza.com/class/jqheoh1h3kl11a?cid=207
    // Reference: https://stackoverflow.com/questions/1293660/is-there-any-way-to-change-directory-using-c-language
    // Reference: https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory-exists
    
    // Get working directory
    char *newDir;
    newDir = malloc( 2048 * sizeof(char) );

    // Check if absolute or relative directory
    DIR* dir = opendir(commandArgs[1]);
    if (dir)
    {
      // Directory exists
      // Absolute reference therefore provided
      closedir(dir);
      strcpy(newDir, commandArgs[1]);

    } else if (ENOENT == errno) {

      // Directory does not exist
      // Relative reference provided
      strcpy(newDir, getcwd(NULL, 0));
      strcat(newDir, "/");
      strcat(newDir, commandArgs[1]);
    
    }

    changeDir = chdir(newDir);
    free(newDir);

  }

}


// Adds a single character to string
// Provide arrayBlock (string), the character to add, and whether the character is the first of word
void addCharacter(char* arrayBlock, char charToAdd, int firstOfWord){

         // Reference: https://stackoverflow.com/questions/4834811/strcat-concat-a-char-onto-a-string
         char cToStr[2];
         cToStr[1] = '\0';
         cToStr[0] = charToAdd;
         
         // Copy if first letter in word, otherwise append
         if( firstOfWord ){

           strcpy(arrayBlock, cToStr); // Copy

         } else {

           strcat(arrayBlock, cToStr); // Append

         }

}


// Signal handler for SIGINT
// Found that no messaging was needed for this assignment
void catchSIGINT(int signo)
{

}


// Signal handler for SIGTSTP
// Switches foreground mode on/off each time received, and prints message
void catchSIGTSTP(int signo)
{

  // Switch foreground mode between 1 and 0
  foregroundOnlyMode = 1 - foregroundOnlyMode;

  // Print message informing user of new foreground mode value
  if( foregroundOnlyMode == 1 ){
    
    char* message = "\nEntering foreground-only mode (& is now ignored)\n";
    write(STDOUT_FILENO, message, 50);

  } else {

    char* message = "\nExiting foreground-only mode\n";
    write(STDOUT_FILENO, message, 30);

  }

}



int main()
{

  // Set up signal handlers
  struct sigaction SIGINT_action = {0}, SIGTSTP_action = {0};

  // Assigns handler functions
  SIGINT_action.sa_handler = catchSIGINT;
  SIGTSTP_action.sa_handler = catchSIGTSTP;
  sigfillset(&SIGINT_action.sa_mask);
  sigfillset(&SIGTSTP_action.sa_mask);

  // https://stackoverflow.com/questions/19140892/strange-sigaction-and-getline-interaction
  // Found that SA_RESTART was needed to prevent seg faults
  SIGINT_action.sa_flags = SA_RESTART;
  SIGTSTP_action.sa_flags = SA_RESTART;

  // Assign signals
  sigaction(SIGINT, &SIGINT_action, NULL);
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);

  // Array to track all background pids
  // Use an int array to track whether each pid has finished
  // myPidTracker denotes length of filled array
  pid_t myPids[1000];
  int finished[1000];
  int myPidTracker=0;
  // Fill finished array with zeroes
  int f;
  for (f = 0; f < 1000; f++) {
   finished[f] = 0;
  } 

  // Returned when command is run
  pid_t spawnPid;

  // Used to check status of background processes
  int childExitStatus = -5;
  int backgroundExitStatusInt;
 
  // Indicates whether command should be run in foreground 
  int foreground;

  // Tracks number of commands being run
  // Used to prevent fork bombs
  int commandCounter = 0;

  // Exit status of last foreground process
  int exitStatus = 0;
  // Was it killed by signal?
  int signalTerm = 0;
  
  // Track command arguments in character array
  char *commandArgs[maxArgs];
  int a;
  for (a = 0; a < maxArgs; a++) {
   commandArgs[a] = malloc( 2048 * sizeof(char) );
  }

  // Keep running until specified number of commands run
  // Used to prevent fork bombs
  while( commandCounter<1000 ){

    // Set each command to a foreground process initially
    // Turn off if & found at end of command and foreground mode is turned off
    foreground = 1;
  
    commandCounter++; // Command counter

    // Check if background pids have finished
    for(f = 0; f < myPidTracker; f++){ 
   
      // Only check the pids not yet marked as completed  
      if( finished[f]==0 ){

        pid_t hasBackgroundFinished = waitpid(myPids[f], &childExitStatus, WNOHANG);
     
        if( hasBackgroundFinished ){

          // Check if exited normally
          if( WIFEXITED( childExitStatus ) ){

            // Print exit status upon normal termination
            backgroundExitStatusInt = WEXITSTATUS(childExitStatus);
            printf("background pid %d is done: exit value %d\n", myPids[f], backgroundExitStatusInt);

          } else {

            // Terminated by signal
            // Print signal number
            backgroundExitStatusInt = WTERMSIG(childExitStatus);
            printf("background pid %d is done: terminated by signal %d\n", myPids[f], backgroundExitStatusInt);

          } 
           
          // Mark background process as finished
          finished[f] = 1;

        }

      }

   }

  // Print command prompt
  printf(": ");
  fflush(stdout);
  
  // Get user input using getline()
  char* lineEntered = NULL;
  size_t bufferSize = 0;
  int numCharsEntered = -5;
  numCharsEntered = getline(&lineEntered, &bufferSize, stdin); 
  lineEntered[strlen(lineEntered)-1] = 0;

  // Check first character entered
  // Used to identify comments and new lines
  char firstChar = lineEntered[0];
 
  // Comment found
  if( firstChar=='#'  ){

    printf("\n");

  // New line
  } else if ( strlen(lineEntered)==0 ) {

    // Do nothing, just go back to start

  } else {

      // A non-comment command has been found

      int commandStatus;

      // TOKENIZER: parse the command into arguments for execvp(), or built-in command
      
      int i;  // Interate through input string
      int argNum = 0; // Track number of arguments entered
      int dollarSign = 0; // Was the last character entered a dollar sign?
      int stdIn = 0; // Argument position of <
      int stdOut = 0; // Argument position of >
      int firstOfWord = 1; // Is character first letter of argument?
      
      // Loop through entered string
      for (i = 0; i < strlen(lineEntered); i++) {

        // Move to next argument when space is found
        if( lineEntered[i]==' ' ){

          // Update argument count, know that next character will be first of next argument
          argNum++;
          firstOfWord = 1;

        } else {

         // stdout character found
         if( lineEntered[i]=='>' ){

           // Set index of location
           stdOut = argNum;
           firstOfWord=0;

	 }
         
         // stdin character found
         if( lineEntered[i]=='<' ){

           // Set index of location
           stdIn = argNum;
           firstOfWord=0;

	 }

         // Dollar sign found
         if( lineEntered[i]=='$' ){

           // If dollarSign is set, the character is second dollar sign in a row
           // Will therefore replace argument with pid
           if( dollarSign==1 ){

             // Get PID and add it
             // Reference: https://stackoverflow.com/questions/15262315/how-to-convert-pid-t-to-string
             char pid[10];
             snprintf(pid, 10,"%d",(int)getpid());
             
             // Add pid as argument
             int p;
             for (p = 0; p < strlen(pid); p++) {

               addCharacter(commandArgs[argNum], pid[p], firstOfWord);
               firstOfWord=0;
             
             }

	     // Reset dollar sign
	     dollarSign=0;             

           } else {

             // First dollar sign found, so set variable
             dollarSign = 1;
           
           }

         // Ampersand found, indicating background process
         } else if ( lineEntered[i]=='&' ){

           // Only make background if foreground only mode turned off, AND & is last character entered
           if( foregroundOnlyMode==0 & (i+1)==strlen(lineEntered) ) foreground = 0;
           // Reduce args by 1...unless the & found in echo
           if ( strcmp(commandArgs[0],"echo") != 0){

             argNum--;

           } else {
             
             // Since part of echo, add to commandArgs
             addCharacter(commandArgs[argNum], lineEntered[i], firstOfWord);
             firstOfWord=0;

           }

	 } else {

         // Character is not a dollar sign, so reset flag
         dollarSign = 0;         

         // Add character to argument
         // Set first word character to zero
         addCharacter(commandArgs[argNum], lineEntered[i], firstOfWord);
         firstOfWord=0;

         }

       }

      }

      // Input string from user has now been tokenized and stored in commandArgs
      // Now run command

      // Check if one of three built-in commands
      if( strcmp(commandArgs[0],"status") == 0 ){

        // Built-in status command
        // Provide ints to track exit status and signal termination number
        myStatus(&exitStatus, 0, 0, &signalTerm, 0, 0);

      } else if ( strcmp(commandArgs[0],"exit") == 0 ){

	  // BUILT IN EXIT COMMAND

	  // Kill any background processes
	   for(f = 0; f < myPidTracker; f++){ 
	    
             // Only check processes marked as unfinished 
	     if( finished[f]==0 ){

	       pid_t hasBackgroundFinished = waitpid(myPids[f], &childExitStatus, WNOHANG);
	     
               // Kill any processes that are yet to finish
	       if( !hasBackgroundFinished ){

                 // Reference: https://www.linuxquestions.org/questions/programming-9/c-program-to-kill-process-4175444948/
                 kill(myPids[f],SIGKILL);

	       }

	     }

	   }

	  // Free memory allocated for commandArgs
	  int freeMem;
	  for (freeMem = 0; freeMem < maxArgs; freeMem++) {
            free(commandArgs[freeMem]);
	  }

          // Exit shell
          exit(0);
      
      } else if ( strcmp(commandArgs[0],"cd") == 0 ){
      
        // Built-in cd command
        // Pass it commandArgs and number of arguments
        // Uses these to figure out if absolute, relative, or no reference has been provided
        myCd(commandArgs, argNum); 

      } else {

        // Run command using commandArgs
        spawnPid = runCommand( commandArgs, argNum, stdOut, stdIn, &exitStatus, foreground, &signalTerm );
   
        // Store background processes in array that can be checked later
        if( foreground==0 ){

          myPids[myPidTracker] =spawnPid; 
          myPidTracker++;
     
        } 
               
      }

    }

  }
 
  return 0;

}
