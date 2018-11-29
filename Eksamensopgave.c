#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define AMOUNT_OF_RUNS 800
#define MAX_LENGTH_NAMES 40
#define MAX_LENGTH_ABBREV 3+1
#define TIME_LENGTH 7
#define AGE_LENGTH 3
/*
TODO LIST:
O Make the program able to read the file
O Find a way to handle "-" in timing. Perhaps use atoi if "-" is not found.
X All italian riders over 30
O Function that returns an array of Danish riders that has a placing or OTL but not listing them twice.

O Give riders points !!!!!

O Print 10 riders that has most points and sort. If 2 or more has the same amount of points sort after surname
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
  int Points;
  int Races;
} Rider;

int getRiders(Rider *_rider); /* Puts the riders in the struct array. Returns 1 if the file is there and 0 if the file isn't there or null */
int checkProgramParameter(const char *_parameter); /* Checks the program parameters if it is --print */
void printUserInteraction(void); /* Prints user interaction sequence */
void printRider(Rider *_rider, int _identifier); /* Prints the rider into console */
void printLine(void);

Rider *danishRidersWithPlacing(Rider *_rider); /* Function that returns a pointer to an array */
int checkArrayForDigit(const char *_string); /* Checks an array. Returns 1 if it is just numbers and 0 if there is a letter */

void givePoints(Rider *_rider, Rider *_points); /* Gives the rider points based on his placings */
void countRidersInRace(Rider *_rider, int *_total); /* Counts the riders, including DNF, in a given race */
int calculatePoints(char *_placing, int _racetotal); /* Calculates the points */
int findRace(Rider _rider); /* Finds a specific race */
int comparePoints(const void *_first, const void *_second); /* Sorting function for qsort */
void getLastName(const char *_input, char *_output, int _stringlength); /* Function that gets the last name via output array */

/* -------------------------------- Main -------------------------------- */

int main(int argc, char *argv[])
{
  int userinput;

  Rider rider[AMOUNT_OF_RUNS];
  Rider points[AMOUNT_OF_RUNS];
  Rider *races;

  if (argv[1] != NULL) /* If the user has putten in a single argument */
  {
    if (checkProgramParameter(argv[1])) /* If the user only wants to print the results of all functions that is relevant */
    {
      if (getRiders(rider) == 0) /* If it returns 0 */
      {
        printf("Exiting program...\n");
        return EXIT_FAILURE;
      }
      else
      {
        /* 1 */

        printLine();
        /* 2 */
        races = danishRidersWithPlacing(rider);
        printRider(races, 2);
        printLine();
        /* 3 */
        givePoints(rider, points);
        qsort(points, AMOUNT_OF_RUNS, sizeof(Rider), comparePoints);
        printRider(points, 3);
        printLine();
        /* 4 */

        printLine();
        /* 5 */

        printLine();
        return EXIT_SUCCESS;
      }
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
      if (userinput == 1) /* Italians over 30 */
      {
        printf("User input: %d\n", userinput);
        return EXIT_SUCCESS;
      }
      else if (userinput == 2) /* Danish with placings */
      {
        races = danishRidersWithPlacing(rider);
        printRider(races, 2);
        return EXIT_SUCCESS;
      }
      else if (userinput == 3) /* Top 10 with points */
      {
        givePoints(rider, points);
        qsort(points, AMOUNT_OF_RUNS, sizeof(Rider), comparePoints);
        printRider(points, 3);
        return EXIT_SUCCESS;
      }
      else if (userinput == 4) /* Best rider in Parix and Amstel */
      {
        printf("User input: %d\n", userinput);
        return EXIT_SUCCESS;
      }
      else if (userinput == 5) /* Average age between top 10 finishes */
      {
        printf("User input: %d\n", userinput);
        return EXIT_SUCCESS;
      }
      else
      {
        printf("Exiting program...\n");
        return EXIT_SUCCESS;
      }
    }
  }
}

/* -------------------------------- End -------------------------------- */
/* ------------------------------- Start ------------------------------- */

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
  printf("(Any other number) Exit program\n");
}

void printRider(Rider *_rider, int _identifier)
{
  int i;
  if (_identifier == 1)
  {
    /*printf("Race name: %s | Rider: %s | Age: %d | ", _rider.RaceName, _rider.Name, _rider.Age);
    printf("Team: %s | Country: %s | Placing: %s | Time: %s\n", _rider.TeamName, _rider.Country, _rider.Placing, _rider.Time);*/
  }
  if (_identifier == 2)
  {
    for(i = 0; strcmp(_rider[i].Name, "") != 0; ++i) /* Runs until name is empty since Race has a lot of empty space */
      printf("Name: %s | Races: %d\n", _rider[i].Name, _rider[i].Races);
  }
  if (_identifier == 3)
  {
    for(i = 0; i < 10; ++i)
    {
      printf("Name: %s | Points: %d\n", _rider[i].Name, _rider[i].Points);
    }
  }
}

void printLine(void)
{
  printf("___________________________________________________\n");
}

/* -------------------------------- End -------------------------------- */
/* ---------------------- Functions for assignement 1 ------------------ */



/* -------------------------------- End -------------------------------- */
/* ---------------------- Functions for assignement 2 ------------------ */

Rider *danishRidersWithPlacing(Rider *_rider) /* Returning a pointer to arrray from: https://www.tutorialspoint.com/cprogramming/c_return_arrays_from_function.htm */
{
  static Rider races[AMOUNT_OF_RUNS];
  int i, j, checkcountry, checkplacing, resultdigits;

  for(i = 0; i < AMOUNT_OF_RUNS; ++i) /* Go through all the riders */
  {
    checkcountry = strcmp(_rider[i].Country, "DEN");
    resultdigits = checkArrayForDigit(_rider[i].Placing);
    checkplacing = strcmp(_rider[i].Placing, "OTL");
    for(j = 0; j <= i; ++j) /* Nested loop to check for every entry in the array*/
    {
      if (races[j].Name[0] == '\0' && checkcountry == 0 && (resultdigits || checkplacing == 0))
      {
        strcpy(races[j].Name, _rider[i].Name); /* Copies the name to the new struct array */
        races[j].Races = 1;
        break;
      }
      else if (strcmp(races[j].Name, _rider[i].Name) == 0 && (resultdigits || checkplacing == 0)) /* Checks if name is there. If not then loop, if there is then add  */
      {
        races[j].Races += 1;
        break; /* Break out of the loop */
      }
    }
  }

  return races;
}

int checkArrayForDigit(const char *_string)
{
  int i, length = strlen(_string);
  for(i = 0; i < length; ++i)
  {
    if(!isdigit(_string[i]))
      return 0;
  }
  return 1;
}

/* -------------------------------- End -------------------------------- */
/* ---------------------- Functions for assignment 3 ------------------- */

void givePoints(Rider *_rider, Rider *_points)
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

int comparePoints(const void *_first, const void *_second)
{
  const Rider *pfirst = _first;
  const Rider *psecond = _second;
  int firstresult = strlen(pfirst->Name), secondresult = strlen(psecond->Name); /* Gets the amount of chars in name */
  char firstsurname[MAX_LENGTH_NAMES], secondsurname[MAX_LENGTH_NAMES];

  if (pfirst->Points < psecond->Points) /* If second has more points than first return second */
    return 1;
  else if (pfirst->Points > psecond->Points) /* If first has more points than second return first */
    return -1;
  else /* If the amount of points are the same, get last names and compare and return that value */
  {
    getLastName(pfirst->Name, firstsurname, firstresult);
    getLastName(psecond->Name, secondsurname, secondresult);
    return strcmp(firstsurname, secondsurname); /* Compares and returns */
  }
}

void getLastName(const char *_input, char *_output, int _stringlength)
{
  int i, j = 0;
  for(i = 0; i < _stringlength; ++i) /* Runs through the entire array */
  {
    if ((isupper(_input[i]) && isupper(_input[i+1])) || (isupper(_input[i]) && _input[i+1] == ' '))
    { /* if name[i] is uppercase and the one after is uppercase or name[i] is uppercase and the one after is a space*/
      _output[j] = _input[i]; /* Copies values over */
      ++j;
    }
    else if (_input[i] == ' ' && isupper(_input[i+1])) /* If the last name contains a space */
    {
      _output[j] = ' ';
      ++j;
    }
  }
  _output[j] = _input[i-1];/* Gets the last character in the name */
  _output[++j] = '\0'; /* Ends with \0 so the string ends there */
}

/* -------------------------------- End -------------------------------- */