
/*****************************************
# Name: Program 2
# Date: February 2019
# Author: Adam Walker
# Description: Adventure game room builder
*****************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

// Struct used to represent a room in the game
struct room
{
  int id; // Room ID
  char* name; // Name of room
  int numOutboundConnections; // Number of connections
  struct room* outboundConnections[6]; // Holds pointers to connecting rooms
};


// Fills a provided int array with distinct random numbers in a specified range
// Essentially sampling without replacement
void randomDistinctNums(int intArray[], int length, int lower, int upper){

  // Total numbers from which to generate integers
  int range = upper - lower + 1;

  // Check range isn't smaller than count of numbers to be generated
  if( range < length ){

    // printf("Random range too small!\n");

  } else {

    // Loop through array using length variable to determine size
    int i;
    for (i = 0; i < length; i++) {

      // Keep generating numbers until unused number found
      int numAdded = 0;
      while( !numAdded ){

        // Get a random number in specified range	
        int num = (rand() % (upper - lower + 1)) + lower; 

        // Check if number already appears in array
        int j;
	int found = 0;
        for (j = 0; j < i; j++) {
          if( intArray[j] == num ){

	    found = 1;

	  }
        }

        // If number has not been used already, add to array
        if (found==0) {
	  intArray[i] = num;
	  numAdded = 1;
        }

      }

    } 	

  }

}


// Returns true if all rooms have at least 3 connections, false otherwise
int IsGraphFull(struct room myRooms[], int roomNumsLength )  
{

  // Loop through each of the rooms in the struct array
  int graphFull = 1;
  int i;
  for (i = 0; i < roomNumsLength; i++) { 
        
    // If a room has under 3 connections, the graph is not full
    if( myRooms[i].numOutboundConnections < 3 ){

      graphFull=0;

    }
  
  } 

  return graphFull;

}


// Returns true if a connection between two rooms already exists, false otherwise
int ConnectionAlreadyExists(struct room myRooms[], int randomRooms[])
{

  // Get the number of connections of the first room specified by randomRooms array
  int roomOneConnections = myRooms[randomRooms[0]].numOutboundConnections;

  // Loop through the first room's connections, checking whether the second room appears
  int alreadyConnected = 0;
  int i;
  for (i = 0; i < roomOneConnections; i++) { 
       
    // If the second room is found, the two rooms are already connected
    if( myRooms[randomRooms[0]].outboundConnections[i]->id == myRooms[randomRooms[1]].id ){

      alreadyConnected = 1;
  
    }
  
  } 

  return alreadyConnected;
  
}


// Connects two rooms together, does not check if this connection is valid
void ConnectRoom(struct room myRooms[], int randomRooms[]) 
{

  // The randomRooms array gives the location of the two rooms within the myRooms struct array
  // Using numOutboundConnections to correctly index the new connection in outboundConnections array
  int connectionNum1 = myRooms[randomRooms[0]].numOutboundConnections;
  myRooms[randomRooms[0]].outboundConnections[connectionNum1] = &myRooms[randomRooms[1]];
  myRooms[randomRooms[0]].numOutboundConnections++; // Increment connection count

  int connectionNum2 = myRooms[randomRooms[1]].numOutboundConnections;
  myRooms[randomRooms[1]].outboundConnections[connectionNum2] = &myRooms[randomRooms[0]];
  myRooms[randomRooms[1]].numOutboundConnections++;
  
}


// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection( struct room myRooms[] )  
{

  // Get two random rooms by picking two distinct integers from 0 to 6 inclusive
  // These integers represent location of rooms in myRooms struct array
  int randomRooms[2];
  randomDistinctNums(randomRooms, 2, 0, 6);

  // Check that each room has under 6 connections
  if( myRooms[randomRooms[0]].numOutboundConnections < 6 && myRooms[randomRooms[1]].numOutboundConnections < 6 ){

    // Check if rooms already connected to each other
    int roomsConnected = ConnectionAlreadyExists(myRooms, randomRooms);
    
    if( roomsConnected ){

      //printf("Rooms already connected\n");

    } else {

      //printf("Connect rooms!\n");
      ConnectRoom( myRooms, randomRooms );

    }

  }


}


// Randomly selects 7 of 10 hardcoded rooms
// Randomly generates connections between rooms
// Write game details to individual room files within a newly created directory
int main()
{

	// Set seed for random number generator
	// Reference: https://www.geeksforgeeks.org/generating-random-number-range-c/
	srand(time(0)); 

	// Create new directory to store room files, using PID in name
	// Reference: class notes
	int processID = getpid();
	
        // Create character array to hold directory name
	char* myDir;
        myDir = malloc(50 * sizeof(char));
	if (myDir == 0)
	  printf("malloc() failed!\n");
	memset(myDir, '\0', 50);

	sprintf(myDir, "./walkerad.rooms.%d",processID); // Create directory name using my OSU ID and the PID
        int result = mkdir(myDir, 0755); // Make the directory

        // Hardcode room names, using notable players from the SB champion New England Patriots
        char roomNames[10][9];
        strcpy(roomNames[0], "Edelman");
        strcpy(roomNames[1], "Brady");
        strcpy(roomNames[2], "Gronk");
        strcpy(roomNames[3], "Hogan");
        strcpy(roomNames[4], "Flowers");
        strcpy(roomNames[5], "Cannon");
        strcpy(roomNames[6], "Mason");
        strcpy(roomNames[7], "McCourty");
        strcpy(roomNames[8], "Gilmore");
        strcpy(roomNames[9], "White");

	// Array of room names and array of room structs
	int roomNumsLength = 7;
	int roomNums[roomNumsLength]; // Array to hold the indexes of 7 of the 10 hardcoded rooms.
	struct room myRooms[roomNumsLength]; // Array of 7 room structs, representing the rooms in the game

	// Fill array with distinct integers in range specified
	int upper = 9;
	int lower = 0;
	randomDistinctNums(roomNums, roomNumsLength, lower, upper);

	// Assign room names to structs
	// Assign IDs to rooms
	// Set outbound connections to zero
	int i;
	for (i = 0; i < roomNumsLength; i++) { 
          
	  int nameLocation = roomNums[i];
	  myRooms[i].name = roomNames[nameLocation];
	  myRooms[i].id = i;
	  myRooms[i].numOutboundConnections = 0;
 
    	} 	

	// Create all connections in graph
	// Each room to have between 3 and 6
	while ( !IsGraphFull( myRooms, roomNumsLength ) )
	{
  	  AddRandomConnection( myRooms );
	}

	// Write out the room files	
	for (i = 0; i < roomNumsLength; i++) { 

          // Character array for file name          
	  char* myFile;
          myFile = malloc(50 * sizeof(char));

	  if (myFile == 0)
	    printf("malloc() failed!\n");
	  memset(myFile, '\0', 50);

	  sprintf(myFile,"%s%s%s%s",myDir,"/",myRooms[i].name,"_room");

	  // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
          FILE * fp;
	  fp = fopen(myFile, "w+");

	  // Write the name of the room
	  fprintf(fp, "%s%s\n", "ROOM NAME: ", myRooms[i].name);

	  // Write the room's connections by looping through outboundConnections array
	  int j;
	  for (j = 0; j < myRooms[i].numOutboundConnections; j++) { 

	    fprintf(fp, "%s%d%s%s\n", "Connection ",j+1,": ",myRooms[i].outboundConnections[j]->name);

	  }
	  
	  // Write out the room type. The room randomly chosen first is the starting room, second is end room, and rest are in the middle
	  if( i==0 ) fprintf(fp, "%s", "ROOM TYPE: START_ROOM");
	  if( i==1 ) fprintf(fp, "%s", "ROOM TYPE: END_ROOM");
	  if( i>1 ) fprintf(fp, "%s", "ROOM TYPE: MID_ROOM");

	  // Close file connection
	  fclose(fp);

	  // Avoid memory leaks
	  free(myFile);
 
    	} 	


	// Avoid memory leaks
	free(myDir);

        return 0;

}

