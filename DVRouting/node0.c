#include <stdio.h>


#define num_nodes 4
#define nodeid 0
#define INT_MAX 9999

#define len(a) (sizeof(a) / sizeof(*a))

struct rtpkt
{
	int sourceid;       /* id of node sending this pkt */
	int destid;         /* id of router to which pkt being sent
						(must be an immediate neighbor) */
	int mincost[4];     /* min cost to node 0 ... 3 */
};


void tolayer2(struct rtpkt packet);

/* please complete the following three routines for part 1 */

int distTable0[num_nodes][num_nodes]; //Distance table which will hold the distance through the different iterartions
static int neighbors0[3] = {1,2,3};
int forwardTable0[4] = {0,1,2,3};
int fdUpdtStatus0[4] = {0,0,0,0};
int linkcost0[4] = {0,1,3,7};

void rtinit0()
{
	
	struct rtpkt packet;
	int i = 0, j = 0;
	
	//Initialize the distance table with Max values
	for(i = 0; i < num_nodes; i++)
	{

		packet.mincost[i] = INT_MAX; //initialize the mincost
		for(j = 0; j < num_nodes; j++)
		{
			distTable0[i][j] = INT_MAX;
		}
	}	

	//Updating the minimum cost for neighbors0.
	packet.mincost[0] = 0;
	packet.mincost[1] = 1;
	packet.mincost[2] = 3;
	packet.mincost[3] = 7;

	//Update distance table for each neighbor
	distTable0[nodeid][0] = 0;
	distTable0[nodeid][1] = 1;
	distTable0[nodeid][2] = 3;
	distTable0[nodeid][3] = 7;

	//sending the packet with distance to neighbors0
	for(i = 0; i < len(neighbors0); i++){
		packet.sourceid = nodeid;
		packet.destid   = neighbors0[i];
		tolayer2(packet);
	}
			
}

void rtfinalize0()
{
	int i = 0;
	printf("\n  Node %d table\n",nodeid);
	printf("            costto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",distTable0[nodeid][i]);
	}
	printf("\n\n");
	printf("            forward to            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",forwardTable0[i]);
	}
	printf("\n\n");
	
}

void rtupdate0(struct rtpkt *rcvdpkt)
{
	int i;
	int costChange = 0;
	struct rtpkt packet;
	
	for(i = 0; i < num_nodes;i++)
	{
		int cost = rcvdpkt->mincost[i]+distTable0[nodeid][rcvdpkt->sourceid];
		if(cost < distTable0[nodeid][i])
		{
			costChange = 1;
			distTable0[nodeid][i] = cost;
			if(fdUpdtStatus0[rcvdpkt->sourceid] == 0)
			{				
				forwardTable0[i] = rcvdpkt->sourceid;
				fdUpdtStatus0[i] = 1;				
			}
			else{
				forwardTable0[i] = forwardTable0[rcvdpkt->sourceid];
			}
		}
		
		distTable0[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];				
		
	}
	
	
	if(costChange == 1)
	{
		for(i = 0; i < len(neighbors0); i++){
			
			packet.sourceid = nodeid;
			packet.destid   = neighbors0[i];
			
			packet.mincost[0] = distTable0[nodeid][0]; 
			packet.mincost[1] = distTable0[nodeid][1]; 
			packet.mincost[2] = distTable0[nodeid][2]; 
			packet.mincost[3] = distTable0[nodeid][3]; 
			
			tolayer2(packet);
		}
		
	      	
	}

}

/* please complete the following routine for part 2 */

void rtlinkhandler0(int linkid, int newcost)
{
	int i,oldcost, costChange = 0,j;
	struct rtpkt packet;
	oldcost = linkcost0[linkid];
	linkcost0[linkid] = newcost;


	for(i = 0; i < num_nodes; i++)
	{
		if(forwardTable0[i] == linkid)
		{
			if(linkcost0[i] >  distTable0[nodeid][i] - oldcost + newcost)
				distTable0[nodeid][i]  = distTable0[nodeid][i] - oldcost + newcost;
			else
				distTable0[nodeid][i]  = linkcost0[i];
				
		}
		distTable0[linkid][i]  = distTable0[linkid][i] - oldcost + newcost;
		
	}


	for(i = 0; i < len(neighbors0); i++){
		
		packet.sourceid = nodeid;
		packet.destid   = neighbors0[i];
		
		packet.mincost[0] = distTable0[nodeid][0]; 
		packet.mincost[1] = distTable0[nodeid][1]; 
		packet.mincost[2] = distTable0[nodeid][2]; 
		packet.mincost[3] = distTable0[nodeid][3]; 
		
		tolayer2(packet);
	}
}
