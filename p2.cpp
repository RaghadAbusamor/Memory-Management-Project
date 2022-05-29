/**
 * @file OS_Project.cpp
 * @author Raghad abu samor
 * @brief  this is the Operating System project
 * @version 0.1
 * @date 2022-05-5
 * @details cocepts
      CPU utilization – keep the CPU as busy as possible
     Turnaround time – amount of time to execute a particular process
     Waiting time – amount of time a process has been waiting in the ready queue

 *
 *
 */
#include <iostream>
#include<algorithm>
#include <iomanip>
#include <fstream>
#include<cmath>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;
/**
 * @brief Global variabels
 * @details nof:# of frames,msize:mwmory size, psz:page size ,q :quantum,cs:context switch
 * @details  ab is a variable we used it in SJF represent Completion time for previous process
 *@details cpu_uti:cpu utilization
 */

int n,msize, psz, q, cs, nof;// n:numberof processes, nof:# of frames,msize:mwmory size, psz:page size ,q :quantum,cs:context switch
string memory[999999];
int i, ab;// ab is a variable we used it in SJF represent Completion time for previous process
float cpu_uti;//cpu_uti:cpu utilization

/**
 * @brief process control block
 * @details contains pro_id:process id
    artime = Arrival time,
	bt = Burst time,
	ct = Completion time,
	ta = Turn around time,
	wt = Waiting time
	prs=process size
	nop :#of pages
	ht :have tabel
 *
 */

struct pcb
{
	int pro_id, artime, bt, ct, ta, wt,  tabel[99999];
	float prs, nop;
	bool ht;
	/*
	pro_id:process id
	artime = Arrival time,
	bt = Burst time,
	ct = Completion time,
	ta = Turn around time,
	wt = Waiting time
	prs=process size
	nop :#of pages
	ht :have tabel
	*/
};


/**
 * @brief This process will always return TRUE if above condition comes and it used to compare the arrival time between processes
 *
 * @param p1  Process # 1
 * @param p2  Process # 2
 * @return true   if  p1.artime < p2.artime
 * @return false   if   p1.artime >= p2.artime
 */
bool compare(pcb p1, pcb p2)// to compare the arrival time between processes
{
	return p1.artime < p2.artime;
	/* This process will always return TRUE if above condition comes*/
}


/**
 * @brief This process will always return TRUE if above condition comes
 *
 * @param p1  Process # 1
 * @param p2  Process # 2
 * @return true
 * @return false
 */
bool compare2(pcb p1, pcb p2) // to compare the burst time between processes and if the process arrival time less than or equal to the Completion time of the previous process
{
	return p1.bt < p2.bt&& p1.artime <= ab;
	/* This process will always return TRUE
	if above condition comes*/
}

/**
 * @brief to implement pagging_memmory
 * @details finding table for processes and showing memory mapping in addition tranforming logical address to physical address
 */
void pagging_memmory(pcb pro[],int n)
{
     vector <int> v;// vector contains random numbers represent the number of free frames

   int r,arr[99999]={0};// r:random number , arr : array is used to prevent redundancy of generated random numbers

   nof=msize/psz; //nof #of frames
   vector <string>sample (nof,"free frame  ");//sample of memory size of it is # of frame
                                             //  each element in vector intialy contain free frame
       for(int i=0;i<nof;i++)
       {
              r=rand()%nof;
              if(arr[r] == 1)// if condition comes true this means the random was used
              {
                           i--;
                           continue;
              }
              arr[r]=1;// this refer this random is used

             v.push_back(r);

       }
       cout << endl;

  ;

  sort(pro,pro+n,compare);//sort the processes according to arival time
  for(int i=0;i<n;i++)
  {


     pro[i].nop=ceil(pro[i].prs/psz);// if the answer is decimal that means we will need extra frame for example: if 3.8 it will nedd 4 frames
     if(pro[i].nop<1)//if the size of process less than page size then # of pages is 1
     pro[i].nop=1;

     if(pro[i].nop<=v.size())
     {
         //construction of table
         pro[i].ht=true;
         cout<<endl<<"table";
          cout<< " for process  " <<pro[i].pro_id<<endl;
         for(int j=0;j<pro[i].nop;j++)
         {
         pro[i].tabel[j]=v[v.size()-1];
         //assign the last random in vector to page j

         cout<<"|page |"<<j<<"|  frame |"<<pro[i].tabel[j]<<"|"<<endl;

         //mapping pages to  memmory sample
         sample.erase(sample.begin()+v[v.size()-1]);
         sample.insert(sample.begin()+v[v.size()-1],"page"+to_string(j)+" for p"+to_string(pro[i].pro_id));


         //we fill the content of each page for the  process then mapping it to it's frame in physical memory

         int base=v[v.size()-1]*psz;// to detrmine the first address of page j in physical memory
         v.pop_back();// this will decrease the number of free frames and to prevent to use the frame we
                      // use to page j

         for(int k=0;k<psz;k++)
         {
              memory[base]="page"+to_string(j)+" for p"+to_string(pro[i].pro_id);

              base++;
         }//we fill the content of each page for the  process then mapping it to it's frame in physical memory



         }
     }

   else
   {
       pro[i].ht=false;
    cout <<endl<<"process "  <<pro[i].pro_id<<" need # pages needs > #of free frames so we cant put it in memory  "<<endl;
   }


  }

  //showing maping and free frames

  cout<<endl<<"memory mapping of the physical memory"<<endl<<endl;
  int itr=0;
  cout<<itr<<"      -------------------"<<endl;
  for(int i=0;i<nof;i++)
  {

      cout<<"      |   "<<sample[i]<<"    |"<<endl;

      itr+=psz;
       cout<<itr<<"   -------------------"<<endl;
  }

  int la ,pa,pid;//la logical address....pa physical address...pid :process id
  cout<<"enter process id you want to enter logical address for it  ";
  cin>>pid;
  int index;//to determine the index of procees you entered

  for (int i=0;i<n;i++)
  {
      if(pro[i].pro_id==pid)
      {
          index=i;
          break;
      }
  }

  if(pro[index].ht)
  {
      cout<<"enter logical adress for it ";
      cin>>la;

      if(la<(pro[index].nop*psz))//to check the validation of logical adress which user enters
     { int page_no = la / psz;
       int offset  = la % psz;
      cout<<"("<<page_no<<","<<offset<<")  -->   "<<"("<<pro[index].tabel[page_no]<<","<<offset<<")  ";
      pa= pro[index].tabel[page_no]*psz + offset;
      cout<<endl<<"physical address  ="<< pa<<endl;
     }
     else
     cout<<"you enter incorrect logical adress"<<endl;
  }
  else
  {
      cout<<"this process has not mapped yet"<<endl;
  }

}
/**
 * @brief in main we calling the functions and read the file contents
 *
 * @return int return 0 to show if no erroe
 */
int main() {
	ifstream inFile;
	// to read the file
	inFile.open("process.txt");
	if (!inFile) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}
	inFile >> n >>msize >> psz >> q >> cs;

     if(n==0){
        cout<<"***********||||||||||||THERE IS NO PROCESS TO BE SCHEDULNG||||||||||||**********"<<endl;
        return 0;
    }
    pcb* PROCESS = new pcb[n];
	for (int i = 0; i < n; i++)
	{
		inFile >> PROCESS[i].pro_id >> PROCESS[i].artime >> PROCESS[i].bt >> PROCESS[i].prs;
	}

	cout<<endl<<"pagging memory"<<endl;
	pagging_memmory(PROCESS,n);

	inFile.close();
	return 0;
}

