#include<stdio.h>
#include<string.h>
#include <stdlib.h>

/*����1����γ�������
����2�������ѭ��ͬʱ����ָ��
3��δ��������������
����4 ��ȡ�ļ�ʱ��һ�и���*/

int flag_step1=0;//�����������Ƿ�������0Ϊ������1Ϊ�ر�
int flag_set_world=0;//
int rows=0;
int cols=0;
char command_start[15];
int flag_startgame=0;
int** world;
int **decide_size_of_world(int rows,int cols){//���ڲ˵���������
    int **world=(int**)malloc(rows*sizeof(int*));//worldָ��ָ��̬��ά����**world��ָ��ָ���ָ�룬���ھ����У��������ܴ�С
    if (world==NULL){       
        return NULL;//��ֹ����
    }
    for (int i=0;i<rows;i++){
        world[i]=(int*)malloc(cols*sizeof(int*));
        if (world[i]==NULL){
        return NULL;
    }
    }
    return world;
}
void free_world(int **world,int rows){//���ʹ�õ��ڴ�
    for(int i=0;i<rows;i++){
        free(world[i]);
    }
    free(world);
}
void print_world(int **world,int rows,int cols){//��ӡ�����������
for (int i=0;i<rows;i++){
    for (int j=0;j<cols;j++){
        if(world[i][j]==0){
            printf( "��  ");
        }else if(world[i][j]==1){
            printf( "��  ");
            
        }
        }
    printf("\n");
    }    
}
//help����
void printhelp(void){
    printf("����ָ������");
}

//��������ʽ��Ϸ�ĺ���
int count_living_cell(int **world,int now_rows,int now_cols){//ע�����߶���0��ʼ
    //��ʱ�����rows��cols�Ƿ�Ϊ0���Ժ�Ҫ������world�Ƿ���ڵ��߼�
    int count_living=0;
    if(now_rows>0&&now_rows<rows-1&&now_cols>0&&now_cols<cols-1){//�����ڱ߽��ϵ����
        for(int i=now_rows-1;i<now_rows+2;i++){
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (world[i][j]==1){
                    count_living+=1;
                }
            }
        }
    }else {for(int i=now_rows-1;i<now_rows+2;i++){//�߽紦�����С��
            for(int j=now_cols-1;j<now_cols+2;j++){
                if(i>=0&&j>=0&&i<rows&&j<cols){
                    if(world[i][j]==1){
                        count_living+=1;
                    }
                    
                }
            }              
        }
    }
    count_living-=world[now_rows][now_cols];//�Լ�Ҫ����
    return count_living;
}//���溯�����ڸ���һ���ϸ��
void update_world(int **world){
    int temp_world[rows][cols];//����ÿһ���޸ģ���ֹ��;���޸�Ӱ��ı�
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            int count_neighbor=count_living_cell(world,i,j);
            if (count_neighbor<2||count_neighbor>3){
                temp_world[i][j]=0;
            }else if(count_neighbor==2||(count_neighbor==3&&world[i][j]==1)){
                temp_world[i][j]=world[i][j];
                
            }else if(count_neighbor==3&&world[i][j]==0){
                temp_world[i][j]=1;//������Ϸ������и���
            }
        }
    }
    for(int i=0;i<rows;i+=1){
        for(int j=0;j<cols;j+=1){
            world[i][j]=temp_world[i][j];//ͨ����������world
        }
    }
}
void read_file(char *filename ){//��read��һ�еõ�rows cols����decidesize�����õ�world������һ������д������
    FILE *fp=fopen(filename,"r");
    if (fp==NULL){
        printf("��ʧ��");
    }
    char ch;
    fscanf(fp, "%d %d", &rows, &cols);
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            ch=fgetc(fp);
            if(ch=='0'){
                world[i][j]=0;
            }else if(ch=='1'){
                world[i][j]=1;
            }else if(ch=='\n'||ch==EOF){
                break;
            }else{
                return;
            }
        }
        fscanf(fp, "\n");  // �������з�

    }
    fclose(fp);

}

int main(){
    int **world;
    printf("welcome word\n");//�������
    while(flag_step1==0){
        printf("���\n");
        scanf("%s",command_start);//����\d��ָ��
        if(strcmp(command_start,"\\d")==0){//�ж�\dָ���Ծ���
            flag_step1=1;
            system("cls");//����
            printf("�����ͼ���ģʽ\n");
            printf("�밴˳����������Ҫ�ĵ�ͼ�ĳ��Ϳ�,�м��ÿո����,��Ҫ����80*80.\n");
            
            scanf("%d %d",&rows,&cols);
            if(rows>0&&cols>0&&rows<81&&cols<81){//�ж��к��������Ƿ�Ϸ�
                flag_set_world=1;
            }
            if(flag_set_world==1){
            world=decide_size_of_world(rows,cols);//����һ��ָ����С������
            for (int i=0;i<rows;i++){
                for (int j=0;j<cols;j++){
                    world[i][j]=0;
            }
            }//���ڳ�ʼ������
            system("cls");                   
            int temp_rows,temp_cols;//����ı���к���
            char input_1[30];//�ж����������
            int flag_change=0;//�ж��Ƿ��˳����ϸ��ͼ��ѭ��
            int count_change=-1;//ͳ�Ʋ����Ĵ�������ֹ�տ�ʼִ��else if
            while(flag_change==0){
                fgets(input_1,30,stdin);
                system("cls"); 
                printf("����������Ҫ�Ļ�ϸ��λ�ã��ÿո������������Χ����ʾ����:\n");
                printf("��������������,������\\q\n");
                if (sscanf(input_1,"%d %d",&temp_rows,&temp_cols)==2&&0 <temp_rows && temp_rows <=rows && 0 < temp_cols && temp_cols <=cols){
                    world[temp_rows-1][temp_cols-1]=1;
                    printf("��%d��%d�б�ɻ�ϸ��!\n",temp_rows,temp_cols);
                }else if(strncmp(input_1,"\\q",2)==0){
                    flag_change=1;//ע�⣬�����Ժ�Ҫ��Ӷ���ͱ����ļ����߼�
                    flag_step1=0; 
                    memset(command_start, 0, sizeof(command_start)); 
                }else if(sscanf(input_1,"%d %d",&temp_rows,&temp_cols)!=2||temp_rows>=rows||temp_cols>=cols||temp_cols<0||temp_rows<0){
                    if(count_change>=0){
                    printf("��Ч������!��������!\n");//�Ը��ִ�������
                    }
                }
            print_world(world,rows,cols);
            count_change+=1;
            }
            }    
}else if(strcmp(command_start,"\\h")==0){
    printf("help����\n");//if�ĵ���һ��
}else if(strcmp(command_start,"\\p")==0){
    print_world(world,rows,cols);
    //��ӡ���е�ͼ
}else if(strcmp(command_start,"\\r")==0){//��ʼ��Ϸ
    flag_startgame=1;
    int auto_game=0;
    char game_state[10];
    if (rows<1||cols<1){
        printf("����:û�������к��е�ֵ,���Զ�����Ϊ10X10.");
        rows=10;
        cols=10;
    }
    printf("1");
    print_world(world,rows,cols);
    scanf("%xs",&game_state);
    
    while(flag_startgame==1){
        update_world(world);
        print_world(world,rows,cols);
        printf("\n");
        sleep(1);//�Զ�ģʽ�����ɡ����Ƶķ����е㸴�ӣ�����ȸ���
        
    }
}else if(command_start[1]=='\\'&&command_start[2]=='l'){
    char filename[20]={0};
    
    strncpy(filename, command_start + 2, sizeof(filename) - 1); // ��command_start[2]��ʼ����
    filename[sizeof(filename) - 1] = '\0';
    read_file(filename);
    print_world(world,rows,cols);
    flag_step1=0; //�ص���ʼ����
    memset(command_start, 0, sizeof(command_start));//��ʼ������ָ�������
}
    

    
}
}