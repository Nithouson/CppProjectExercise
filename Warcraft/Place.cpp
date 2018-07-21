/*
Filename: Place.cpp
Author: Guo Hao
Date: 2018/6/27
Description: implementation of place class, and subclass city & headquarter
*/

#include "Place.h"
#include "Warrior.h"

extern bool war_end;
extern int cities, orielements;
extern int oriatt[5], orilife[5];
extern vector<Warrior> warriors;
extern vector<Message> messagequeue;
extern int hour, rednum, bluenum;

Place::Place(int _num)
{
	num = _num;
}

void Place::Setflag(int color)
{
	flag = color;
	if (num > 0 && num <= cities)//city
	{
		messagequeue.push_back(Message(8, hour * 60 + 40, num, color));
	}
}

int Place::Getelements()
{
	return elements;
}

Headquarter::Headquarter(int color) :Place(color==1? 0 : cities + 1)
{
	elements = orielements;
	flag = color ? 1 : 0;
	cyclenum = 0;
	enemies = 0;
}

void Headquarter::Init()
{
	num = flag== 1 ? 0 : cities + 1;
	elements = orielements;
	cyclenum = 0;
	enemies = 0;
}

void Headquarter::Create_Warrior()
{
	if (flag == 1) //RedHQ
	{
		if (elements < orilife[RedCycle[cyclenum]]) return;
		else
		{
			Warrior nwar = Warrior(RedCycle[cyclenum], 1);
			warriors.push_back(nwar);
			elements -= orilife[RedCycle[cyclenum]];
			cyclenum=(cyclenum+1)%5; rednum++;
			messagequeue.push_back(Message(1, hour * 60, 0, 1, nwar.Getinfo()));
		}
	}
	else if (flag == 0)//BlueHQ
	{
		if (elements < orilife[BlueCycle[cyclenum]]) return;
		else
		{
			Warrior nwar = Warrior(BlueCycle[cyclenum], 0);
			warriors.push_back(nwar);
			elements -= orilife[BlueCycle[cyclenum]];
			cyclenum = (cyclenum + 1) % 5; bluenum++;
			messagequeue.push_back(Message(1, hour * 60, cities+1, 0, nwar.Getinfo()));
		}
	}
}

void Headquarter::Invaded()
{
	enemies++;
	if (enemies == 2)
	{
		war_end = true;//headquarter taken
		messagequeue.push_back(Message(10, hour * 60 + 10, num, flag));
	}
}

void Headquarter::Addelements(int q)
{
	elements += q;
}

void Headquarter::Award(Warrior& war)
{
	if (elements < 8) return;
	war.Getlife() += 8;
	elements -= 8;
}

void Headquarter::Report(void)
{
	messagequeue.push_back(Message(11, hour * 60 + 50, num, flag, elements));
}

City::City(int _num):Place(_num)
{
	elements = 0;
	flag = -1;
	lastres = -1;
}

void City::Produce_elements()
{
	elements += 10;
}

int City::Elements_taken()
{
	int got = elements;
	elements = 0;
	return got;
}

int City::Battle(Warrior& red, Warrior& blue)
{
	int res=-1;//-1-no death 1-blue dead 0-red dead
	int prelifeR = red.Getlife(), prelifeB = blue.Getlife();//life before battle
	if (flag == 1 || (flag == -1 && num % 2 == 1))
	{
		red.Attack(blue);
		if (blue.Getlife ()<= 0)//dead
		{
			res=1;
			messagequeue.push_back(Message(5, hour * 60 + 40, num, 0, blue.Getinfo()));
			red.Att_kill();
		}
		else if(blue.Gettype()!=ninja)//fightback
		{
			blue.Fightback(red);
			if (red.Getlife ()<= 0)
			{
				res=0;
				messagequeue.push_back(Message(5, hour * 60 + 40, num, 1,red.Getinfo()));
			}
		}
		if (red.Gettype() ==dragon&&red.Getlife()>0)
		{
			red.Yell();
		}
	}
	else
	{
		blue.Attack(red);
		if (red.Getlife() <= 0)//dead
		{
			res=0;
			messagequeue.push_back(Message(5, hour * 60 + 40, num, 1, red.Getinfo()));
			blue.Att_kill();
		}
		else if(red.Gettype()!=ninja)//fightback
		{
			red.Fightback(blue);
			if (blue.Getlife() <= 0)
			{
				res=1;
				messagequeue.push_back(Message(5, hour * 60 + 40, num, 0, blue.Getinfo()));
			}
		}
		if (blue.Gettype() == dragon&&blue.Getlife()>0)
		{
			blue.Yell();
		}
	}
	//lion dead, enemy get life
	if (res == 0 && red.Gettype() == lion) blue.Getlife() += prelifeR;
	if (res == 1 && blue.Gettype() == lion) red.Getlife() += prelifeB;
	if (res!=-1 && res == lastres && res!=flag) Setflag(res);//setflag
	lastres = res;
	return res;
}
