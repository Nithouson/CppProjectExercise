/*
Filename: Message.h
Author: Guo Hao
Date: 2018/6/27
Description: message class
*/

#pragma once

struct WarriorInfo
{
	int type, color, id, life, att;
	WarriorInfo() {};
	WarriorInfo(int _type, int _color, int _id, int _life, int _att)
	{
		type = _type; color = _color; id = _id; life = _life; att = _att;
	}
};

struct Message
{
	int type, time, place, color,param;
	//message type is in accordance with the 11 types in the description;
	WarriorInfo w1, w2;
	//constructors
	Message(int _type, int _time, int _place, int _color, int _param = 0)
	{
		type = _type; time = _time; place = _place; color = _color; param = _param;
	}
	Message(int _type, int _time, int _place, int _color, WarriorInfo _w1, int _param = 0)
	{
		type = _type; time = _time; place = _place; color = _color; param = _param;
		w1 = _w1;
	}
	Message(int _type, int _time, int _place, int _color, WarriorInfo _w1,
		WarriorInfo _w2, int _param = 0)
	{
		type = _type; time = _time; place = _place; color = _color; param = _param;
		w1 = _w1; w2 = _w2;
	}
	//comparator,for output order
	bool operator<(const Message& arg)
	{
		if (time != arg.time) return time < arg.time;
		else if (place != arg.place) return place < arg.place;
		else if (type != arg.type) return type < arg.type;
		else if(color != arg.color) return color==1;
		else return false;
	}
};