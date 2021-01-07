/*
 * JHU Deliverbot Navigation Group 
 * Navigation simulation system 
 * Robot simulations --- header file 
 *
 * @avnoks (GitHub)  5 Jan 2021 (create date)
 *
 */

/* 
 * This file contains protoypes for commonly-used functions and data structures.
 * [7 January 2021] Definitions contained herein are subject to change.
 *   Use abstractions to minimize the number of places in code that 
 *   will require changes if the following definitions change.
 */

#define ROBOT

typedef struct {
  /* X,Y,Z coordinates of robot */
  double x;
  double y;
  double z;
  /* Rotation of robot about X,Y,Z axes */
  double rx;
  double ry;
  double rz;

} Robot;
