#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>

using namespace std;
int typeOfSelection, algorithmNo, totalTime, noOfProcesses, quant=1;
char **twoD_array;
//if type is trace = 0
//if type is stats = 1

typedef struct
{
    char processName[255];
    int arrivalTime = 0;
    int timeSlot = 0;
    int waiting = 0;
    int finish = 0;
    int turnOver =0;
    int num, exeTime=0;
    float normTurn= 0.0,responseRatio=0.0;
} process;
struct comp
{
    bool operator()(process p1,process p2)
    {
        if(p1.timeSlot >= p2.timeSlot)
        {
            if(p1.timeSlot == p2.timeSlot)
            {
                return (p1.arrivalTime > p2.arrivalTime);
            }
            else
                return true;
        }
        else
            return false;
    }
};
struct comp2
{
    bool operator()(process p1,process p2)
    {
        if(p1.responseRatio <= p2.responseRatio)
        {
            if(p1.responseRatio == p2.responseRatio)
            {
                return (p1.num > p2.num);
            }
            else
                return true;
        }
        else
            return false;
    }
};
process* ReadingInput(char * fileDiection)
{
    FILE *file = fopen(fileDiection,"r");
    if(file==NULL)
    {
        printf("File Not Found");
        return NULL;
    }
    char type[10]="NULL";
    fscanf(file,"%s\n",&type);
    if(!strcasecmp(type,"trace"))
        typeOfSelection = 0;
    else if (!strcasecmp(type,"stats"))
        typeOfSelection = 1;
    else
    {
        printf("error in type Selection");
        exit(0);
        return NULL;
    }
    if(!feof(file))
    {
        char line[255],*token;
        fscanf(file,"%s\n",&line);
        token = strtok(line,"-");
        algorithmNo=atoi(token);
        token = strtok(NULL,"\n");
        if(token!=NULL)
            quant=atoi(token);
        fscanf(file,"%d\n",&totalTime);
        fscanf(file,"%d\n",&noOfProcesses);
        process* processes = (process *) malloc(noOfProcesses*sizeof(process));
        int i=0;
        while(!feof(file) && i<noOfProcesses)
        {
            fscanf(file,"%s\n",&line);
            token = strtok(line,",");
            strcpy(processes[i].processName,token);
            token = strtok(NULL,",");
            processes[i].arrivalTime=atoi(token);
            token = strtok(NULL,",");
            processes[i].timeSlot=atoi(token);
            processes[i].num=i;
            i++;
        }
        fclose(file);
        return processes;
    }
    else
    {
        printf("Error in the File");
        return NULL;
    }
}
void drawLine(FILE *file)
{
    int x = 2*totalTime+8;
    for(int i =0 ; i < x; i++)
    {
        fprintf(file,"-");
        printf("-");
    }
    fprintf(file,"\n");
    printf("\n");
}
void drawNumber(FILE *file )
{
    int j=0;
    for(int i=0; i <totalTime +1 ; i++)
    {
        fprintf(file,"%d ",j);
        printf("%d ",j);
        j++;
        if(j%10 == 0)
            j=0;
    }
    fprintf(file,"\n");
    printf("\n");
    drawLine(file);
}
void arrayCreation(process* p)
{
    char name[255];
    twoD_array = (char**)malloc(noOfProcesses * sizeof(char*));
    for(int i =0; i< noOfProcesses; i++)
    {
        twoD_array[i]=(char*)malloc((2*totalTime+12)*sizeof(char));
        strcpy(name,p[i].processName);
        strcpy(name,p[i].processName);
        strcat(name,"     |");
        strcpy(twoD_array[i],name);
    }

}
void print2DArray(FILE *file)
{
    drawNumber(file);
    for(int i=0; i<noOfProcesses; i++)
    {
        fprintf(file,"%s\n",twoD_array[i]);
        printf("%s\n",twoD_array[i]);
    }
    drawLine(file);
    fclose(file);
}
void printStat(process *items,FILE *file)
{
    fprintf(file,"\n");
    printf("\n");
    fprintf(file,"Process    |");
    printf("Process    |");
    char word[255];
    for(int i=0 ; i <noOfProcesses; i++)
    {
        strcpy(word,"  ");
        strcat(word,items[i].processName);
        strcat(word,"  |");
        fprintf(file,"%s",word);
        printf("%s",word);
    }
    fprintf(file,"\n");
    printf("\n");
    fprintf(file,"Arrival    |");
    printf("Arrival    |");
    char str[25];
    for(int i=0 ; i <noOfProcesses; i++)
    {
        if(items[i].arrivalTime >= 10)
            strcpy(word," ");
        else
            strcpy(word,"  ");
        sprintf(str,"%d",items[i].arrivalTime);
        strcat(word,str);
        strcat(word,"  |");
        fprintf(file,"%s",word);
        printf("%s",word);
    }
    fprintf(file,"\n");
    printf("\n");
    fprintf(file,"Service    |");
    printf("Service    |");
    for(int i=0 ; i <noOfProcesses; i++)
    {
        if(items[i].timeSlot >= 10)
            strcpy(word," ");
        else
            strcpy(word,"  ");
        sprintf(str,"%d",items[i].timeSlot);
        strcat(word,str);
        strcat(word,"  |");
        fprintf(file,"%s",word);
        printf("%s",word);
    }
    fprintf(file," Mean|\n");
    printf(" Mean|\n");
    fprintf(file,"Finish     |");
    printf("Finish     |");
    for(int i=0 ; i <noOfProcesses; i++)
    {
        if(items[i].finish >= 10)
            strcpy(word," ");
        else
            strcpy(word,"  ");
        sprintf(str,"%d",items[i].finish);
        strcat(word,str);
        strcat(word,"  |");
        fprintf(file,"%s",word);
        printf("%s",word);
    }
    fprintf(file,"-----|\n");
    printf("-----|\n");
    fprintf(file,"Turnaround |");
    printf("Turnaround |");
    float summation = 0.0;
    for(int i=0 ; i <noOfProcesses; i++)
    {
        if(items[i].turnOver >= 10)
            strcpy(word," ");
        else
            strcpy(word,"  ");
        sprintf(str,"%d",items[i].turnOver);
        summation += items[i].turnOver;
        strcat(word,str);
        strcat(word,"  |");
        fprintf(file,"%s",word);
        printf("%s",word);
    }
    summation = summation/noOfProcesses;
    if(summation >=10)
    {
        fprintf(file,"%0.2f|\n",summation);
        printf("%0.2f|\n",summation);
    }
    else
    {
        fprintf(file," %0.2f|\n",summation);
        printf(" %0.2f|\n",summation);
    }
    fprintf(file,"NormTurn   |");
    printf("NormTurn   |");
    summation = 0.0;
    for(int i=0 ; i <noOfProcesses; i++)
    {
        strcpy(word," ");
        sprintf(str,"%0.2f",items[i].normTurn);
        summation += items[i].normTurn;
        strcat(word,str);
        strcat(word,"|");
        fprintf(file,"%s",word);
        printf("%s",word);
    }
    summation = summation/noOfProcesses;
    fprintf(file," %0.2f|\n",summation);
    printf(" %0.2f|\n",summation);
    fclose(file);
}
void FCFS(process * items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"FCFS  ");
    printf("FCFS  ");
    arrayCreation(items);
    const char *wait=".|", *working="*|", *notArrived=" |";
    items[0].finish=items[0].arrivalTime+items[0].timeSlot;
    items[0].turnOver = items[0].finish - items[0].arrivalTime;
    items[0].normTurn = (items[0].turnOver*1.0)/items[0].timeSlot;
    items[0].exeTime = 0;
    for(int i=1; i<noOfProcesses; i++)
    {
        items[i].waiting = items[i-1].timeSlot+items[i-1].waiting ;
        items[i].finish = items[i].waiting + items[i].timeSlot;
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
        items[i].exeTime = items[i].arrivalTime+items[i].waiting;
    }
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<totalTime; j++)
        {
            if(items[i].arrivalTime <= j)
            {
                if(items[i].waiting ==0 )
                {
                    if(items[i].timeSlot > 0)
                    {
                        strcat(twoD_array[i],working);
                        items[i].timeSlot--;
                    }
                    else
                        strcat(twoD_array[i],notArrived);
                }
                else
                {
                    strcat(twoD_array[i],wait);
                    items[i].waiting--;
                }
            }
            else
            {
                strcat(twoD_array[i],notArrived);
                items[i].waiting--;
            }
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
void RR(process* items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"RR-%d  ",quant);
    printf("RR-%d  ",quant);
    queue<process> execution;
    int j=0;
    int temp_quant = quant;
    process temp;
    int executionsPro[noOfProcesses][totalTime];
    for(int x=0; x<noOfProcesses; x++)
    {
        for(int y=0; y<totalTime; y++)
        {
            executionsPro[x][y]=0;
        }
    }
    for(int i=0; i<totalTime; i++)
    {
        if(items[j].arrivalTime ==i && j<noOfProcesses)
        {
            execution.push(items[j++]);
        }
        if(temp_quant ==0 && temp.timeSlot!=0)
        {
            execution.push(temp);
            temp_quant=quant;
        }
        if(temp_quant == quant)
        {
            temp = execution.front();
            items[temp.num].exeTime = i;
            execution.pop();
        }
        if(temp.timeSlot != 0)
        {
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            temp_quant--;
        }
        if(temp.timeSlot ==0)
        {
            items[temp.num].finish = i+1;
            temp_quant=quant;
        }
    }
    for(int i=0; i<noOfProcesses; i++)
    {
        items[i].waiting = items[i].exeTime-items[i].arrivalTime;
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
    }
    arrayCreation(items);
    const char *wait=".|", *working="*|", *notArrived=" |";
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<items[i].arrivalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        for(int j=items[i].arrivalTime; j<items[i].finish; j++)
        {
            if(executionsPro[i][j]==0)
            {
                strcat(twoD_array[i],wait);
            }
            else if(executionsPro[i][j]==1)
            {
                strcat(twoD_array[i],working);
            }
        }
        for(int j= items[i].finish; j<totalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
void SPN(process * items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"SPN   ");
    printf("SPN   ");
    priority_queue<process, vector<process>, comp> readyQueue;
    queue<process> execution;
    int j=0;
    for(int i=0; i<totalTime; i++)
    {
        if(items[j].arrivalTime == i && j<noOfProcesses)
        {
            if(execution.empty())
            {
                items[j].waiting=0;
                items[j].exeTime=0;
                execution.push(items[j]);
                j++;
            }
            else
            {
                readyQueue.push(items[j]);
                j++;
            }
        }
        execution.front().timeSlot--;
        if(execution.front().timeSlot <= 0)
        {
            int num = execution.front().num;
            items[num].finish = i+1;
            execution.pop();
        }
        if(execution.empty()&&!readyQueue.empty())
        {
            process inTurn = readyQueue.top();
            items[inTurn.num].exeTime = i+1;
            execution.push(items[inTurn.num]);
            readyQueue.pop();
        }
    }
    for(int i=0; i<noOfProcesses; i++)
    {
        items[i].waiting = items[i].exeTime-items[i].arrivalTime;
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
    }
    arrayCreation(items);
    const char *wait=".|", *working="*|", *notArrived=" |";

    for(int i=0; i<noOfProcesses; i++)
    {

        for(int j=0; j<items[i].arrivalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        if(items[i].exeTime == items[i].arrivalTime)
        {
            for(int j=items[i].arrivalTime; j < items[i].finish; j++)
            {
                strcat(twoD_array[i],working);
            }
        }
        else
        {
            for(int j=items[i].arrivalTime; j<items[i].exeTime; j++)
            {
                strcat(twoD_array[i],wait);
            }
            for(int j=items[i].exeTime; j<items[i].finish; j++)
            {
                strcat(twoD_array[i],working);
            }
        }
        for(int j= items[i].finish; j<totalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
void SRT(process * items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"SRT   ");
    printf("SRT   ");
    priority_queue<process, vector<process>, comp> readyQueue;
    int executions[noOfProcesses][totalTime];
    for(int x=0; x<noOfProcesses; x++)
    {
        for(int y=0; y<totalTime; y++)
        {
            executions[x][y]=0;
        }
    }
    int j=0;
    for(int i=0; i< totalTime; i++)
    {
        if(items[j].arrivalTime==i && j<noOfProcesses)
        {
            items[j].exeTime=i;
            readyQueue.push(items[j++]);
        }
        process sth = readyQueue.top();
        readyQueue.pop();
        sth.timeSlot--;
        executions[sth.num][i]=1;
        if(sth.timeSlot==0)
        {
            items[sth.num].finish=i+1;
        }
        else
        {
            readyQueue.push(sth);
        }
    }
    for(int i=0; i<noOfProcesses; i++)
    {
        items[i].waiting = items[i].exeTime-items[i].arrivalTime;
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
    }
    arrayCreation(items);
    const char *wait=".|", *working="*|", *notArrived=" |";
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<items[i].arrivalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        for(int j=items[i].arrivalTime; j<items[i].finish; j++)
        {
            if(executions[i][j]==0)
            {
                strcat(twoD_array[i],wait);
            }
            else if(executions[i][j]==1)
            {
                strcat(twoD_array[i],working);
            }
        }
        for(int j= items[i].finish; j<totalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
void HRRN(process * items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"HRRN  ");
    printf("HRRN  ");
    priority_queue<process, vector<process>, comp2> readyQueue;
    queue<process> runTime;
    int j=0;
    for(int i=0; i<totalTime; i++)
    {
        if(items[j].arrivalTime==i && j<noOfProcesses)
        {
            if(runTime.empty())
            {
                items[j].exeTime=i;
                runTime.push(items[j++]);
            }
            else
            {
                items[j].waiting = runTime.front().timeSlot;
                items[j].responseRatio = ((items[j].waiting+items[j].timeSlot)*1.0)/items[j].timeSlot;
                readyQueue.push(items[j++]);
            }
        }
        runTime.front().timeSlot--;
        if(runTime.front().timeSlot==0)
        {
            items[runTime.front().num].finish=i+1;
            runTime.pop();
            if(!readyQueue.empty())
            {
                items[readyQueue.top().num].exeTime = i+1;
                runTime.push(readyQueue.top());
                readyQueue.pop();
                queue<process> subReady;
                while(!readyQueue.empty())
                {
                    subReady.push(readyQueue.top());
                    readyQueue.pop();
                }
                process host;
                while(!subReady.empty())
                {
                    host=subReady.front();
                    subReady.pop();
                    host.waiting += runTime.front().timeSlot;
                    host.responseRatio = (host.waiting+host.timeSlot)*1.0/host.timeSlot;
                    readyQueue.push(host);
                }
            }
        }
    }
    for(int i=0; i<noOfProcesses; i++)
    {
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
    }

    arrayCreation(items);
    char wait[]=".|", working[]="*|", notArrived[]=" |";
    for(int i=0; i<noOfProcesses; i++)
    {

        for(int j=0; j<items[i].arrivalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        if(items[i].exeTime == items[i].arrivalTime)
        {
            for(int j=items[i].arrivalTime; j < items[i].finish; j++)
            {
                strcat(twoD_array[i],working);
            }
        }
        else
        {
            for(int j=items[i].arrivalTime; j<items[i].exeTime; j++)
            {
                strcat(twoD_array[i],wait);
            }
            for(int j=items[i].exeTime; j<items[i].finish; j++)
            {
                strcat(twoD_array[i],working);
            }
        }
        for(int j= items[i].finish; j<totalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
void FB1(process * items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"FB-1  ");
    printf("FB-1  ");
    queue<process> execution,execution2,execution3,execution4;
    int j=0;
    int executionsPro[noOfProcesses][totalTime];
    for(int x=0; x<noOfProcesses; x++)
    {
        for(int y=0; y<totalTime; y++)
        {
            executionsPro[x][y]=0;
        }
    }
    for(int i=0; i<totalTime; i++)
    {
        if(items[j].arrivalTime ==i)
        {
            execution.push(items[j++]);
        }
        if(!execution.empty())
        {
            process temp = execution.front();
            execution.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                if(!execution.empty())
                {
                    execution2.push(temp);
                }
                else if(execution.empty()&&execution2.empty()&&execution3.empty()&&execution4.empty())
                {
                    if(items[j].arrivalTime ==i+1)
                    {
                        execution.push(items[j++]);
                    }
                    execution.push(temp);
                }
                else
                {
                    execution2.push(temp);
                }
            }

        }
        else if(!execution2.empty())
        {
            process temp = execution2.front();
            execution2.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                execution3.push(temp);
            }
        }
        else if(!execution3.empty())
        {
            process temp = execution3.front();
            execution3.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                execution4.push(temp);
            }
        }
        else if(!execution4.empty())
        {
            process temp = execution4.front();
            execution4.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                execution4.push(temp);
            }
        }

    }
    for(int i=0; i<noOfProcesses; i++)
    {
        items[i].waiting = items[i].exeTime-items[i].arrivalTime;
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
    }
    arrayCreation(items);
    const char *wait=".|", *working="*|", *notArrived=" |";
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<items[i].arrivalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        for(int j=items[i].arrivalTime; j<items[i].finish; j++)
        {
            if(executionsPro[i][j]==0)
            {
                strcat(twoD_array[i],wait);
            }
            else if(executionsPro[i][j]==1)
            {
                strcat(twoD_array[i],working);
            }
        }
        for(int j= items[i].finish; j<totalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
void FB2I(process * items,char *output)
{
    FILE *file = fopen(output,"w");
    fprintf(file,"FB-2i ");
    printf("FB-2i ");
    queue<process> execution,execution2,execution3,execution4;
    int j=0;
    int executionsPro[noOfProcesses][totalTime];
    for(int x=0; x<noOfProcesses; x++)
    {
        for(int y=0; y<totalTime; y++)
        {
            executionsPro[x][y]=0;
        }
    }
    for(int i=0; i<totalTime; i++)
    {
        if(items[j].arrivalTime ==i)
        {
            execution.push(items[j++]);
        }
        if(!execution.empty())
        {
            process temp = execution.front();
            execution.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                if(!execution.empty())
                {
                    execution2.push(temp);
                }
                else if(execution.empty()&&execution2.empty()&&execution3.empty()&&execution4.empty())
                {
                    if(items[j].arrivalTime ==i+1)
                    {
                        execution.push(items[j++]);
                    }
                    execution.push(temp);
                }
                else
                {
                    execution2.push(temp);
                }
            }

        }
        else if(!execution2.empty())
        {
            process temp = execution2.front();
            execution2.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                execution3.push(temp);
            }
        }
        else if(!execution3.empty())
        {
            process temp = execution3.front();
            execution3.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                execution4.push(temp);
            }
        }
        else if(!execution4.empty())
        {
            process temp = execution4.front();
            execution4.pop();
            executionsPro[temp.num][i]=1;
            temp.timeSlot--;
            if(temp.timeSlot == 0)
            {
                items[temp.num].finish=i+1;
            }
            else
            {
                execution4.push(temp);
            }
        }

    }
    for(int i=0; i<noOfProcesses; i++)
    {
        items[i].waiting = items[i].exeTime-items[i].arrivalTime;
        items[i].turnOver = items[i].finish - items[i].arrivalTime;
        items[i].normTurn = (items[i].turnOver*1.0)/items[i].timeSlot;
    }
    arrayCreation(items);
    const char *wait=".|", *working="*|", *notArrived=" |";
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<items[i].arrivalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        for(int j=items[i].arrivalTime; j<items[i].finish; j++)
        {
            if(executionsPro[i][j]==0)
            {
                strcat(twoD_array[i],wait);
            }
            else if(executionsPro[i][j]==1)
            {
                strcat(twoD_array[i],working);
            }
        }
        for(int j= items[i].finish; j<totalTime; j++)
        {
            strcat(twoD_array[i],notArrived);
        }
        strcat(twoD_array[i]," ");
    }
    if(typeOfSelection == 1)
        printStat(items,file);
    else if(typeOfSelection == 0)
        print2DArray(file);
}
int main()
{
    char line[255],output[255] = "out.txt";
    printf("\tEnter file Content \n");
    printf("Trace OR Stats: ");
    scanf("%s",&line);
    if(!strcasecmp(line,"trace"))
        typeOfSelection = 0;
    else if (!strcasecmp(line,"stats"))
        typeOfSelection = 1;
    else
    {
        printf("error in type Selection\n\n\n");
        main();
    }
    char *token;
    printf("Algorithm no.: ");
    scanf("%s",&line);
    token = strtok(line,"-");
    algorithmNo=atoi(token);
    token = strtok(NULL,"\n");
    if(token!=NULL)
        quant=atoi(token);
    printf("Total Time: ");
    scanf("%d",&totalTime);
    printf("Number Of Processes: ");
    scanf("%d",&noOfProcesses);
    process* processes = (process *) malloc(noOfProcesses*sizeof(process));
    for(int i=0; i < noOfProcesses; i++)
    {
        printf("Process no.%d: ",i+1);
        scanf("%s",&line);
        token = strtok(line,",");
        strcpy(processes[i].processName,token);
        token = strtok(NULL,",");
        processes[i].arrivalTime=atoi(token);
        token = strtok(NULL,",");
        processes[i].timeSlot=atoi(token);
        processes[i].num=i;
    }
    printf("\n\n");
    switch(algorithmNo)
    {
    case 1:
        FCFS(processes,output);
        break;
    case 2:
        RR(processes,output);
        break;
    case 3:
        SPN(processes,output);
        break;
    case 4:
        SRT(processes,output);
        break;
    case 5:
        HRRN(processes,output);
        break;
    case 6:
        FB1(processes,output);
        break;
    case 7:
        FB2I(processes,output);
        break;
    default:
        printf("\n\tNot Found Algorithm\n");
        main();
        break;
    }
}
