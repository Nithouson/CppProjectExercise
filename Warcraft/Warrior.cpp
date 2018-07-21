/*
Filename: Warrior.cpp
Author: Guo Hao
Date: 2018/6/27
Description: implementaion of warrior class
*/

#include "Warrior.h"
#include "Place.h"
#include "Message.h"

extern int oriatt[5], orilife[5];
extern int cities;
extern vector<Warrior> warriors;
extern vector<City> cityline;
extern vector<Message> messagequeue;
extern int hour;
extern Headquarter RedHQ, BlueHQ;
extern int rednum, bluenum;

Warrior::Warrior(int _type,int color)
{
	id = color ? rednum : -bluenum;
	type = _type;
	att = oriatt[type];
	life = orilife[type];
	position = color ? 0 : cities + 1;
	att_kill = 0;
}

void Warrior::Proceed()
{
	if (id > 0) //Red Warrior
	{
		if (position == cities + 1) return;//already in enemy's headquarter
		else 
		{ 
			position++; 
			if (position % 2 == 0 && type == iceman)
			{
				att += 20;
				life = life >= 10 ? life - 9 : 1;
			}
			if (position == cities+1)//reach headquarter
			{
				messagequeue.push_back(Message(9, hour * 60 + 10, position, 1, Getinfo()));
				BlueHQ.Invaded();
			}
			else
			{
				messagequeue.push_back(Message(2, hour * 60 + 10, position, 1, Getinfo()));
			}
		}

	}
	else
	{
		if (position == 0) return;
		else
		{
			position--;
			if ((cities-position) % 2 == 1 && type == iceman)
			{
				att += 20;
				life = life >= 10 ? life - 9 : 1;
			}
			if (position== 0)
			{
				messagequeue.push_back(Message(9, hour * 60 + 10, position, 0, Getinfo()));
				RedHQ.Invaded();
			}
			else
			{
				messagequeue.push_back(Message(2, hour * 60 + 10, position, 0, Getinfo()));
			}
		}
	}
}


int Warrior::Getpos()
{
	return position;
}

int Warrior::Getid()
{
	return id;
}

int& Warrior::Getlife()
{
	return life;
}

int Warrior::Gettype()
{
	return type;
}

WarriorInfo Warrior::Getinfo()
{
	int color = id > 0 ? 1 : 0;
	int _id = id > 0 ? id : -id;
	return WarriorInfo(type, color, _id, life, att);
}

void Warrior::Getelements(int city,int casenum) 
{
	int earn = cityline[city - 1].Elements_taken();//take all elements
	if (id > 0) RedHQ.Addelements(earn);
	else BlueHQ.Addelements(earn);
	messagequeue.push_back(Message(7, casenum==1?(hour*60+30):(hour*60+40), city, 
		id > 0 ? 1 : 0, Getinfo(), earn));
	
}

void Warrior::Attack(Warrior& enemy)
{
	enemy.Getlife() -= att;
	messagequeue.push_back(Message(3, hour * 60 + 40, position, id>0?1:0, Getinfo(), enemy.Getinfo()));
}

void Warrior::Fightback(Warrior& enemy)
{
	enemy.Getlife() -= att/2;
	messagequeue.push_back(Message(4, hour * 60 + 40, position, id>0?1:0, Getinfo(), enemy.Getinfo()));
}

void Warrior::Yell()
{
	messagequeue.push_back(Message(6, hour * 60 + 40, position, id > 0 ? 1 : 0, Getinfo()));
}

void Warrior::Att_kill()
{
	att_kill++;
	if (type == wolf&&att_kill % 2 == 0)
	{
		att *= 2;
		life *= 2;
	}
}
