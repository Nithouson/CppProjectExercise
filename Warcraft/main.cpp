/*
Filename: main.cpp
Author: Guo Hao
Date: 2018/6/27
Description: main function(timeline&output)
*/

#include "Place.h" //city/headquarter
#include "Warrior.h"
#include "Message.h"
#include <cstdio>
#include <algorithm>
#pragma warning(disable:4996) //fopen,fscanf,fprintf
using namespace std;

int cities;
int orielements;
int oriatt[5], orilife[5];
int rednum = 1, bluenum = 1;//for assigning id
int hour;
vector<Warrior> warriors;
vector<City> cityline;
vector<Message> messagequeue;
Headquarter RedHQ(1), BlueHQ(0);
bool war_end = false;
const char* strcolor[2] = { "blue","red" };
const char* strtype[5] = { "dragon","ninja","iceman","lion","wolf" };

int main()
{
	FILE* in,* out;
	in = fopen("input.txt", "r");
	out = fopen("output.txt", "w");
	int cases,timelim,hourlim,minlim;
	fscanf(in,"%d", &cases);
	for (int c=1;c<= cases;c++)
	{
		if (c > 1) fprintf(out, "\n");
		fprintf(out, "Case:%d\n", c);
		//read data
		fscanf(in, "%d %d %d", &orielements, &cities, &timelim);
		hourlim = timelim / 60; minlim = timelim % 60;
		fscanf(in, "%d %d %d %d %d", &orilife[0], &orilife[1], &orilife[2], &orilife[3], &orilife[4]);
		fscanf(in, "%d %d %d %d %d", &oriatt[0], &oriatt[1], &oriatt[2], &oriatt[3], &oriatt[4]);
		//initialize
		warriors.clear();
		cityline.clear();
		messagequeue.clear();
		rednum = bluenum = 1;
		RedHQ.Init();
		BlueHQ.Init();
		war_end = false;
		for (int id = 1; id <=cities; id++)
		{
			City city(id);
			cityline.push_back(city);//rebuild cities
		}
		hour = 0;
		while (hour <= hourlim)
		{
			//0 minute:Create warrior
			RedHQ.Create_Warrior();
			BlueHQ.Create_Warrior();
			//10 minute:Proceed
			for (int i = 0; i < warriors.size(); i++)
			{
				warriors[i].Proceed();
			}
			if (war_end) break;
			//20 minute:Produce elements
			for (int i = 0; i < cities; i++)
			{
				cityline[i].Produce_elements();
			}
			//30 minutes:Get elements
			//check warriors in each city,one warrior->get elements
			for (int id = 1; id <= cities; id++)
			{
				int curwarriors = 0,warriorpos;
				for (int i = 0; i < warriors.size(); i++)
				{
					if (warriors[i].Getpos() == id)
					{
						curwarriors++;
						warriorpos = i;
					}
				}
				if (curwarriors == 1)
				{
					warriors[warriorpos].Getelements(id,1);
				}
			}
			//40 minutes:Battle
			vector<int> RedAwardlist,BlueAwardlist;
			//check warriors in each city,two warrior->get elements
			for (int id = 1; id <= cities; id++)
			{
				int curwarriors = 0, redpos,bluepos;
				for (int i = 0; i < warriors.size(); i++)
				{
					if (warriors[i].Getpos() == id)
					{
						curwarriors++;
						if (warriors[i].Getid() > 0) redpos = i;
						else bluepos = i;
					}
				}
				if (curwarriors == 2)
				{
					int res=cityline[id-1].Battle(warriors[redpos], warriors[bluepos]);
					if (res == 1)// blue dead
					{
						RedAwardlist.push_back(warriors[redpos].Getid());
						warriors.erase(warriors.begin() + bluepos);
					}
					else if (res==0) //red dead
					{
						BlueAwardlist.push_back(warriors[bluepos].Getid());
						warriors.erase(warriors.begin() + redpos);
					}
				}
			}
			reverse(RedAwardlist.begin(), RedAwardlist.end());
			//warrior near blue headquarter will be awarded first
			int pos;
			for (int j = 0; j < RedAwardlist.size(); j++)
			{
				for (pos = 0; pos < warriors.size(); pos++)
				{
					if (warriors[pos].Getid() == RedAwardlist[j]) break;
				}
				RedHQ.Award(warriors[pos]);
			}
			for (int k = 0; k < BlueAwardlist.size(); k++)
			{
				for (pos = 0; pos < warriors.size(); pos++)
				{
					if (warriors[pos].Getid() == BlueAwardlist[k]) break;
				}
				BlueHQ.Award(warriors[pos]);
			}
			//one warrior remains in the city->get elements
			for (int id = 1; id <= cities; id++)
			{
				int curwarriors = 0, warriorpos;
				for (int i = 0; i < warriors.size(); i++)
				{
					if (warriors[i].Getpos() == id)
					{
						curwarriors++;
						warriorpos = i;
					}
				}
				if (curwarriors == 1&&cityline[id-1].Getelements()>0)
				{
					warriors[warriorpos].Getelements(id,2);
				}
			}
			//50 minutes:Report
			RedHQ.Report();
			BlueHQ.Report();
			hour++;
		}
		//output
		sort(messagequeue.begin(), messagequeue.end());
		for (int i = 0; i < messagequeue.size(); i++)
		{
			Message cur = messagequeue[i];
			int h = cur.time / 60, min = cur.time % 60;
			if (h == hourlim&&min > minlim) break;
			switch (messagequeue[i].type)
			{
			case 1:
				fprintf(out, "%03d:%02d %s %s %d born\n", h, min, strcolor[cur.color], 
					strtype[cur.w1.type],cur.w1.id);
				break;
			case 2:
				fprintf(out, "%03d:%02d %s %s %d marched to city %d with %d elements"
					" and force %d\n", h, min, strcolor[cur.color], strtype[cur.w1.type],
					cur.w1.id, cur.place, cur.w1.life, cur.w1.att);
				break;
			case 3:
				fprintf(out, "%03d:%02d %s %s %d attacked %s %s %d in city %d"
					" with %d elements and force %d\n", h, min, strcolor[cur.w1.color], 
					strtype[cur.w1.type],cur.w1.id,strcolor[cur.w2.color],
					strtype[cur.w2.type],cur.w2.id,cur.place, cur.w1.life, cur.w1.att);
				break;
			case 4:
				fprintf(out, "%03d:%02d %s %s %d fought back against %s %s %d"
					" in city %d\n", h, min, strcolor[cur.w1.color],
					strtype[cur.w1.type], cur.w1.id, strcolor[cur.w2.color],
					strtype[cur.w2.type], cur.w2.id, cur.place);
				break;
			case 5:
				fprintf(out, "%03d:%02d %s %s %d was killed in city %d\n",
					h, min, strcolor[cur.color],strtype[cur.w1.type], cur.w1.id,
					cur.place);
				break;
			case 6:
				fprintf(out, "%03d:%02d %s %s %d yelled in city %d\n",
					h, min, strcolor[cur.color], strtype[cur.w1.type], cur.w1.id,
					cur.place);
				break;
			case 7:
				fprintf(out, "%03d:%02d %s %s %d earned %d elements for" 
					" his headquarter\n",h, min, strcolor[cur.color], strtype[cur.w1.type],
					cur.w1.id,cur.param);
				break;
			case 8:
				fprintf(out, "%03d:%02d %s flag raised in city %d\n", h, min,
					strcolor[cur.color], cur.place);
				break;
			case 9:
				fprintf(out, "%03d:%02d %s %s %d reached %s headquarter with %d elements"
					" and force %d\n", h, min, strcolor[cur.color], strtype[cur.w1.type],
					cur.w1.id, strcolor[1-cur.color], cur.w1.life, cur.w1.att);
				break;
			case 10:
				fprintf(out, "%03d:%02d %s headquarter was taken\n", h, min, 
					strcolor[cur.color]);
				break;
			case 11:
				fprintf(out, "%03d:%02d %d elements in %s headquarter\n", h, min,
					cur.param,strcolor[cur.color]);
				break;
			}
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}
