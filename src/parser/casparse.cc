#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "math.h"

enum TRANS_STATES{
	TX = 0,
	RX = 1,
	FAILED = 2
};

enum FAIL_STATES{
	NO_INF = 0,	
	WITH_INF = 1,
	BELOW_SENS = 2,
	NOT_IN_RX = 3
};

int tx_count[50] = {0};

FILE * fp;
char * line = NULL;
size_t len = 0;
ssize_t read;

char buf[100];
int node_id;
int STATE;



void getColumn(char *row, int col, char *col_val){

	int col_index = 1;
	int str_len = strlen(row);
	int j=0;

	for(int i=0;i<str_len;i++){

		if(row[i] == ' ')
			continue;
		else
			if(row[i-1] == ' ')
				col_index++;

		if(col_index == col)
			col_val[j++] = row[i];
	}
	// end the string
	col_val[j] = '\0';
}

int getNodeId(char *buf){

	int str_len = strlen(buf);
	int node_id;

	buf[str_len-1] = '\0';

	return atoi(&buf[8]);
}

int getTransType(char* buf){

	if(strcmp(buf,"SET") == 0)
		return TX;
	else if(strcmp(buf,"Received") == 0)
		return RX;
	else if(strcmp(buf,"Failed") == 0)
		return FAILED;
}

int getFailureType(char* buf){

	if(strcmp(buf,"below") == 0){
		return BELOW_SENS;
	}
	else if(strcmp(buf,"NO") == 0)
		return NO_INF;
	else if(strcmp(buf,"radio") == 0)
		return NOT_IN_RX;
	else
		return WITH_INF;
}

int getX(char *buf){

	int str_len = strlen(buf);

	for(int i=0;i<str_len;i++)
		if(buf[i] == ':')
			buf[i] = '\0';

	return atoi(buf);
}

int getY(char *buf){

	int str_len = strlen(buf);
	char str_num[3];

	int j=0;
	for(int i=0;i<str_len;i++)
		if(buf[i] == ':'){
			while(buf[i+1] != ':'){
				str_num[j] = buf[i+1];
				i++;
				j++;
			}
			break;
		}

	return atoi(str_num);
}




void parseMobility(char *file_name){

	FILE *wfp;

	fp =  fopen(file_name, "r");
	wfp = fopen("Mobility.txt","w");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	fprintf(wfp,"\n");
	while ((read = getline(&line, &len, fp)) != -1) {

		// --- Node ID ---//
		getColumn(line,1,buf); 
		fprintf(wfp,"%s ",buf);
		getColumn(line,2,buf);
		fprintf(wfp,"%d ",getNodeId(buf));

		// get (x,y,z)
		getColumn(line,6,buf);
		fprintf(wfp,"%d ",getX(buf));
		getColumn(line,6,buf);
		fprintf(wfp,"%d 0",getY(buf));
		fprintf(wfp,"\n");
	}

	fclose(fp);
	if (line)
		free(line);

}


void parseTopology(char* file_name){

	FILE *wfp;

	fp =  fopen(file_name, "r");
	wfp = fopen("Topology.txt","w");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	fprintf(wfp,"\n");
	while ((read = getline(&line, &len, fp)) != -1) {

		// --- Node ID ---//
		getColumn(line,2,buf); 
		fprintf(wfp,"%d ",getNodeId(buf));

		// get (x,y,z)
		getColumn(line,6,buf);
		for(int i=0;i<strlen(buf);i++)
			if(buf[i] == ':')
				buf[i] = ' ';

		fprintf(wfp,"%s",buf);
	//	fprintf(wfp,"\n");
	}

	fclose(fp);
	if (line)
		free(line);
}

void parsePDR(char* file_name){

	fp = fopen(file_name, "r");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	double pdr = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);

		getColumn(line,2,buf);
		if(getNodeId(buf) != 0){
			// --- Simulation Time ---//
			getColumn(line,1,buf); 
			printf("%s",buf);


			// --- get energy value ---
			getColumn(line,4,buf);
			printf(" %s",buf);
			//printf("\n");

		}
	}


	fclose(fp);
	if (line)
		free(line);
}



void parseEnergyConsumption(char* file_name, int numNodes){

	fp = fopen(file_name, "r");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	double energy = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);

		getColumn(line,2,buf);
		if(getNodeId(buf) > 0){
			// --- Simulation Time ---//
			getColumn(line,1,buf); 
			printf("%f",atof(buf));


			// --- get energy value ---
			getColumn(line,10,buf);
			energy += (atof(buf)*0.001)/(double)numNodes;
			//energy += atof(buf)/(double)numNodes;
			printf(" %f",energy);
			printf("\n");


		}
	}

	printf("\n#------- Energy:0  %f W --------\n",energy);

	fclose(fp);
	if (line)
		free(line);
}




void parseRadioCommunication(char* file_name){

	FILE *wfp;

	fp =  fopen(file_name, "r");
	wfp = fopen("RadioCommunication.txt","w");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	fprintf(wfp,"\n");
	while ((read = getline(&line, &len, fp)) != -1) {

		// --- Simulation Time ---//
		getColumn(line,1,buf); 
		fprintf(wfp,"%s ",buf);

		// --- Transmission Type --- //
		getColumn(line,3,buf);


		if( strcmp(buf,"SET") == 0 ){
			fprintf(wfp,"TX ");
			// --- Node ID --- //
			getColumn(line,2,buf);
			node_id = getNodeId(buf);
			tx_count[node_id]++;
			fprintf(wfp,"%d ",node_id);
			fprintf(wfp,"%d ",tx_count[node_id]);
		}

		getColumn(line,3,buf);
		if(strcmp(buf,"Received") == 0 ){
			fprintf(wfp,"RX ");
			// --- Node ID --- //
			getColumn(line,2,buf);
			fprintf(wfp,"%d ",getNodeId(buf));
			// --- Source Address ---//
			getColumn(line,8,buf);
			buf[strlen(buf)-1] = '\0';
			fprintf(wfp,"%d ",atoi(buf));
			// --- Success ---//
			fprintf(wfp,"1 ");
			fprintf(wfp,"with_no_interference");
		}

		getColumn(line,3,buf);
		if(strcmp(buf,"Failed") == 0){

			fprintf(wfp,"RX ");
			// --- Node ID --- //
			getColumn(line,2,buf);
			fprintf(wfp,"%d ",getNodeId(buf));
			// --- Source Address ---//
			getColumn(line,8,buf);
			buf[strlen(buf)-1] = '\0';
			fprintf(wfp,"%d ",atoi(buf));
			// --- Failure ---//
			fprintf(wfp,"-1 ");

			getColumn(line,9,buf);
			switch(getFailureType(buf)){

				case WITH_INF:
					fprintf(wfp,"with_interference");
					break;
				case NO_INF:
					fprintf(wfp,"NO_interference");
					break;
				case BELOW_SENS:
					fprintf(wfp,"below_sensitivity");
					break;
				case NOT_IN_RX:
					fprintf(wfp,"radio_not_in_RX_state");
					break;
			}
		}

		fprintf(wfp,"\n");
	}


	fclose(fp);
	if (line)
		free(line);
}



void updateNodeTiming(double* node_arr){
	for(int i=1;i<5;i++)
		node_arr[i] = node_arr[i-1] + (0.004 * 2);
}

void parseRounds(char* file_name){

	int round = 0;

	double node_x_wake_up[5] = {0,0,0,0,0};
	double node_0_wake_up[5] = {0,0,0,0,0};

	double diff = 0;

	fp = fopen(file_name, "r");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);

		getColumn(line,3,buf);
		buf[6] = '\0';
		if(strstr(buf,"round") != NULL){
			getColumn(line,3,buf);
			round = atoi(&buf[6]);
			printf("\n-------------- ROUND : %d ---------------\n", round);

			printf("dif: \n");
			for(int i=0;i<5;i++)
				printf("%d : %f\n", i+1,node_0_wake_up[i] - node_x_wake_up[i] );
			continue;
		}

		getColumn(line,3,buf);
		if(strcmp(buf,"WOKE_FOR_DATA_REQ") == 0 || strcmp(buf,"ENTER_DATA_PHASE") == 0){

			
			getColumn(line,2,buf);
			int node_id = getNodeId(buf);
			switch(node_id){

				case 0:
					getColumn(line,1,buf);
					node_0_wake_up[0] = atof(buf);
					updateNodeTiming(node_0_wake_up);
					printf("\n0: %f",node_0_wake_up[0]);
					break;

				default:
					getColumn(line,1,buf);
					node_x_wake_up[node_id -1] = atof(buf);
					printf("\n%d: %f",node_id,node_x_wake_up[node_id-1]);
					break;
			}

		}

	}

	fclose(fp);
	if (line)
		free(line);

}


int main(int argc, char** argv){

	char castrace_file[20] = "Castalia-Trace.txt";
	switch(atoi(argv[1])){

		case 1:
			parseRadioCommunication(argv[2]);
			break;

		case 2:
			parseTopology(argv[2]);
			break;

		case 3:
			parseMobility(argv[2]);
			break;

		case 4:
			parseEnergyConsumption(argv[2],atoi(argv[3]));
			break;

		case 5:
			parseRounds(castrace_file);
			break;

		case 6:
			parsePDR(argv[2]);
			break;

	}

	return 0;
}
