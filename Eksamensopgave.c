#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMOUNT_OF_RUNS 790
#define MAX_LENGTH_NAMES 40
#define MAX_LENGTH_ABBREV 3+1
#define TIME_LENGTH 7
#define AGE_LENGTH 3
/*
TODO LIST:
O Make the program able to read the file
O Find a way to handle "-" in timing. Perhaps use atoi if "-" is not found.
X All italian riders over 30
X Function that returns an array of Danish riders that has a placing or OTL but not listing them twice.

X Give riders points !!!!!

X Print 10 riders that has most points and sort. If 2 or more has the same amount of points sort after surname
X In Paris Roubaix and Amstel Gold Race, find the rider that has completed both and has the lowest amount of runtime.
X Calculate average age(doubles) across all riders that has gotten a top 10 in a run and not listing them twice.
*/

enum RaceNames {ParisRoubaix, AmstelGoldRace, LaFlecheWallonne, LiegeBastogneLiege};

typedef struct Rider
{
  char RaceName[MAX_LENGTH_NAMES];
  char Name[MAX_LENGTH_NAMES];
  char TeamName[MAX_LENGTH_ABBREV];
  char Country[MAX_LENGTH_ABBREV];
  char Placing[MAX_LENGTH_ABBREV];
  char Time[TIME_LENGTH];
  int Age;
} Rider;

typedef struct RiderPoints
{
  char Name[MAX_LENGTH_NAMES];
  int Points;
} RiderPoints;


int getRiders(Rider *_rider); /* Puts the riders in the struct array. Returns 1 if the file is there and 0 if the file isn't there or null */
int checkProgramParameter(const char *_parameter); /* Checks the program parameters if it is --print */
void printUserInteraction(void); /* Prints user interaction sequence */
void printRider(Rider _rider); /* Prints the rider into console */
void givePoints(Rider *_rider); /* Gives the rider points based on his placings */
void countRidersInRace(Rider *_rider, int *total);
int calculatePoints(char *placing, int racetotal);
int findRace(Rider _rider);

int main(int argc, char *argv[])
{
  Rider rider[AMOUNT_OF_RUNS];
  if (argv[1] != NULL) /* If the user has putten in a single argument */
  {
    if (checkProgramParameter(argv[1])) /* If the user only wants to print the results of all functions that is relevant */
    {
      printf("Argv = %s\n", argv[1]);
      return EXIT_SUCCESS;
    }
    else /* If the function checkProgramParameter returns 0 */
    {
      printf("Not Valid!\n");
      return EXIT_FAILURE;
    }
  }
  else /* User interaction. Goes to this if argv[1] is null */
  {
    /*int i, userinput*/;
    printUserInteraction();
    /* scanf(" %d", &userinput); */
    
    if (getRiders(rider) == 0)
    {
      printf("Exiting program...\n");
      return EXIT_FAILURE;
    }
    else
    {
      /*for(i = 0; i < AMOUNT_OF_RUNS; ++i)
      {
        printRider(rider[i]);
      }*/
      printf("Points: %d\n", calculatePoints("1", 199));
      givePoints(rider);
      return EXIT_SUCCESS;
    }
  }
}

int getRiders(Rider *_rider)
{
  char age[AGE_LENGTH]; /* Temporary storage for the age */
  
  /* Read the file */
  FILE *fp;
  fp = fopen("cykelloeb", "r");
  
  if (fp == NULL)
  {
    printf("File (cykelloeb) not found or content is null!\n");
    return 0;
  }
  else
  {
    int i;
    for (i = 0; !feof(fp); ++i)
    {
      fscanf(fp, " %s \"%[A-Za-z-' ]\" | %s %s %s | %s %s", /* Tak til Anja Elisasen Lumholtz Nielsen for \" */
      _rider[i].RaceName,
      _rider[i].Name,
      age,
      _rider[i].TeamName,
      _rider[i].Country,
      _rider[i].Placing,
      _rider[i].Time);

      _rider[i].Age = atoi(age);
    }
    return 1;
  }
}

int checkProgramParameter(const char *_parameter)
{
  int result = strcmp(_parameter, "--print"); /* Compares the string with --print */
  if (result != 0) /* If they arent equal then return 0 (false) */
    return 0;
  return 1; /* Else return 1 (true) */
}

void printUserInteraction(void)
{
  printf("(1) Italian riders over 30 and their results\n");
  printf("(2) Danish riders that has gotten a placing or OTL\n");
  printf("(3) Top 10 riders\n");
  printf("(4) Best rider in Paris Roubaix & Amstel Gold Race\n");
  printf("(5) Average age of riders with top 10 in a race\n");
}

void printRider(Rider _rider)
{
  printf("Race name: %s | Rider: %s, Age: %d, ", _rider.RaceName, _rider.Name, _rider.Age);
  printf("Team: %s, Country: %s, Placing: %s, Time: %s\n", _rider.TeamName, _rider.Country, _rider.Placing, _rider.Time);
}

void givePoints(Rider *_rider)
{
  int i, j, total[4], race;
  RiderPoints points[AMOUNT_OF_RUNS] = {0};
  countRidersInRace(_rider, total);


  printf("Total[0]: %d, Total[1]: %d, Total[2]: %d, Total[3]: %d\n", total[0], total[1], total[2], total[3]);
  for(i = 0; i < AMOUNT_OF_RUNS; ++i)
  {
    race = findRace(_rider[i]);
    for(j = 0; j <= i; ++j)
    {
      if (points[j].Name[0] == '\0')
      {
        strcpy(points[j].Name, _rider[i].Name);
        points[j].Points += calculatePoints(_rider[i].Placing, total[race]);
        break;
      }
      else if (strcmp(points[j].Name, _rider[i].Name) == 0)
      {
        points[j].Points += calculatePoints(_rider[i].Placing, total[race]);
        break;
      }
    }
  }
  printf("Name: %s Points: %d\n", points[0].Name, points[0].Points);
}

void countRidersInRace(Rider *_rider, int *total)
{
  int i;
  total[ParisRoubaix] = 0, total[AmstelGoldRace] = 0, total[LaFlecheWallonne] = 0, total[LiegeBastogneLiege] = 0; /* Resets the array values to 0 */
  for(i = 0; i < AMOUNT_OF_RUNS; ++i) /* Runs until the end of AMOUNT_OF_RUNS */
  {
    if (strcmp(_rider[i].RaceName, "ParisRoubaix") == 0) /* If race name is Paris */
      total[0] += 1;
    else if ((strcmp(_rider[i].RaceName, "AmstelGoldRace") == 0)) /* If racename is Amstel... */
      total[1] += 1;
    else if ((strcmp(_rider[i].RaceName, "LaFlecheWallonne") == 0))
      total[2] += 1;
    else /* If the others didn't catch then go to this */
      total[3] += 1;
  }
}

int findRace(Rider _rider)
{
  if(strcmp(_rider.RaceName, "ParisRoubaix") == 0)
    return ParisRoubaix;
  else if (strcmp(_rider.RaceName, "AmstelGoldRace") == 0)
    return AmstelGoldRace;
  else if (strcmp(_rider.RaceName, "LaFlecheWallonne") == 0)
    return LaFlecheWallonne;
  else
    return LiegeBastogneLiege;
}

int calculatePoints(char *placing, int racetotal)
{
  if (strcmp(placing, "OTL") == 0)
    return 1;
  else if (strcmp(placing, "DNF") == 0)
    return 0;
  else if (strcmp(placing, "1") == 0)
    return (racetotal - atoi(placing))/13 + 10 + 3;
  else if (strcmp(placing, "2") == 0)
    return (racetotal - atoi(placing))/13 + 5 + 3;
  else if (strcmp(placing, "3") == 0)
    return (racetotal - atoi(placing))/13 + 2 + 3;
  else
    return (racetotal - atoi(placing))/13;
}