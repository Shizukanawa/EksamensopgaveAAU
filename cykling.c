#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define AMOUNT_OF_RUNS 800
#define MAX_LENGTH_NAMES 40
#define MAX_LENGTH_ABBREV 3+1
#define TIME_LENGTH 7
#define AGE_LENGTH 3

enum RaceNames {ParisRoubaix, AmstelGoldRace, LaFlecheWallonne, LiegeBastogneLiege};
enum Time {Hours, Minutes, Seconds};

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
  int Hours;
  int Minutes;
  int Seconds;
} Rider;

int getRiders(Rider *_rider); /* Puts the riders in the struct array. Returns 1 if the file is there and 0 if the file isn't there or null */
int checkProgramParameter(const char *_parameter); /* Checks the program parameters if it is --print */
void printUserInteraction(void); /* Prints user interaction sequence */
void printRider(Rider *_rider, int _identifier); /* Prints the rider into console */
void printLine(void);

void printItalianResultsOver30(const Rider *_rider);

Rider *danishRidersWithPlacing(const Rider *_rider); /* Function that returns a pointer to an array */
int checkArrayForDigit(const char *_string); /* Checks an array. Returns 1 if it is just numbers and 0 if there is a letter */

Rider *givePoints(const Rider *_rider); /* Gives the rider points based on his placings */
void countRidersInRace(const Rider *_rider, int *_total); /* Counts the riders, including DNF, in a given race */
int calculatePoints(const char *_placing, const int _racetotal); /* Calculates the points */
int findRace(const Rider _rider); /* Finds a specific race */
int comparePoints(const void *_first, const void *_second); /* Sorting function for qsort */
void getLastName(const char *_input, int _stringLength, char *_output); /* Function that gets the last name via output array */

void getBestPerforming(const Rider *_rider, Rider *_outRider); /* Finds the best performing that has a time in both Paris and Amstel */
int compareTime(const Rider _firstRider, const Rider _secondRider); /* Comparing function to find the lowest time */

double averageAgeTop10(const Rider *_rider); /* Returns average age for riders with top 10 placings */
int isTop10(const Rider _rider); /* Checks if rider has a top10 placing */

/* -------------------------------- Main -------------------------------- */

int main(int argc, char *argv[]) /* Help with argc and argv from https://www.tutorialspoint.com/cprogramming/c_command_line_arguments.htm */
{
  int userinput;
  Rider rider[AMOUNT_OF_RUNS];
  Rider *races, *points;
  Rider bestRiders[2];

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
        printItalianResultsOver30(rider);
        printLine();

        /* 2 */

        races = danishRidersWithPlacing(rider);
        printRider(races, 2);
        printLine();

        /* 3 */

        points = givePoints(rider);
        qsort(points, AMOUNT_OF_RUNS, sizeof(Rider), comparePoints);
        printRider(points, 3);
        printLine();

        /* 4 */

        getBestPerforming(rider, bestRiders);
        printRider(bestRiders, 4);
        printLine();

        /* 5 */
        
        printf("| Average age for riders with top 10 placing: %lf |\n", averageAgeTop10(rider));
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
        printItalianResultsOver30(rider);
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
        points = givePoints(rider);
        qsort(points, AMOUNT_OF_RUNS, sizeof(Rider), comparePoints);
        printRider(points, 3);
        return EXIT_SUCCESS;
      }
      else if (userinput == 4) /* Best rider in Parix and Amstel */
      {
        getBestPerforming(rider, bestRiders);
        printRider(bestRiders, 4);
        return EXIT_SUCCESS;
      }
      else if (userinput == 5) /* Average age between top 10 finishes */
      {
        printf("| Average age for riders with top 10 placing: %lf |\n", averageAgeTop10(rider));
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
      fscanf(fp, " %s \"%[A-Za-z-' ]\" | %s %s %s | %s %s", /* Thanks to Anja Elisasen Lumholtz Nielsen for \" */
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
  int i, top10 = 10;
  if (_identifier == 2)
  {
    for(i = 0; strcmp(_rider[i].Name, "") != 0; ++i) /* Runs until name is empty since Race has a lot of empty space */
      printf("| Name: %-25s | Country: %-3s | Races: %-1d |\n", _rider[i].Name, _rider[i].Country ,_rider[i].Races); /* https://stackoverflow.com/questions/14420924/aligning-output-values-in-c for alignment */
  }
  if (_identifier == 3)
  {
    for(i = 0; i < top10; ++i) 
    {
      if(_rider[i].Name[0] == '\0' || _rider[i].Name[0] == '') /* Failsafe.  is 3. 3 is apparently end of text according to http://www.asciitable.com/ */
      {
        ++top10;
      }
      else if (strcmp(_rider[i].Name, " ") != 0 && _rider[i].Name[0] != '\0')
      {
        printf("| Name: %-28s | Points: %-1d |\n", _rider[i].Name, _rider[i].Points);
      }
    }
  }
  if (_identifier == 4)
  {
    for(i = 0; i < 2; ++i)
      printf("| Best rider: %-20s | Hours: %d | Minutes: %d | Seconds: %d |\n", _rider[i].Name, _rider[i].Hours, _rider[i].Minutes, _rider[i].Seconds);
  }
}

void printLine(void)
{
  printf("_______________________________________________________________________________________________________\n");
}

/* -------------------------------- End -------------------------------- */
/* ---------------------- Functions for assignement 1 ------------------ */

void printItalianResultsOver30(const Rider *_rider)
{
  int i;
  for(i = 0; i < AMOUNT_OF_RUNS; ++i)
  {
    if(strcmp(_rider[i].Country, "ITA") == 0 && _rider[i].Age > 30)
    {
      printf("| Race: %-20s | Name: %-20s | Age: %-1d ", _rider[i].RaceName, _rider[i].Name, _rider[i].Age);
      printf("| Team: %-1s | Country: %-1s | Placing: %-3s | Time: %-10s |\n", _rider[i].TeamName, _rider[i].Country, _rider[i].Placing, _rider[i].Time);
    }
  }
}

/* -------------------------------- End -------------------------------- */
/* ---------------------- Functions for assignement 2 ------------------ */

Rider *danishRidersWithPlacing(const Rider *_rider) /* Returning a pointer to arrray from: https://www.tutorialspoint.com/cprogramming/c_return_arrays_from_function.htm */
{
  static Rider races[AMOUNT_OF_RUNS];
  int i, j, checkCountry, checkPlacing, resultDigits; /* i = going through all riders and j = races array */

  for(i = 0; i < AMOUNT_OF_RUNS; ++i) /* Go through all the riders */
  {
    checkCountry = strcmp(_rider[i].Country, "DEN");
    resultDigits = checkArrayForDigit(_rider[i].Placing);
    checkPlacing = strcmp(_rider[i].Placing, "OTL");
    for(j = 0; j <= i; ++j) /* Nested loop to check for every entry in the array*/
    {
      if (races[j].Name[0] == '\0' && checkCountry == 0 && (resultDigits || checkPlacing == 0))
      {
        strcpy(races[j].Name, _rider[i].Name); /* Copies the name to the new struct array */
        strcpy(races[j].Country, _rider[i].Country);
        races[j].Races = 1;
        break;
      }
      else if (strcmp(races[j].Name, _rider[i].Name) == 0 && (resultDigits || checkPlacing == 0)) /* Checks if name is there. If not then loop, if there is then add  */
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

Rider *givePoints(const Rider *_rider)
{
  static Rider points[AMOUNT_OF_RUNS];
  int i, j, total[4], race; /* i = Going through all the riders and j = going through _points array */
  countRidersInRace(_rider, total);
  memset(points, 0, sizeof(Rider));

  for(i = 0; i < AMOUNT_OF_RUNS; ++i) /* Go through all the riders */
  {
    race = findRace(_rider[i]);
    for(j = 0; j <= i; ++j) /* Nested loop to check for every entry in the array*/
    {
      if (points[j].Name[0] == '\0') /* If points[j] is null, then add a new entry. Skip if it isn't */
      {
        strcpy(points[j].Name, _rider[i].Name); /* Copies the name to the new struct array */
        points[j].Points += calculatePoints(_rider[i].Placing, total[race]);
        break;
      }
      else if (strcmp(points[j].Name, _rider[i].Name) == 0) /* Checks if name is there. If not then loop, if there is then add  */
      {
        points[j].Points += calculatePoints(_rider[i].Placing, total[race]); /* Assigns the current amount + calculated points */
        break; /* Break out of the loop */
      }
    }
  }
  return points;
}

void countRidersInRace(const Rider *_rider, int *_total)
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

int findRace(const Rider _rider)
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

int calculatePoints(const char *_placing, const int _racetotal)
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
  int firstResult = strlen(pfirst->Name), secondResult = strlen(psecond->Name); /* Gets the amount of chars in name */
  char firstSurname[MAX_LENGTH_NAMES], secondSurname[MAX_LENGTH_NAMES];

  if (pfirst->Points < psecond->Points) /* If second has more points than first return second */
    return 1;
  else if (pfirst->Points > psecond->Points) /* If first has more points than second return first */
    return -1;
  else /* If the amount of points are the same, get last names and compare and return that value */
  {
    getLastName(pfirst->Name, firstResult, firstSurname);
    getLastName(psecond->Name, secondResult, secondSurname);
    return strcmp(firstSurname, secondSurname);
  }
}

void getLastName(const char *_input, int _stringLength, char *_output)
{
  int i, j = 0;
  for(i = 0; i < _stringLength; ++i) /* Runs through the entire array */
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
/* ---------------------- Functions for assignment 4 ------------------- */

void getBestPerforming(const Rider *_rider, Rider *_outRider)
{
  Rider parisRiders[AMOUNT_OF_RUNS];
  Rider bestRider[2];
  int i, j = 0, total[4], timeHours, timeMinutes, timeSeconds;
  /* i = start of paris and j = the start of array parisRiders */
  int k, l; /* k = start of amstel and l = end of amstel */
  countRidersInRace(_rider, total);

  l = total[1];
  for(i = 0; i < total[0]; ++i) /* Finds every racer in Paris */
  {
    k = total[0]; /* Goes through every racer in Amstel */
    for(; k < total[0] + l; ++k)
    {
      if(strcmp(_rider[i].Name, _rider[k].Name) == 0 && strcmp(_rider[i].Time, "-") != 0 && strcmp(_rider[k].Time, "-") != 0)
      { /* If there is a time for both, get values */
        sscanf(_rider[i].Time, " %d:%d:%d", &parisRiders[j].Hours, &parisRiders[j].Minutes, &parisRiders[j].Seconds); /* Scans a string */
        sscanf(_rider[k].Time, " %d:%d:%d", &timeHours, &timeMinutes, &timeSeconds); /* Scans for the rider in k and stores it */
        strcpy(parisRiders[j].Name, _rider[i].Name); /* Copies name */
        parisRiders[j].Hours += timeHours; /* Takes hours + timeHours */
        parisRiders[j].Minutes += timeMinutes;
        parisRiders[j].Seconds += timeSeconds;

        if(parisRiders[j].Seconds >= 60) /* If seconds is equal or more than 60 */
        {
          parisRiders[j].Minutes += 1;
          parisRiders[j].Seconds -= 60;
        }
        if(parisRiders[j].Minutes >= 60) /* Same but with minutes */
        {
          parisRiders[j].Hours += 1;
          parisRiders[j].Minutes -= 60; 
        }
        ++j;
      }
    }
  }
  bestRider[0] = parisRiders[0]; /* Just takes the 1st parisrider and puts it in bestrider */

  for(i = 1; parisRiders[i].Name[0] != '\0'; ++i) /* Compares each in parisriders with the best rider. */
  {
    if(compareTime(bestRider[0], parisRiders[i]) == 0) /* if parisriders have more time than bestrider */
      bestRider[0] = bestRider[0];
    else if(compareTime(bestRider[0], parisRiders[i]) == 1) /* Vice versa */
      bestRider[0] = parisRiders[i];
    else if(compareTime(bestRider[0], parisRiders[i]) == 2) /* If they are the same */
      bestRider[1] = parisRiders[i];
  }

  for(i = 0; i < 2; ++i)
    _outRider[i] = bestRider[i]; /* Copies values over */
}

int compareTime(const Rider _firstRider, const Rider _secondRider)
{
  if(_firstRider.Hours < _secondRider.Hours)
    return 0;
  else if(_firstRider.Hours > _secondRider.Hours)
    return 1;
  else if(_firstRider.Minutes < _secondRider.Minutes)
    return 0;
  else if(_firstRider.Minutes > _secondRider.Minutes)
    return 1;
  else if(_firstRider.Seconds < _secondRider.Seconds)
    return 0;
  else if(_firstRider.Seconds > _secondRider.Seconds)
    return 1;
  else
    return 2;
}

/* -------------------------------- End -------------------------------- */
/* ---------------------- Functions for assignement 5 ------------------ */

double averageAgeTop10(const Rider *_rider)
{
  int i = 0, j, sumOfAge = 0, personsCounted = 0; 
  Rider names[40];

  for(i = 0; i < AMOUNT_OF_RUNS; ++i)
  {
    for(j = 0; j <= i; ++j)
    {
      if(isTop10(_rider[i]) && names[j].Name[0] == '\0') /* If names[j] is empty */
      {
        strcpy(names[j].Name, _rider[i].Name);
        sumOfAge += _rider[i].Age;
        ++personsCounted;
        break;
      }
      else if(isTop10(_rider[i]) && strcmp(names[j].Name, _rider[i].Name) == 0) /* If name exist, break */
      {
        break;
      }
    }
  }
  return (double) sumOfAge / personsCounted; /* Typecasting for average age */
}

int isTop10(const Rider _rider)
{
  int result;
  if (strcmp(_rider.Placing, "OTL") == 0 || strcmp(_rider.Placing, "DNF") == 0)
    return 0;
  result = atoi(_rider.Placing);
  if (result >= 1 && result <= 10)
    return 1;
  else
    return 0;
}
/* -------------------------------- End -------------------------------- */