/* Queue simulation */
/*
1. 서비스하는 사람을 3명으로 수정한다.
2. duration을 10,000으로 수정한다.
3. 서비스하는 사람들이 서비스를 못하는 시간(service_idle_time)의 총합을 계산하고 print_stat()에서 출력하도록 추가한다.
4. x축을 arrival_prob으로, y축을 통계값으로 사용하여 그래프를 그린다.
5. 그래프를 보면서 통계값에 대한 관찰소견을 프로그램과 그래프와 함께 제출한다.
6. 로그파일은 출력하지 않으며 제출하지 않는다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <time.h>

#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

typedef struct {
    int id;
    int arrival_time;
    int service_time;
} element;

typedef struct {
    element queue[MAX_QUEUE_SIZE];
    int front, rear;
} QueueType;
QueueType queue;

void init(QueueType *q)
{
    q->front = q->rear = 0;
}

int is_empty(QueueType *q)
{
    return (q->front == q->rear); 
}

int is_full(QueueType*q)
{
    return ((q->rear+1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType *q, element e)
{
    if(is_full(q))
        fprintf(stderr, "큐가 포화 상태!\n");
    q->rear = (q->rear+1) % MAX_QUEUE_SIZE;
    q->queue[q->rear] = e;
}

element dequeue(QueueType *q)
{
    if(is_full(q))
        fprintf(stderr, "큐가 공백 상태!\n");
    q->front = (q->front+1) % MAX_QUEUE_SIZE;
    return q->queue[q->front];
}

element peek(QueueType *q)
{
    if(is_empty(q))
        fprintf(stderr, "큐가 공백 상태\n");
    return q->queue[(q->front+1) % MAX_QUEUE_SIZE];
}


// 0에서 1사이의 실수 난수 생성 함수
double generate_random_number()
{
    return rand()/(double)RAND_MAX;
}

// 시뮬레이션에 필요한 여러가지 상태 변수
int duration = 10000;
double arrival_prob = 0.7;  // 하나의 시간 단위에 도착하는 평균 고객의 수
int max_serv_time = 5;      // 하나의 고객에 대한 최대 서비스 시간
int clock;

// 시뮬레이션의 결과
int customers = 0;          // 전체고객수
int served_customers = 0;       // 서비스받은 고객수
int waited_time = 0;            // 고객들이 기다린 시간
int service_idle_time[3] = {};

// 랜덤 숫자를 생성하여 고객이 도책했는지 도착하지 않았는지를 판단
int is_customer_arrived()
{
    if( generate_random_number() < arrival_prob )
        return TRUE;
    else return FALSE;
}
// 새로 도착한 고객을 큐에 삽입
void insert_customer(int arrival_time)
{
    element customer;

    customer.id = customers++;
    customer.arrival_time = arrival_time;
    customer.service_time = (int)(max_serv_time*generate_random_number()) + 1;
    enqueue(&queue, customer);
    printf("고객 %d이 %d분에 들어옵니다. 서비스시간은 %d분입니다.\n",
            customer.id, customer.arrival_time, customer.service_time);
}
// 큐에서 기다리는 고객을 꺼내어 고객의 서비스 시간을 반환한다.
int remove_customer()
{
    element customer;
    int service_time = 0;

    if( is_empty(&queue) ) return 0;
    customer = dequeue(&queue);
    service_time = customer.service_time - 1;
    served_customers++;
    waited_time += clock - customer.arrival_time;
    printf("고객 %d이 %d분에 서비스를 시작합니다. 대기시간은 %d분이었습니다.\n", 
            customer.id, clock, clock - customer.arrival_time);
    return service_time;
}
// 통계치를 출력한다.
void print_stat()
{
    int service_idle_time_total = 0;
    printf("\n");
    printf("서비스받은 고객수 = %d ", served_customers);
    printf("전체 대기 시간 = %d분 ", waited_time);
    printf("1인당 평균 대기 시간 = %f분 ", (double)waited_time/served_customers);
    printf("아직 대기중인 고객수 = %d명\n", customers - served_customers);
    for(int i=0; i<3; i++){
        service_idle_time_total += service_idle_time[0];
        printf("server[%d]의 idle 시간의 총합: %d", i, service_idle_time[i]);
    }
    printf("server들의 idle 시간의 총합: %d\n", service_idle_time_total);
}

// x axis: arrival_prob, y axis: value
void print_graph(int arrival_prob[], int value[])
{
    //
}

int main(void)
{
//    srand((unsigned int)time(NULL));

    int service_time[3] = {};     // 서비스의 alarm clock
    clock = 0;      // global clock

    while(clock < duration){
        clock++;    // clock 시간 하나 경과
        printf("현재시각=%d\n", clock);

        if( is_customer_arrived() ){      // new customer 처리
            insert_customer(clock);
        }

        // server의 idle time 계산
        for(int i=0; i<3; i++){
            if(service_time[i] == 0)
                service_idle_time[i]++;
        }

        
        for(int i=0; i<3; i++)
            printf("service time[%d] : %d\n", i, service_time[i]);
        

        for(int k=0; k < 3; k++){    // every service man k
            if(service_time[k] > 0) // 여기부터 service 처리
                service_time[k]--;
            else{
                service_time[k] = remove_customer();
            }
        }
    }

    print_stat();   // 변경이 필요함;

    return 0;
}

/*
// 시뮬레이션 프로그램
void main()
{
    int service_time = 0;   // 서비스의 alarm clock

    clock = 0;  // global clock
    while(clock < duration){
        clock++;    // clock 시간 하나 경과
        printf("현재시각=%d\n", clock);
        if( is_customer_arrived() ){    // new customer 처리
            insert_customer(clock);
        }
        if(service_time > 0)    // 여기부터 service 처리
            service_time--;
        else {
            service_time = remove_customer();
        }
    }
    print_stat();
}
*/