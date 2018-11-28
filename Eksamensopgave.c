#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

O Give riders points !!!!!

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
void givePoints(Rider *_rider, RiderPoints *_points); /* Gives the rider points based on his placings */
void countRidersInRace(Rider *_rider, int *_total); /* Counts the riders, including DNF, in a given race */
int calculatePoints(char *_placing, int _racetotal); /* Calculates the points */
int findRace(Rider _rider); /* Finds a specific race */
void sortPoints(RiderPoints *_points);
int comparePoints(const void *_first, const void *_second);

int main(int argc, char *argv[])
{
  int i, userinput;

  Rider rider[AMOUNT_OF_RUNS];
  RiderPoints points[AMOUNT_OF_RUNS] = {0};

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
    printUserInteraction();
    scanf(" %d", &userinput);
    
    if (getRiders(rider) == 0)
    {
      printf("Exiting program...\n");
      return EXIT_FAILURE;
    }
    else
    {
      if (userinput == 1)
      {
        printf("User input: %d", userinput);
      }
      else if (userinput == 2)
      {
        printf("User input: %d", userinput);
      }
      else if (userinput == 3)
      {
        givePoints(rider, points);
        sortPoints(points);

        for(i = 0; i < 10; ++i)
        {
          printf("Name: %s Points: %d\n", points[i].Name, points[i].Points);
        }
      }
      else if (userinput == 4)
      {
        printf("User input: %d", userinput);
      }
      else if (userinput == 5)
      {
        printf("User input: %d", userinput);
      }
      else if (userinput == -1)
      {
        printf("Exiting program...\n");
      }
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
  printf("(-1) Exit program\n");
}

void printRider(Rider _rider)
{
  printf("Race name: %s | Rider: %s, Age: %d, ", _rider.RaceName, _rider.Name, _rider.Age);
  printf("Team: %s, Country: %s, Placing: %s, Time: %s\n", _rider.TeamName, _rider.Country, _rider.Placing, _rider.Time);
}

void givePoints(Rider *_rider, RiderPoints *_points)
{
  int i, j, total[4], race;
  countRidersInRace(_rider, total);

  for(i = 0; i < AMOUNT_OF_RUNS; ++i) /* Go through all the riders */
  {
    race = findRace(_rider[i]);
    for(j = 0; j <= i; ++j) /* Nested loop to check for every entry in the array*/
    {
      if (_points[j].Name[0] == '\0') /* If points[j] is null, then add a new entry. Skip if it isn't */
      {
        strcpy(_points[j].Name, _rider[i].Name); /* Copies the name to the new struct array */
        _points[j].Points += calculatePoints(_rider[i].Placing, total[race]);
        break;
      }
      else if (strcmp(_points[j].Name, _rider[i].Name) == 0) /* Checks if name is there. If not then loop, if there is then add  */
      {
        _points[j].Points += calculatePoints(_rider[i].Placing, total[race]); /* Assigns the current amount + calculated points */
        break; /* Break out of the loop */
      }
    }
  }
}

void countRidersInRace(Rider *_rider, int *_total)
{
  int i;
  _total[ParisRoubaix] = 0, _total[AmstelGoldRace] = 0, _total[LaFlecheWallonne] = 0, _total[LiegeBastogneLiege] = 0; /* Resets the array values to 0 */
  for(i = 0; i < AMOUNT_OF_RUNS; ++i) /* Runs until the end of AMOUNT_OF_RUNS */
  {
    if (strcmp(_rider[i].RaceName, "ParisRoubaix") == 0) /* If race name is Paris */
      _total[0] += 1;
    else if ((strcmp(_rider[i].RaceName, "AmstelGoldRace") == 0)) /* If racename is Amstel... */
      _total[1] += 1;
    else if ((strcmp(_rider[i].RaceName, "LaFlecheWallonne") == 0))
      _total[2] += 1;
    else /* If the others didn't catch then go to this */
      _total[3] += 1;
  }
}

int findRace(Rider _rider)
{
  if(strcmp(_rider.RaceName, "ParisRoubaix") == 0) /* If the racename is equal to ParisRoubaix */
    return ParisRoubaix; /* Return enum ParisRoubaix(0) */
  else if (strcmp(_rider.RaceName, "AmstelGoldRace") == 0)
    return AmstelGoldRace;
  else if (strcmp(_rider.RaceName, "LaFlecheWallonne") == 0)
    return LaFlecheWallonne;
  else
    return LiegeBastogneLiege;
}

int calculatePoints(char *_placing, int _racetotal)
{
  if (strcmp(_placing, "OTL") == 0) /* Riders gets 1 point for being over time limit */
    return 1;
  else if (strcmp(_placing, "DNF") == 0) /* Riders gets 0 points for not finishing */
    return 0;
  else if (strcmp(_placing, "1") == 0) /* If the rider gets 1st */
    return (_racetotal - atoi(_placing))/13 + 10 + 3;
  else if (strcmp(_placing, "2") == 0)
    return (_racetotal - atoi(_placing))/13 + 5 + 3; /* 2nd */
  else if (strcmp(_placing, "3") == 0)
    return (_racetotal - atoi(_placing))/13 + 2 + 3; /* 3rd */
  else /* Gets a placing that isn't OTL or DNF */
    return (_racetotal - atoi(_placing))/13 + 3; /* Gets 3 extra points for being within timelimit */
}

void sortPoints(RiderPoints *_points)
{
  qsort(_points, AMOUNT_OF_RUNS, sizeof(RiderPoints), comparePoints);
}

int comparePoints(const void *_first, const void *_second)
{
  const RiderPoints *pfirst = _first;
  const RiderPoints *psecond = _second;
  int i, j = 0, firstresult = strlen(pfirst->Name), secondresult = strlen(psecond->Name);
  char firstsurname[MAX_LENGTH_NAMES] = {0}, secondsurname[MAX_LENGTH_NAMES] = {0};

  if (pfirst->Points < psecond->Points)
    return 1;
  else if (pfirst->Points > psecond->Points)
    return -1;
  else
  {
    for(i = 0; i < firstresult; ++i) /* Gets the first persons last name */
    {
      if ((isupper(pfirst->Name[i]) && isupper(pfirst->Name[i+1])) || (isupper(pfirst->Name[i]) && pfirst->Name[i+1] == ' '))
      { /* if name[i] is uppercase and the one after is uppercase or name[i] is uppercase and the one after is a space*/
        firstsurname[j] = pfirst->Name[i];
        ++j;
      }
      else if (pfirst->Name[i] == ' ' && isupper(pfirst->Name[i+1]))
      { /* If the last name contains a space */
        firstsurname[j] = ' ';
        ++j;
      }
    }
    firstsurname[j] = pfirst->Name[i-1];
    firstsurname[++j] = '\0';

    j = 0;
    for(i = 0; i < secondresult; ++i) /* Get second persons last name */
    {
      if ((isupper(psecond->Name[i]) && isupper(psecond->Name[i+1])) || (isupper(psecond->Name[i]) && psecond->Name[i+1] == ' '))
      {
        secondsurname[j] = psecond->Name[i];
        ++j;
      }
      else if (psecond->Name[i] == ' ' && isupper(psecond->Name[i+1]))
      {
        secondsurname[j] = ' ';
        ++j;
      }
    }
    secondsurname[j] = psecond->Name[i-1];
    secondsurname[++j] = '\0';
    
    return strcmp(firstsurname, secondsurname); /* Compares and returns */
  }
}