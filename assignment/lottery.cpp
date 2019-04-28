/*-----------------------------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class name here>
 *-----------------------------------------------------------------------------
 * Exercise Number: #exercise_number#
 * File:			lottery.c
 * Author(s):		Peter Bauer
 * Due Date:		#due#
 *-----------------------------------------------------------------------------
 * Description:
 * Implementation of a library for analyzing lottery tips.
 *-----------------------------------------------------------------------------
*/
#include "lottery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _file {
  FILE* fd;
  char csv_separator;
  int drawing[TIP_SIZE];
};

typedef struct _file* File;
File crnt_file;

#define UUID_LEN 40
#define MAX_TIP_LEN 17
#define MAX_LINE_LEN (UUID_LEN + 1 + MAX_TIP_LEN + 1)

bool 	init_lottery (const char *csv_file, char csv_separator) {
  crnt_file = (File)malloc(sizeof(struct _file));
  crnt_file->fd = fopen(csv_file, "r");
  crnt_file->csv_separator = csv_separator;
  for(int i = 0; i < 6; i++)
  {
    crnt_file->drawing[i] = -1;
  }
  return crnt_file->fd != 0;
}

bool get_tip (int tip_number, int tip[TIP_SIZE]) {
  fseek(crnt_file->fd, 0, SEEK_SET);
  char* crnt_tip = new char[MAX_LINE_LEN];
  for(int i = 0; i <= tip_number; i++) {
    crnt_tip = fgets(crnt_tip, MAX_LINE_LEN, crnt_file->fd);
    if(crnt_tip == 0) { return false; }
  }

  if(tip_number >= 0 && crnt_tip != 0)
  {
    if(crnt_tip[strlen(crnt_tip)-1] == '\n') // Wenn hinten '\n' ist wird es durch den csv_separator ersetzt
    {
        crnt_tip[strlen(crnt_tip)-1] = crnt_file->csv_separator;
    }

    const char* sep = &crnt_file->csv_separator;
    char* token = 0;
    int count = 0;
    token = strtok(crnt_tip, sep);
    token = strtok(0, sep);
    while (token != 0)
    {
      if(strlen(token) > 1)
      {
        tip[count] = (token[0]- '0')*10 + token[1]-'0';
      }
      else {
        tip[count] = *token - '0';
      }
      count++;
      token = strtok(0, sep);
    }
    return crnt_tip != 0;
  }
  return false;
}

bool set_drawing (int drawing_numbers[TIP_SIZE]) {
  for(int i = 0; i < TIP_SIZE; i++)
  {
    if(drawing_numbers[i] < 1 || drawing_numbers[i] > 45)
    {
      return false;
    }
    crnt_file->drawing[i] = drawing_numbers[i];
  }
  return true;
}

bool is_tip_in_drawing(int tip)
{
  for(int i = 0; i < TIP_SIZE; i++)
  {
    if(crnt_file->drawing[i] == tip)
    {
      return true;
    }
  }
  return false;
}

int get_tip_result (int tip_number) {
  if(crnt_file->drawing[0] == -1) { return -1; }
  int crnt_tip[6];
  if(!get_tip(tip_number, crnt_tip)) { return -2; }
  int count = 0;
  for(int i = 0; i < TIP_SIZE; i++)
  {
    if(is_tip_in_drawing(crnt_tip[i]))
    {
      count++;
    }
  }
  return count;
}

int get_right_tips_count (int right_digits_count) {
  if(right_digits_count < 0 || right_digits_count > TIP_SIZE || crnt_file->drawing[0] == -1) { return -1; }
  int count = 0;
  int i = 0;
  while(get_tip_result(i) >= 0)
  {
    if(get_tip_result(i) == right_digits_count)
    {
        count ++;
    }
    i++;
  }
  return count;
}
