/* COP 3502C Assignment 2
This program is written by: Harold Saenz */

#include <stdio.h>
#include <stdlib.h>


/*---------------------------------(structures)---------------------------------------------------*/

typedef struct Node
{
    int sequenceNumber;
    struct Node* next;
}Student;

typedef struct Queue
{
    int garageNumber;
    Student* front;
    Student* back;
    int nodeCount;
    int k;
    int th;
}queue;

/*---------------------------------(end of structures)---------------------------------------------------*/


/*---------------------------------(prototypes)---------------------------------------------------*/
Student* createStudent(int sequence);
void createReverseCircle(queue *q);
void rearrangeCircle(queue* q);
void display(queue *q);
queue** ArrayOfQueues(int numGarages);
int smallGarageNumber(queue* q[], int numGar);
queue** smallFirst(queue* q[], int numGar);
Student* DelList(queue* q, int k);
int enqueue(queue* q, Student* s);
int dequeue(queue* q);
int empty(queue* q);
int peek(queue* q);
int total_Student(queue* q);
queue** phase1(queue* q[], int num);
queue** phase2(queue* q[], int num);
int findMax_Q(queue** q, int num);

/*---------------------------------(end of prototypes)---------------------------------------------------*/





int main()
{
    int numGarages;

    scanf("%d", &numGarages);

    queue** Queues = ArrayOfQueues(numGarages);     // array of pointer queues

    printf("Initial status of nonempty queues\n");      
    smallGarageNumber(Queues, numGarages);          // print the small garage number first thats all it does

    for(int i = 0; i < numGarages; i++)         // change the order 
    {
        rearrangeCircle(Queues[i]);  
    }

    printf("After ordering status of nonempty queues\n");       // after order printing
    smallGarageNumber(Queues, numGarages); 
    
    //phase1
    Queues= phase1(Queues, numGarages);
    //phase2
    phase2(Queues, numGarages);

    return 0;
}


/*-------------------------------------(functions)---------------------------------------------------------------*/

queue** phase1(queue* q[], int num)
{
    printf("Phase1 elimination\n\n");
    q = smallFirst(q,num);
    return q;
}



queue** phase2(queue* q[], int num)
{
    printf("Phase2 elimination\n");
    int remainingStudents = 0;
    for(int i = 0; i < num; i++)
    {
        remainingStudents += total_Student(q[i]);       //count all the students(nodes)
        
    }
    //printf("%d  main..", remainingStudents);

    //find max queue sequence
    

    while(remainingStudents > 1)        // break when theres one student left
    {
        int deleteMax_Q = findMax_Q(q , num);       // get the max number in queue
        int fromGarage = q[deleteMax_Q]->garageNumber;      // store garage
        
        int delete_student = dequeue(q[deleteMax_Q]);       // store student and delete 
        remainingStudents--;
        //printf("remaining: %d", remainingStudents);     // debugger to check

        printf("Eliminated student %d from group for garage %d\n", delete_student,fromGarage);
        //display(q[deleteMax_Q]);

    }
    

    // get the last remaining
    for(int i = 0; i < num; i++)
    {
        if(!empty(q[i]))
        {   
            int winner_student = peek(q[i]);
            int Gar_win = q[i]->garageNumber;
            printf("\nStudent %d from the group for garage %d is the winner!", winner_student, Gar_win);
            dequeue(q[i]);
            break;
        }
    }

        
    // free everything
    for(int i = 0; i < num; i++)
    {
        free(q[i]);
    }
    free(q);

}



int total_Student(queue* q)
{
    int count;
    count = q->nodeCount;
    return count;
}



int findMax_Q(queue** q, int num)
{
    int max = -1;
    int maxIndex = -1;
    
    for (int i = 0; i < num; i++)
    {
        if (!empty(q[i])) // Only non-empty queues
        {   
            int frontSeq = peek(q[i]);

            if(frontSeq != -1)  //incase it comes back null (empty queue)
            {
                // trying to see if it even goes through all queues (debugger)
                //printf("Checking queue %d, front: %d\n", i, q[i]->front->sequenceNumber);

                if(frontSeq > max || (frontSeq == max && q[i]->garageNumber < q[maxIndex]->garageNumber))   //check if multiple have the same number or check fro highest
                {
                    max = frontSeq;
                    maxIndex = i;
                }
            }
        }
        
    }
    return maxIndex;
}



queue** ArrayOfQueues(int numGarages)   // this function will create an array of queues 
{
    
    queue** ArrrayQueue = malloc(numGarages * sizeof(queue*));      //allocate memory for queues

    for(int i = 0; i < numGarages; i++)                 // makes queues that will have their own linked list 
    {
        ArrrayQueue[i] = malloc(sizeof(queue));
        createReverseCircle(ArrrayQueue[i]);
    }

    return ArrrayQueue;

}



int enqueue(queue* q, Student* s)   //add to list 
{
    if(q->front == NULL)        // if no list set the new node as the front and back
    {
        q->front = s;
        q->back = s;
        s->next = q->front;

        q->nodeCount++;         // track how many are in the queue
    }
    else
    {
        q->back->next = s;          // place the new node to the back
        q->back = s;
        s->next = q->front;

        q->nodeCount++;
    }
    return 1;
}



void createReverseCircle(queue *q)      // creates a circular singly link list for specific queue and enqueus student nodes
{
    int garNumber, numStudents, k, th;

    scanf("%d %d %d %d",&garNumber, &numStudents, &k, &th);

    q->front = NULL;
    q->back = NULL;   
    q->nodeCount = 0;
    q->garageNumber = garNumber;
    q->k = k;
    q->th = th;

    for(int i = numStudents; i > 0; i--)        // loop the number of stundents
    {
        Student* newStudent = createStudent(i);
        enqueue(q, newStudent);
    }

}



Student* createStudent(int sequence)    // (allocate mem)student node creation
{
    Student* student = malloc(sizeof(Student));     // allocate memory and fill out the node

    student->sequenceNumber = sequence;
    student->next = NULL;

    return student;
}




void display(queue *q)      //print out the queues and the student sequences
{
    

    printf("%d", q->garageNumber);     
    if(empty(q))
    {
        printf("\n");
        return;
    }

    Student* t = q->front;      //walker

    do          // must use do loop cause we want to go until the end but since its circular we must end at the front 
    {
        printf(" %d", t->sequenceNumber);
        t = t->next;
        
    } while (t!=q->front);          //will help end at the front because the itll start there but the walker will aready be moved so it doesnt automatically stop at the beginning 
    //printf("back %d" ,q->back->next->sequenceNumber);
    printf("\n");
    
}



int smallGarageNumber(queue* q[], int numGar)   // find the smallest garage number so it can print out in the order of smallest garage Number 
{
    int finished[numGar];   // got to keep track of queues done 

    for(int i = 0; i < numGar; i++)
    {
        finished[i] = 0; // this will be the start meaning all are not printed yet
    }

    for(int remaining = numGar; remaining > 0; remaining--)        //loop throught the total of garages (numGarages) 
    {
        int min = -1;
        int smallGar = 10;      // since theres only 1-10 garages that can be used as options



        for(int i = 0; i < numGar; i++)        // this will go and find the smallest not printed garage number in the options
        {
            if(!finished[i] && q[i]->garageNumber <= smallGar)        //!finished is to skip the ones that already were done 
            {
                smallGar = q[i]->garageNumber;
                min = i;
            }
        }

        if(min!= -1 && q[min]->nodeCount > 0)  // when found have it go throught the display function and print 
        {
            display(q[min]);
            finished[min] = 1;       // this marks the location of the queue as done or printed so next time it goes around it will skip it
        }
    }
    printf("\n");
    return 1;
}



void rearrangeCircle(queue* q)      // make the new order
{
    if(q->front == NULL || q->front->next == q->front)      // if empty or only one node
    {
        return;
    }

    Student* prev, *next = NULL, *current = q->front;

    do                  // walk the circular list and reverse
    {                       // stores next node, reverse current pointer, move p
        next = current->next;              
        current->next = prev;
        prev = current;
        current = next;

    } while (current != q->front);       // stops when it goes back to front
    
    q->front->next = prev;          // make it circular to point back to the front node (was back node)
    q->front = prev;               // have front point to new front
    q->back = current;              // have back point to new back
    
}



Student* DelList(queue* q, int k)
{
    int skip = 1;       // counter
    int limit = q->th;

    Student* t;         // walker
    Student* temp;      // use to delete


    printf("Group for Garage# %d\n", q->garageNumber);

    if(empty(q))               // check if the list is empty
    {
        return q->front;
    }

    t = q->front;
    Student* prev = NULL;

    while(q->nodeCount > limit)
    {
        
        if(skip % k == 0)
        {
            printf("Student# %d eliminated\n", t->sequenceNumber);

            if(t == q->front)       // deleting from the front
            {
                temp = t;
                t = t->next;
                
                q->front = t;
                q->back->next = t;
                free(temp);

            }
            else if(t == q->back)      // deleting from the back
            {
            
                prev->next = q->front;
                q->back = prev;
                temp = t;
                t = prev->next;

                free(temp);

            }
            else
            {
                temp = t;
                // prev = one node before t
                prev->next = t->next;
                t = t->next;
                free(temp);

            }
            q->nodeCount--;

        }
        else
        {

            prev = t;
            t = t->next;
        }
        skip++;
    }

    printf("\n");
    return q->front;
}



int empty(queue* q)         // this just checks for empty queues
{
    return q->front == NULL;
}


// the function below is reused from function smallGarageNumber but altered to fit in the linked list deleting function.
queue** smallFirst(queue* q[], int numGar)   // find the smallest garage number so it can print out in the order of smallest garage Number 
{
    int finished[numGar];   // got to keep track of queues done 

    for(int i = 0; i < numGar; i++)
    {
        finished[i] = 0; // this will be the start meaning all are not printed yet
    }

    for(int remaining = numGar; remaining > 0; remaining--)        //loop throught the total of garages (numGarages) 
    {
        int min = -1;
        int smallGar = 10;      // since theres only 1-10 garages that can be used as options



        for(int i = 0; i < numGar; i++)        // this will go and find the smallest not printed garage number in the options
        {
            if(!finished[i] && q[i]->garageNumber <= smallGar)        //!finished is to skip the ones that already were done 
            {
                smallGar = q[i]->garageNumber;
                min = i;
            }
        }

        if(min!= -1 && q[min]->nodeCount > 0)  // when found have it go throught the display function and print 
        {
            q[min]->front= DelList(q[min], q[min]->k);     // delete
            finished[min] = 1;       // this marks the location of the queue as done or printed so next time it goes around it will skip it
        }
    }
    
    return q;
}



int peek(queue* q)      //look at who is front
{
    if(q->front != NULL)
    {
        return q->front->sequenceNumber;
    }
    else
    {
        return -1;
    }
    
    
}


int dequeue(queue* q)   // removes fron front
{
    if(q->front == NULL)
    {
        //printf("empty dequeue called");
        return 0;
    }

    Student* temp = q->front;
    int retval = temp->sequenceNumber;
    if(q->front == q->back)
    {
        q->front = NULL;
        q->back = NULL;
    }
    else
    {
        q->front = q->front->next;
        q->back->next = q->front;
    }

    free(temp);
    q->nodeCount--;

    
    return retval;
}
/*-----------------------------------------(end of functions)-------------------------------------------------------------*/
