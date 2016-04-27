#include <stdio.h>

#define num_nodes 4
#define nodeid 1
#define len(a) (sizeof(a) / sizeof(*a))
#define INT_MAX 9999

struct rtpkt
{
	int sourceid;       /* id of node sending this pkt */
	int destid;         /* id of router to which pkt being sent
						(must be an immediate neighbor) */
	int mincost[4];     /* min cost to node 0 ... 3 */
};

void tolayer2(struct rtpkt packet);

/* please complete the following three routines for part 1 */

int distTable1[num_nodes][num_nodes]; //Distance table which will hold the distance through the different iterartions
int neighbors1[2] = {0,2};
int forwardTable1[4] = {0,1,2,999};
int fdUpdtStatus1[4] = {0,0,0,0};
int linkcost1[4] = {1,0,1,INT_MAX};

void rtinit1()
{
	struct rtpkt packet;
	int i = 0, j = 0;
	
	//Initialize the distance table with Max values
	for(i = 0; i < num_nodes; i++)
	{

		packet.mincost[i] = INT_MAX; //initialize the mincost
		for(j = 0; j < num_nodes; j++)
		{
			distTable1[i][j] = INT_MAX;
		}
	}
	
	//Updating the minimum cost for neighbors1.
	packet.mincost[0] = 1;
	packet.mincost[1] = 0;
	packet.mincost[2] = 1;

	//Update distance table for each neighbor
	distTable1[nodeid][0] = 1;
	distTable1[nodeid][1] = 0;
	distTable1[nodeid][2] = 1;

	//sending the packet with distance to neighbors1
	for(i = 0; i < len(neighbors1); i++){
		packet.sourceid = nodeid;
		packet.destid   = neighbors1[i];
		tolayer2(packet);
	}
		
}

void rtfinalize1()
{
	int i = 0;
	printf("\n  Node %d table\n",nodeid);
	printf("            costto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",distTable1[nodeid][i]);
	}
	printf("\n\n");
	printf("            forwardto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",forwardTable1[i]);
	}
	printf("\n\n");

}

void rtupdate1(struct rtpkt *rcvdpkt)
{
	int i;
	int costChange = 0;
	struct rtpkt packet;
	
	for(i = 0; i < num_nodes;i++)
	{
		int cost = rcvdpkt->mincost[i]+distTable1[nodeid][rcvdpkt->sourceid];
		if(cost < distTable1[nodeid][i])
		{
			costChange = 1;
			distTable1[nodeid][i] = cost;				
			if(fdUpdtStatus1[rcvdpkt->sourceid] == 0)
			{					
				forwardTable1[i] = rcvdpkt->sourceid;
				fdUpdtStatus1[i] = 1;				
			}
			else{
				forwardTable1[i] = forwardTable1[rcvdpkt->sourceid];
			}
		}
		
		distTable1[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];				
		
	}
	
	
	if(costChange == 1)
	{
		for(i = 0; i < len(neighbors1); i++){
			
			packet.sourceid = nodeid;
			packet.destid   = neighbors1[i];
			
			packet.mincost[0] = distTable1[nodeid][0]; 
			packet.mincost[1] = distTable1[nodeid][1]; 
			packet.mincost[2] = distTable1[nodeid][2]; 
			packet.mincost[3] = distTable1[nodeid][3]; 
			
			tolayer2(packet);
		}
	}

}

/* please complete the following routine for part 2 */

void rtlinkhandler1(int linkid, int newcost)
{
	int i,oldcost, costChange = 0,j;
	struct rtpkt packet;
	oldcost = linkcost1[linkid];
	linkcost1[linkid] = newcost;
		
	for(i = 0; i < num_nodes; i++)
	{
		if(forwardTable1[i] == linkid)
		{
			if(linkcost1[i] >  distTable1[nodeid][i] - oldcost + newcost)
				distTable1[nodeid][i]  = distTable1[nodeid][i] - oldcost + newcost;
			else
				distTable1[nodeid][i]  = linkcost1[i];
				
		}
		distTable1[linkid][i]  = distTable1[linkid][i] - oldcost + newcost;
		
	}

	
	for(i = 0; i < len(neighbors1); i++){
		
		packet.sourceid = nodeid;
		packet.destid   = neighbors1[i];
		
		packet.mincost[0] = distTable1[nodeid][0]; 
		packet.mincost[1] = distTable1[nodeid][1]; 
		packet.mincost[2] = distTable1[nodeid][2]; 
		packet.mincost[3] = distTable1[nodeid][3]; 
		
		tolayer2(packet);
	}
	
	
}
