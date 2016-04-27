#include <stdio.h>


#define num_nodes 4
#define nodeid 2
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
int distTable2[num_nodes][num_nodes]; //Distance table which will hold the distance through the different iterartions
int neighbors2[3] = {0,1,3};
int forwardTable2[4] = {0,1,2,3};
int fdUpdtStatus2[4] = {0,0,0,0};

void rtinit2()
{
	struct rtpkt packet;
	int i = 0, j = 0;
	
	//Initialize the distance table with Max values
	for(i = 0; i < num_nodes; i++)
	{

		packet.mincost[i] = INT_MAX; //initialize the mincost
		for(j = 0; j < num_nodes; j++)
		{
			distTable2[i][j] = INT_MAX;
		}
	}


	//Updating the minimum cost for neighbors2.
	packet.mincost[0] = 3; 
	packet.mincost[1] = 1;
	packet.mincost[2] = 0;
	packet.mincost[3] = 2;

	//Update distance table for each neighbor
	distTable2[nodeid][0] = 3;
	distTable2[nodeid][1] = 1;
	distTable2[nodeid][2] = 0;
	distTable2[nodeid][3] = 2;

	//sending the packet with distance to neighbors2
	for(i = 0; i < len(neighbors2); i++){
		packet.sourceid = nodeid;
		packet.destid   = neighbors2[i];
		tolayer2(packet);
	}
}

void rtfinalize2()
{
	int i = 0;
	printf("\n  Node %d table\n",nodeid);
	printf("            costto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",distTable2[nodeid][i]);
	}
	printf("\n\n");
	printf("            forwardto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",forwardTable2[i]);
	}
	printf("\n\n");
}

void rtupdate2(struct rtpkt *rcvdpkt)
{
	int i;
	int costChange = 0;
	struct rtpkt packet;
	
	for(i = 0; i < num_nodes;i++)
	{
		int cost = rcvdpkt->mincost[i]+distTable2[nodeid][rcvdpkt->sourceid];
		if(cost < distTable2[nodeid][i])
		{
			costChange = 1;
			distTable2[nodeid][i] = cost;				
			if(fdUpdtStatus2[rcvdpkt->sourceid] == 0)
			{					
				forwardTable2[i] = rcvdpkt->sourceid;
				fdUpdtStatus2[i] = 1;				
			}
			else{
				forwardTable2[i] = forwardTable2[rcvdpkt->sourceid];
			}
		}
		
		distTable2[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];				
		
	}
	
	
	if(costChange == 1)
	{
		for(i = 0; i < len(neighbors2); i++){
			
			packet.sourceid = nodeid;
			packet.destid   = neighbors2[i];
			
			packet.mincost[0] = distTable2[nodeid][0]; 
			packet.mincost[1] = distTable2[nodeid][1]; 
			packet.mincost[2] = distTable2[nodeid][2]; 
			packet.mincost[3] = distTable2[nodeid][3]; 
			
			tolayer2(packet);
		}
		
	      	
	}


}
