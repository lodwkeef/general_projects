// ******************************************************************************************* //
//
// File:        timer.c
// Date:        3/28/2016
// Authors:     Alex Thompson
//              Ben Schifman
//              Justin Siekmann
//              Kevin Curtis
//
// Created on May 1, 2016
//
// Description: Motor Go RIGHT HERE
// ******************************************************************************************* //
#include <xc.h>
#include "triangulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define T3x 0.0
#define T3y 0.0
#define T2x 0.0
#define T2y 132.0
#define T1x 216.0
#define T1y 48.0
#define pi 3.14159265
#define STEPS_PER_DEG 142.222

float posXreturn(float steps1, float steps2, float steps3){

	float x_R;
	float deg1 = steps1/STEPS_PER_DEG;
	float deg2 = steps2/STEPS_PER_DEG;
	float deg3 = steps3/STEPS_PER_DEG;

    float alpha_1 = deg1 * (pi/180);
    float alpha_2 = deg2 * (pi/180);
    float alpha_3 = deg3 * (pi/180);

	float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;
    
    float x1_p;
    float y1_p;
    float x2_p;
    float y2_p;
    float x3_p;
    float y3_p;
    
    float T_12;
    float T_23;
    float T_31;
    
    float x_12;
    float y_12;
    float x_23;
    float y_23;
    float x_31;
    float y_31;
    
    float k_31; 
    float D;

	if ((alpha_2 - alpha_1) >= pi || (alpha_2 - alpha_1) <= -1*pi) {
		x_R = -1.0;
	}
	else if ((alpha_3 - alpha_2) >= pi || (alpha_3 - alpha_2) <= -1*pi) {
		x_R = -1.0;
	}
	else {
		x_R = 0.0;
	}
    /*
    float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;*/
    
    x1_p = x1 - x2;
    y1_p = y1 - y2;
    x2_p = 0;
    y2_p = 0;
    x3_p = x3 - x2;
    y3_p = y3 - y2;
    
    T_12 = cos(alpha_2 - alpha_1)/sin(alpha_2 - alpha_1);
    T_23 = cos(alpha_3 - alpha_2)/sin(alpha_3 - alpha_2);
    T_31 = (1 - (T_12 * T_23))/(T_12 + T_23);
    
    x_12 = x1_p + (T_12*y1_p);
    y_12 = y1_p - (T_12*x1_p);
    x_23 = x3_p - (T_23*y1_p);
    y_23 = y3_p + (T_23*x1_p);
    x_31 = (x1_p + x3_p) + (T_31*(y3_p - y1_p));
    y_31 = (y1_p + y3_p) - (T_31*(x3_p - x1_p));
    
    k_31 = (x1_p*x3_p)+(y1_p*y3_p)+(T_31*((x1_p*y3_p)-(x3_p*y1_p))); 
    D = ((x_12-x_23)*(y_23-y_31)) - ((y_12-y_23)*(x_23-x_31));
    
    x_R = x2 + ((k_31*(y_12-y_23))/D);
    
    return (x_R);
}

float posYreturn(float steps1, float steps2, float steps3){

	float y_R;
	float deg1 = steps1/STEPS_PER_DEG;
	float deg2 = steps2/STEPS_PER_DEG;
	float deg3 = steps3/STEPS_PER_DEG;

    float alpha_1 = deg1 * (pi/180);
    float alpha_2 = deg2 * (pi/180);
    float alpha_3 = deg3 * (pi/180);

	float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;
    
    float x1_p;
    float y1_p;
    float x2_p;
    float y2_p;
    float x3_p;
    float y3_p;
    
    float T_12;
    float T_23;
    float T_31;
    
    float x_12;
    float y_12;
    float x_23;
    float y_23;
    float x_31;
    float y_31;
    
    float k_31; 
    float D;

	if ((alpha_2 - alpha_1) >= pi || (alpha_2 - alpha_1) <= -1*pi) {
		y_R = -1.0;
	}
	else if ((alpha_3 - alpha_2) >= pi || (alpha_3 - alpha_2) <= -1*pi) {
		y_R = -1.0;
	}
	else {
		y_R = 0.0;
	}
	/*
    float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;
    */
    x1_p = x1 - x2;
    y1_p = y1 - y2;
    x2_p = 0;
    y2_p = 0;
    x3_p = x3 - x2;
    y3_p = y3 - y2;
    
    T_12 = cos(alpha_2 - alpha_1)/sin(alpha_2 - alpha_1);
    T_23 = cos(alpha_3 - alpha_2)/sin(alpha_3 - alpha_2);
    T_31 = (1 - (T_12 * T_23))/(T_12 + T_23);
    
    x_12 = x1_p + (T_12*y1_p);
    y_12 = y1_p - (T_12*x1_p);
    x_23 = x3_p - (T_23*y1_p);
    y_23 = y3_p + (T_23*x1_p);
    x_31 = (x1_p + x3_p) + (T_31*(y3_p - y1_p));
    y_31 = (y1_p + y3_p) - (T_31*(x3_p - x1_p));
    
    k_31 = (x1_p*x3_p)+(y1_p*y3_p)+(T_31*((x1_p*y3_p)-(x3_p*y1_p))); 
    D = ((x_12-x_23)*(y_23-y_31)) - ((y_12-y_23)*(x_23-x_31));
    
    y_R = y2 + ((k_31*(x_23-x_12))/D);
    
    return (y_R);
}

float heading(float steps1, float steps2, float steps3) {

	float deg1 = steps1/STEPS_PER_DEG;
	float deg2 = steps2/STEPS_PER_DEG;
	float deg3 = steps3/STEPS_PER_DEG;

    float alpha_1 = deg1 * (pi/180);
    float alpha_2 = deg2 * (pi/180);
    float alpha_3 = deg3 * (pi/180);
    
    float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;

	float L_12[2] = {x1-x2,y1-y2};
	float L_31[2] = {x3-x1,y3-y1};
	float x_axis[2] = {1,0};
	
	float Mag_L12;
	float Mag_L31;
	float dot_product_12;
	float dot_product_31;
	
	float phi;
	float sigma;
	
	float lambda_12;
	float lambda_31;
	
	float gamma;
	float p;
	float tau;
    float dir;

	Mag_L12 = sqrt(pow(L_12[0],2) + pow(L_12[1],2));
	Mag_L31 = sqrt(pow(L_31[0],2) + pow(L_31[1],2));
	dot_product_12 = ((L_12[0] * x_axis[0]) + (L_12[1] * x_axis[1]));
	dot_product_31 = ((L_31[0] * x_axis[0]) + (L_31[1] * x_axis[1]));
	
	phi = acos(dot_product_12/(Mag_L12));
	sigma = phi + acos(dot_product_31/(Mag_L31));
	
	lambda_12 = alpha_2 - alpha_1;
	lambda_31 = alpha_1 - alpha_3;
	
	if (alpha_3 > alpha_1) {
		lambda_31 = lambda_31 + (2*pi);
	}

	if (alpha_1 > alpha_2) {
		lambda_12 = lambda_12 + (2*pi);
	}
	
	gamma = sigma - lambda_31;
	p = (Mag_L31*sin(lambda_12))/(Mag_L12*sin(lambda_31));
	tau = atan((sin(lambda_12)-(p*sin(gamma)))/((p*cos(gamma))-(cos(lambda_12))));

	if ((lambda_12 < pi) && (tau < 0.0)) {
		tau = tau + pi;
	}
	else if ((lambda_12 > pi) && (tau > 0.0)) {
		tau = tau - pi;
	}
	
	
    dir = tau + phi - alpha_1;
	dir = dir * (180/pi);
	if (dir <= -180) {
		dir += 360;
	}
	else if (dir > 180) {
		dir = dir - 360;
	}

	return(dir);
}

void getWaypointHeading(float normalizedX, float normalizedY, float *relNormAngle){
    if(normalizedX==0){
        if(normalizedY>0){*relNormAngle= (float)90;}
        else {*relNormAngle= (float)270;}
    }
    else if(normalizedY==0){
        if(normalizedX>0){*relNormAngle= (float)0;}
        else {*relNormAngle= (float)180;}
    }
    else if(normalizedY>0){
        if(normalizedX>0){
            *relNormAngle= (float)(atan(normalizedY/normalizedX)*(180/pi));
        }
        else{
            *relNormAngle= (float)(180-atan(normalizedY/normalizedX)*(180/pi));
        }
    }
    else {
        if(normalizedX>0){
            *relNormAngle= (float)(360+atan(normalizedY/normalizedX)*(180/pi));
        }
        else{
            *relNormAngle= (float)(180+atan(normalizedY/normalizedX)*(180/pi));
        }
    }
    return;
}

void getLocAndHeading(float steps1, float steps2, float steps3, float dataArray[]){
    
	float x_R;
	float deg1 = steps1/STEPS_PER_DEG;
	float deg2 = steps2/STEPS_PER_DEG;
	float deg3 = steps3/STEPS_PER_DEG;

    float alpha_1 = deg1 * (pi/180);
    float alpha_2 = deg2 * (pi/180);
    float alpha_3 = deg3 * (pi/180);

	float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;
    
    float x1_p;
    float y1_p;
    float x2_p;
    float y2_p;
    float x3_p;
    float y3_p;
    
    float T_12;
    float T_23;
    float T_31;
    
    float x_12;
    float y_12;
    float x_23;
    float y_23;
    float x_31;
    float y_31;
    
    float k_31; 
    float D;

	if ((alpha_2 - alpha_1) >= pi || (alpha_2 - alpha_1) <= -1*pi) {
		x_R = -1.0;
	}
	else if ((alpha_3 - alpha_2) >= pi || (alpha_3 - alpha_2) <= -1*pi) {
		x_R = -1.0;
	}
	else {
		x_R = 0.0;
	}
    /*
    float x1 = T1x;
    float y1 = T1y;
    float x2 = T2x;
    float y2 = T2y;
    float x3 = T3x;
    float y3 = T3y;*/
    
    x1_p = x1 - x2;
    y1_p = y1 - y2;
    x2_p = 0;
    y2_p = 0;
    x3_p = x3 - x2;
    y3_p = y3 - y2;
    
    T_12 = cos(alpha_2 - alpha_1)/sin(alpha_2 - alpha_1);
    T_23 = cos(alpha_3 - alpha_2)/sin(alpha_3 - alpha_2);
    T_31 = (1 - (T_12 * T_23))/(T_12 + T_23);
    
    x_12 = x1_p + (T_12*y1_p);
    y_12 = y1_p - (T_12*x1_p);
    x_23 = x3_p - (T_23*y1_p);
    y_23 = y3_p + (T_23*x1_p);
    x_31 = (x1_p + x3_p) + (T_31*(y3_p - y1_p));
    y_31 = (y1_p + y3_p) - (T_31*(x3_p - x1_p));
    
    k_31 = (x1_p*x3_p)+(y1_p*y3_p)+(T_31*((x1_p*y3_p)-(x3_p*y1_p))); 
    D = ((x_12-x_23)*(y_23-y_31)) - ((y_12-y_23)*(x_23-x_31));
    
    dataArray[0] = x2 + ((k_31*(y_12-y_23))/D);
    dataArray[1] = y2 + ((k_31*(x_23-x_12))/D);
    
    
	float L_12[2] = {x1-x2,y1-y2};
	float L_31[2] = {x3-x1,y3-y1};
	float x_axis[2] = {1,0};
	
	float Mag_L12;
	float Mag_L31;
	float dot_product_12;
	float dot_product_31;
	
	float phi;
	float sigma;
	
	float lambda_12;
	float lambda_31;
	
	float gamma;
	float p;
	float tau;
    float dir;

	Mag_L12 = sqrt(pow(L_12[0],2) + pow(L_12[1],2));
	Mag_L31 = sqrt(pow(L_31[0],2) + pow(L_31[1],2));
	dot_product_12 = ((L_12[0] * x_axis[0]) + (L_12[1] * x_axis[1]));
	dot_product_31 = ((L_31[0] * x_axis[0]) + (L_31[1] * x_axis[1]));
	
	phi = acos(dot_product_12/(Mag_L12));
	sigma = phi + acos(dot_product_31/(Mag_L31));
	
	lambda_12 = alpha_2 - alpha_1;
	lambda_31 = alpha_1 - alpha_3;
	
	if (alpha_3 > alpha_1) {
		lambda_31 = lambda_31 + (2*pi);
	}

	if (alpha_1 > alpha_2) {
		lambda_12 = lambda_12 + (2*pi);
	}
	
	gamma = sigma - lambda_31;
	p = (Mag_L31*sin(lambda_12))/(Mag_L12*sin(lambda_31));
	tau = atan((sin(lambda_12)-(p*sin(gamma)))/((p*cos(gamma))-(cos(lambda_12))));

	if ((lambda_12 < pi) && (tau < 0.0)) {
		tau = tau + pi;
	}
	else if ((lambda_12 > pi) && (tau > 0.0)) {
		tau = tau - pi;
	}
	
	
    dir = tau + phi - alpha_1;
	dir = dir * (180/pi);
	if (dir <= -180) {
		dir += 360;
	}
	else if (dir > 180) {
		dir = dir - 360;
	}

	dataArray[2] = dir;
    return;
}

int towerIdentification (float tower1, float tower2, float tower3, float unknownTower){
    float tower1Diff = fabsf(angleToTarget(unknownTower-tower1));
    float tower2Diff = fabsf(angleToTarget(unknownTower-tower2));
    float tower3Diff = fabsf(angleToTarget(unknownTower-tower3));
    int identifiedTower = 1;
    
    if((tower1Diff<tower2Diff)&&(tower1Diff<tower3Diff)){identifiedTower = 1;}
    if((tower2Diff<tower1Diff)&&(tower2Diff<tower3Diff)){identifiedTower = 2;}
    if((tower3Diff<tower1Diff)&&(tower3Diff<tower2Diff)){identifiedTower = 3;}
    
    return identifiedTower;
}

float angleToTarget (float target, float source){
    return (target-source+540.0)%360-180;
}