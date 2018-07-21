/*
Filename: Warrior.h
Author: Guo Hao
Date: 2018/6/27
Description: warrior class
*/

#pragma once
#include <vector>
#include "Message.h"
using namespace std;

enum types{dragon,ninja,iceman,lion,wolf};

class Warrior
{
	int id;//positive-Red negative-Blue
	int type;
	int att;
	int life;
	int position;
	int att_kill;//times of first attack & kill
public:
	Warrior(int _type,int color); //color:1-red 0-blue
	void Proceed();
	int Getid();
	int Getpos();
	int Gettype();
	int& Getlife();
	WarriorInfo Getinfo();//for message
	void Att_kill();
	void Getelements(int city,int casenum);//case 1:no battle; case 2:after battle;
	void Attack(Warrior&);
	void Fightback(Warrior&);
	void Yell();
};

