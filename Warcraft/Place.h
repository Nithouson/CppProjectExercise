/*
Filename: Place.h
Author: Guo Hao
Date: 2018/6/27
Description: place class, and subclass city & headquarter
*/

#pragma once
#include "Warrior.h"
const int RedCycle[5] = {iceman,lion,wolf,ninja,dragon },
	BlueCycle[5] = { lion,dragon,ninja,iceman,wolf };

class Place
{
protected:
	int num;//RedHQ-0 BlueHQ-n+1 other-num of city
	int elements; //elements remains
	int flag;//-1-unset 1-Red 0-Blue
public:
	Place(int _num);
	void Setflag(int);
	int Getelements();
};

class Headquarter :public Place
{
	int cyclenum;//record which type of warrior to create
	int enemies;//enemies in headquarter
public:
	Headquarter(int color);//1-Red 0-Blue
	void Create_Warrior();
	void Init();
	void Invaded();
	void Addelements(int);
	void Award(Warrior&);
	void Report();
};

class City:public Place
{
	int lastres;//-1:none 1:red kills blue 0:blue kills red
public:
	City(int _num);
	void Produce_elements();
	int Elements_taken();//return the amount of elements 
	int Battle(Warrior& red, Warrior&blue);//-1-no death 0-Red dead 1-Blue dead
};