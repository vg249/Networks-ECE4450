#include <stdio.h>


#define num_nodes 4
#define nodeid 3

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
int distTable3[num_nodes][num_nodes]; //Distance table which will hold the distance through the different iterartions
int neighbors3[3] = {0,2};
int forwardTable3[4] = {0,999,2,3};
int fdUpdtStatus3[4] = {0,0,0,0};

void rtinit3()
{
	struct rtpkt packet;
	int i = 0, j = 0;
	
	//Initialize the distance table with Max values
	for(i = 0; i < num_nodes; i++)
	{

		packet.mincost[i] = INT_MAX; //initialize the mincost
		for(j = 0; j < num_nodes; j++)
		{
			distTable3[i][j] = INT_MAX;
		}
	}	

	//Updating the minimum cost for neighbors3.
	packet.mincost[0] = 7;
	packet.mincost[2] = 2;
	packet.mincost[3] = 0;
	
	//Update distance table for each neighbor
	distTable3[nodeid][0] = 7;
	distTable3[nodeid][2] = 2;
	distTable3[nodeid][3] = 0;

	//sending the packet with distance to neighbors3
	for(i = 0; i < len(neighbors3); i++){
		packet.sourceid = nodeid;
		packet.destid   = neighbors3[i];
		tolayer2(packet);
	}
}

void rtfinalize3()
{
	int i = 0;
	printf("\n  Node %d table\n",nodeid);
	printf("            costto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",distTable3[nodeid][i]);
	}
	printf("\n\n");
	printf("            forwardto            \n");
	printf("   |   0   1   2   3\n");
	printf("---|----------------------\n");
	printf("%d  |",nodeid);
	for(i = 0; i < num_nodes; i++){
		printf("   %d",forwardTable3[i]);
	}
	printf("\n\n");

}

void rtupdate3(struct rtpkt *rcvdpkt)
{
	int i;
	int costChange = 0;
	struct rtpkt packet;
	
	for(i = 0; i < num_nodes;i++)
	{
		int cost = rcvdpkt->mincost[i]+distTable3[nodeid][rcvdpkt->sourceid];
		if(cost < distTable3[nodeid][i])
		{
			costChange = 1;
			distTable3[nodeid][i] = cost;				
			if(fdUpdtStatus3[rcvdpkt->sourceid] == 0)
			{				
				forwardTable3[i] = rcvdpkt->sourceid;
				fdUpdtStatus3[i] = 1;				
			}
			else{
				forwardTable3[i] = forwardTable3[rcvdpkt->sourceid];
			}
		}
		
		distTable3[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];				
		
	}
	
	
	if(costChange == 1)
	{
		for(i = 0; i < len(neighbors3); i++){
			
			packet.sourceid = nodeid;
			packet.destid   = neighbors3[i];
			
			packet.mincost[0] = distTable3[nodeid][0]; 
			packet.mincost[1] = distTable3[nodeid][1]; 
			packet.mincost[2] = distTable3[nodeid][2]; 
			packet.mincost[3] = distTable3[nodeid][3]; 
			
			tolayer2(packet);
		}
		
	      	
	}
	
}
