
/*****************************************
# Name: CS344 Program 2
# Date: February 2019
# Author: Adam Walker
# Description: Adventure game player
*****************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// Struct representing room within game
struct room
{
  int startRoom; // Flag if room is starting room
  int endRoom; // Flag if room is ending room
  char* name; // Room name
  int numOutboundConnections; // Number of outbound connections
  struct room* outboundConnections[6]; // Array holding pointers to each of room's connections (between 3 and 6)
};

// Reference: TLPI, Chapters 29 and 30
// Initialize mutex
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


// Function to write current date and time to a file
// Uses mutex lock/unlock
void* writeTime(void *arg)
{

 int l;
 l = pthread_mutex_lock(&mtx); // Lock mutex

 // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
 time_t rawtime;
 struct tm *info;
 char buffer[120];

 time( &rawtime );
 info = localtime( &rawtime );

 // Format time
 strftime(buffer,120,"%l:%M%P, %A, %B %d, %Y", info);

 // Character array for file name          
 char* myFile;
 myFile = malloc(50 * sizeof(char));

 if (myFile == 0)
   printf("malloc() failed!\n");
 memset(myFile, '\0', 50);

 sprintf(myFile,"currentTime.txt");

 // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
 FILE * fp;
 fp = fopen(myFile, "w+");

 // Write the time
 fprintf(fp, "%s", buffer);

 // Close file connection
 fclose(fp);

 // Avoid memory leaks
 free(myFile);

 l = pthread_mutex_unlock(&mtx); // Unlock

}


// Function to read and print time from a file
void printTime()
{
 
 // Open file
 char* myFile;
 myFile = malloc(50 * sizeof(char));

 if (myFile == 0)
   printf("malloc() failed!\n");
 memset(myFile, '\0', 50);

 sprintf(myFile,"./currentTime.txt");

 // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
 FILE * fp;
 fp = fopen(myFile, "r");
 char buf[256];

 // Get datetime from file and print
 fgets(buf, 256, fp);
 printf("\n%s\n\n", buf); 

 fclose(fp);
 free(myFile);
 
}


// Connects rooms by matching room name to a character array
// roomNum is the index of a room within myRooms. connectShort is a char array representing room to match with
void makeConnection( struct room myRooms[], int roomNumsLength, int roomNum, char* connectShort ){

  // Loop through each room in myRooms
  // Check if name matches connectShort. If so, make the connection with roomNum room
  int i;
  for (i = 0; i < roomNumsLength; i++) { 
          
    // Match found if true
    if( strcmp( myRooms[i].name, connectShort ) == 0 ){

      // Adding connection
      int connectNum = myRooms[roomNum].numOutboundConnections;
      myRooms[roomNum].outboundConnections[connectNum] = &myRooms[i];
      myRooms[roomNum].numOutboundConnections++;

    }

  } 	

};


// Prints game menu to player during each step of game
// currentLocation is struct representing player's current room
void printOptions( struct room currentLocation ){

  printf("CURRENT LOCATION: %s\n",currentLocation.name);

  // Loop through room's outbound connections to generate player's move options
  printf("POSSIBLE CONNECTIONS: ");
  int i;
  for (i = 0; i < currentLocation.numOutboundConnections; i++) { 

    printf("%s",currentLocation.outboundConnections[i]->name);
    if( (i+1) != currentLocation.numOutboundConnections) printf(", ");          
    if( (i+1) == currentLocation.numOutboundConnections) printf(".\n"); // Get correct punctuation for list of connecting rooms         

  } 	

  printf("WHERE TO? >");

};


// Updated the player's location (if possible) based on their character input
void updateLocation( struct room* currentLocation, char* lineEntered, int* steps ){

  int goodStep = 0; // Used to track if error message should be sent

  // Loop through the current room's outbound connections
  // Check if the string player entered is a match with any of the names
  int i;
  for (i = 0; i < currentLocation->numOutboundConnections; i++) { 

    // A match if true
    if( strcmp( currentLocation->outboundConnections[i]->name, lineEntered) == 0 ){

      // Update current location by copying over details of connecting room
      struct room tempRoom;
      tempRoom = *(currentLocation->outboundConnections[i]); 
      currentLocation->startRoom = tempRoom.startRoom;
      currentLocation->endRoom = tempRoom.endRoom;
      currentLocation->name = tempRoom.name;
      currentLocation->numOutboundConnections = tempRoom.numOutboundConnections;

      // Copying over the new room's outbound connections
      int j;
      for (j = 0; j < 6; j++) { 
        currentLocation->outboundConnections[j] = tempRoom.outboundConnections[j];
      }

      // Update step count since player successfully entered a connecting room
      (*steps)++;
      goodStep = 1;

    }

  }

  // No match was found, so return error message
  if( !goodStep ) printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n"); 	

};


// Function to play out the game
// Player moves from room to room until they find the end room. At that point game is over
void playGame( struct room myRooms[], int roomNumsLength, pthread_t *timeKeeper ){

  // Struct to hold player's current location
  struct room currentLocation;
  int s;

  // Populate current location by finding the start room using startRoom flag
  int i;
  for (i = 0; i < roomNumsLength; i++) { 
          
    if( myRooms[i].startRoom ){

      currentLocation = myRooms[i];

    }

  } 

  // Reference: https://stackoverflow.com/questions/1088622/how-do-i-create-an-array-of-strings-in-c
  // Used to track name of each room player has visited
  // Very conservatively allowing 500 steps...any player should be able to finish game in this time!
  const char *stepTracker[500];

  int steps = 0;
  while( currentLocation.endRoom != 1 ){

    // Print menu
    printOptions( currentLocation );
  
    // Get user input
    char* lineEntered = NULL;
    size_t bufferSize = 0;
    int numCharsEntered = -5;
    numCharsEntered = getline(&lineEntered, &bufferSize, stdin); 
    lineEntered[strlen(lineEntered)-1] = 0;
    
    // Write and print time
    if( strcmp(lineEntered,"time")==0 ){

      s = pthread_join(*timeKeeper, NULL); // Run timekeeper thread until done
      printTime();

    } else {

      // Update player's location. If player has correctly entered a connecting room name, they will move into it
      printf("\n");
      updateLocation( &currentLocation, lineEntered, &steps );

    }
    
    // Track the room a player is in during each step
    if(steps>0) stepTracker[(steps-1)] = currentLocation.name;

    // Avoid memory leak
    free(lineEntered);
    lineEntered = NULL;

  }

  // Game is over. Print victory message
  printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");

  // Print the steps taken
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n",steps);  
  for(i=0; i<steps; i++){

    printf("%s\n", stepTracker[i]);

  }

};


// Read in room files, set up room struct array, then play adventure game
int main()
{

  pthread_t timeKeeper;
  int s;
  s = pthread_create(&timeKeeper, NULL, writeTime, NULL);

  // Find most recently created directory
  // Reference: https://oregonstate.instructure.com/courses/1706555/pages/2-dot-4-manipulating-directories
  int newestDirTime = -1; // Modified timestamp of newest subdir examined
  char targetDirPrefix[32] = "walkerad.rooms."; // Prefix we're looking for
  char newestDirName[256]; // Holds the name of the newest dir that contains prefix
  memset(newestDirName, '\0', sizeof(newestDirName));

  DIR* dirToCheck; // Holds the directory we're starting in
  struct dirent *fileInDir; // Holds the current subdir of the starting dir
  struct stat dirAttributes; // Holds information we've gained about subdir

  dirToCheck = opendir("."); // Open up the directory this program was run in

  if (dirToCheck > 0) // Make sure the current directory could be opened
  {
    while ((fileInDir = readdir(dirToCheck)) != NULL) // Check each entry in dir
    {
      if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If entry has prefix
      {
        stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

        if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
        {
          newestDirTime = (int)dirAttributes.st_mtime;
          memset(newestDirName, '\0', sizeof(newestDirName));
          strcpy(newestDirName, fileInDir->d_name);
        }
      }
    }
  }

  closedir(dirToCheck); // Close the directory we opened


  // Set-up array of room structs to hold details of rooms in the game
  int roomNumsLength = 7;
  struct room myRooms[roomNumsLength];

  // Initialize some array struct variables
  int i;
  for (i = 0; i < roomNumsLength; i++) { 
          
    myRooms[i].numOutboundConnections = 0;
    myRooms[i].startRoom = 0;
    myRooms[i].endRoom = 0;
 
  } 	


  // Read in room name and types
  // Reference: https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
  struct dirent *de;  // Pointer for directory entry 
  DIR *dr = opendir(newestDirName);
  int roomNum = 0;   
  while ((de = readdir(dr)) != NULL){

    // https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
    if(strstr(de->d_name, "room") != NULL) {

          // Open file 
	  char* myFile;
          myFile = malloc(50 * sizeof(char));

	  if (myFile == 0)
	    printf("malloc() failed!\n");
	  memset(myFile, '\0', 50);

	  sprintf(myFile,"%s%s%s",newestDirName,"/",de->d_name);

	  // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
          FILE * fp;
	  fp = fopen(myFile, "r");
	  char buf[256];

	  // Get room name and type
	  while( !feof(fp) ){

    	    fgets(buf, 256, fp); 

	    // Get name of room
            if( strstr(buf, "ROOM NAME") != NULL ){

	      // Reference: https://stackoverflow.com/questions/1726298/strip-first-and-last-character-from-c-string
              char* roomName = malloc(10 * sizeof(char));
	      sprintf(roomName, buf + 11);
              roomName[strlen(roomName)-1] = 0;
              myRooms[roomNum].name = roomName;

	    }

	    // Check if start or end room, and update struct flags when found
            if( strstr(buf, "START_ROOM") != NULL ){

              myRooms[roomNum].startRoom = 1; 	

	    } else if ( strstr(buf, "END_ROOM") != NULL ){

              myRooms[roomNum].endRoom = 1; 	

	    }

          }

	  fclose(fp);
	  roomNum++;
	  free(myFile);

    }
  }
 
  closedir(dr);


  // Make a second pass over the room files
  // Now, filling out all the connections between rooms
  for (i = 0; i < roomNumsLength; i++) { 
          
	  char* myFile;
          myFile = malloc(50 * sizeof(char));
	  sprintf(myFile,"%s%s%s%s",newestDirName,"/",myRooms[i].name,"_room");

	  // Fill out connections
          FILE * fp;
	  fp = fopen(myFile, "r");
	  char buf[256];

	  while( !feof(fp) ){

    	    fgets(buf, 256, fp); 

	    // Get connection details
	    // Remove first portion of string to get connection info
            if( strstr(buf, "Connection") != NULL ){

	      char *connectShort = buf + 14;
              connectShort[strlen(connectShort)-1] = 0;

	      // Function to connect room to room named in connectShort string
	      makeConnection( myRooms, roomNumsLength, i, connectShort );

	    }

          }

          fclose(fp);
	  free(myFile); 

  } 	


  // Play game
  playGame( myRooms, roomNumsLength, &timeKeeper ); 
 
  // Free rooms to avoid memory leaks
  for (i = 0; i < roomNumsLength; i++) {        
    free(myRooms[i].name);
  } 	


  // Free thread memory
  pthread_join(timeKeeper, NULL);
 
  return 0;

}

