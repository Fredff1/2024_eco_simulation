#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<conio.h>
#include <windows.h>
#include <time.h> 

int rows=0,cols=0;
int **world;
int flag_main_menu=0;
int flag_set_world=0;
//��ϰ����if��ո�,else if�󲻿ո� �տ�ʼ�õ���sleep����������������Ҫ�Ժ���Ϊ��λ���ּ�����windows.h��Sleep�������Ժ󲻻����
//�����ǹ����Ժ���

//��ʼ�����纯��
int **decide_size_of_world(int rows,int cols){//���ڲ˵���������
    int **world=(int**)malloc(rows*sizeof(int*));//worldָ��ָ��̬��ά����**world��ָ��ָ���ָ�룬���ھ����У��������ܴ�С
    if (world==NULL){       
        return NULL;//��ֹ����
    }
    for (int i=0;i<rows;i++){
        world[i]=(int*)malloc(cols*sizeof(int));
        if (world[i]==NULL){
        return NULL;
    }
    }
    return world;
}
//���ļ�,�����printf��debugʱʹ�õ�
void read_file(char *filename){
    FILE*fp=fopen(filename,"r");
    //printf("�򿪳ɹ�\n");
    if (fp == NULL) {
        printf("����\n");
        return;
    }
    if (fscanf(fp, "%d %d", &rows, &cols) != 2) {
        //printf("��ȡ����������ʧ��\n");
        return;
    }//��ȡ������Ϣ
    //printf("%d %d��һ�ж�ȡ�ɹ�\n",rows,cols);
    world=decide_size_of_world(rows,cols);
    

    //printf("��ʼ���ɹ�\n");
    // ����world
    fscanf(fp, "\n");
    //printf("������");
    char scan_input;
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            scan_input=fgetc(fp);//�����ȡ
            if (scan_input=='0'){
                world[i][j]=0;                
            }else if(scan_input=='1'){
                world[i][j]=1;
            }else if(scan_input=='\n'||scan_input==EOF){
                break;
            }else if(scan_input==' '){
                j-=1;//�����ո�
            }

        }fscanf(fp,"\n");//�������з�
        //printf("���гɹ�\n");
    }
    fclose(fp);
}
//��д���еĵ���ͼ
void write_file(char *filename){
    FILE*fp=fopen(filename,"w");
    fprintf(fp, "%d %d\n", rows, cols);//��һ�е�����
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            fprintf(fp,"%d ",world[i][j]);
        }
    fprintf(fp,"\n");
    }
    fclose(fp);
}

//����ڴ�ʵ�麯��
void free_world(int **world,int rows){
    for(int i=0;i<rows;i++){
        free(world[i]);
    }
    free(world);
}
//��ӡ���纯��
void print_world(int **world,int rows,int cols){
printf(" ");
for(int i=1;i<=cols;i+=1){
    if (i<10){
        printf(" %d ",i);
    }else{
        printf(" %d",i);
    }
}
printf("\n");
for (int i=0;i<rows;i++){
    if (i+1<10){
        printf(" %d",i+1);
    }else{
        printf("%d",i+1);
    }
    for (int j=0;j<cols;j++){
        if (world[i][j]==0){
            printf( "��  ");
        }else if(world[i][j]==1){
            printf( "��  ");
            
        }
        }
    printf("\n");
    }    
}
//�����Ǽ����ھӺ���
int count_living_cell(int **world,int now_rows,int now_cols){//ע�����߶���0��ʼ
    //��ʱ�����rows��cols�Ƿ�Ϊ0���Ժ�Ҫ������world�Ƿ���ڵ��߼�
    int count_living=0;
    if (now_rows>0&&now_rows<rows-1&&now_cols>0&&now_cols<cols-1){//�����ڱ߽��ϵ����
        for(int i=now_rows-1;i<now_rows+2;i++){
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (world[i][j]==1){
                    count_living+=1;
                }
            }
        }
    }else {for(int i=now_rows-1;i<now_rows+2;i++){//�߽紦�����С��
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (i>=0&&j>=0&&i<rows&&j<cols){
                    if (world[i][j]==1){
                        count_living+=1;
                    }
                    
                }
            }              
        }
    }
    count_living-=world[now_rows][now_cols];//�Լ�Ҫ����
    return count_living;
}
//���溯�����ڸ���һ���ϸ��
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
//�������������
void clear_all_input() {
    int remain;
    while ((remain = getchar()) != '\n' && remain != EOF);
}


//��������ʽ��Ϸ���̺���

//��ӭ���溯��
void welcome_to_game(){
    printf("****************************************\n");
    printf("****************************************\n");
    printf("\n");
    printf("\n");
    printf("          ��    ��    ��    Ϸ           \n");
    printf("             ף��van�Ŀ���!              \n");
    printf("\n");
    printf("\n");
    printf("****************************************\n");
    printf("****************************************\n");
}
//help����
void print_help(void){
    printf("����ָ������\n[\\h]��ӡ������ʾ\n[\\p]��ӡ��ǰ��ͼ\n[\\l]�����ͼ\n[\\s]������ͼ\n[\\d]�����ͼ���ģʽ\n");
    printf("[\\q]�˳���ͼ���ģʽ\n[\\r]��ʼ������Ϸ\n[end]�˳���Ϸ\n");
} 

//����Ϸ
void main_game(){
    int flag_start_game=1;
    int auto_game;
    char game_state[10];
    char input_test[20];
    int pause_time=1000;
    int flag_auto_pause=0;
    print_world(world,rows,cols);
    printf("��ѡ������ģʽ,����\\a�����Զ�ģʽ(�Զ�ģʽ��Ĭ�ϼ��Ϊ1��),����\\g�����ֶ�ģʽ\n");
    scanf("%s",game_state);
    clear_all_input();  
    if (strcmp(game_state,"\\a")==0) {
        auto_game=1;
    }else if(strcmp(game_state,"\\g")==0){
        auto_game=0;
    }else{
        printf("��Ч���룬Ĭ��Ϊ�Զ�ģʽ");
        auto_game=1;
        Sleep(1000);
    }
    system("cls");
    print_world(world,rows,cols);
    Sleep(1000);
    system("cls");
    while(flag_start_game==1){            
        if (auto_game==1){            
            if (kbhit()){//����������
                char pressed=getch();
                if (pressed=='\r'){//Windowsϵͳ�лس���\r\n��ʾ�����ԱȽ�\r����ȡ�ڶ����ַ��������
                    
                    system("cls");
                    print_world(world,rows,cols);
                     printf("�Զ�ģʽ����ͣ\n����\\e�����ֶ�ģʽ,����\\q�ص��˵���,����\\t���޸�ÿһ����֮��ļ��ʱ��,���س������Զ�ģʽ\n");
                    fgets(input_test,sizeof(input_test),stdin);
                    if (strcmp(input_test,"\n")==0){
                        system("cls");
                        continue;
                    }else if(strcmp(input_test,"\\e\n")==0){
                        auto_game=0;
                        continue;
                    }else if(strcmp(input_test,"\\q\n")==0){
                        flag_start_game=0;
                        break;
                    }else if(strcmp(input_test,"\\t\n")==0){
                        printf("��������ϣ���ļ��ʱ��(�Ժ���Ϊ��λ)\n");                     
                        if(scanf("%d",&pause_time)==1&&pause_time>0){                       
                            printf("���޸�ʱ��Ϊ%d,��ȴ�����",pause_time);
                            clear_all_input();
                            Sleep(2000);
                            system("cls");
                            continue;
                        }else{
                            printf("���������,���ı�ʱ��");
                            pause_time=1000;
                            continue;
                        }
                    }
                    
                }
            }  
        
            update_world(world);
            print_world(world,rows,cols);
            printf("�����Զ�ģʽ,���س���ͣ�Խ�һ���ı���ʱ�䡢�˳����˵�������ֶ�ģʽ");
            printf("\n");
            Sleep(pause_time);
            system("cls");
        }else if(auto_game==0){//�ֶ�ģʽ
            char continue_game[10];
            printf("�㴦���ֶ�ģʽ\n����\\g����һ����,����\\q�ص��˵���,����\\sw�����Զ�ģʽ\n");
            scanf("%s",continue_game);
            if (strcmp(continue_game,"\\g")==0){
                system("cls");
                update_world(world);
                print_world(world,rows,cols);
                printf("\n");
                printf("��������һ������\n");
            }else if(strcmp(continue_game,"\\q")==0){
                flag_start_game=0;
                printf("�ѻص��˵�\n");
            }else if(strcmp(continue_game,"\\sw")==0){//�л�ģʽ
                auto_game=1;
                clear_all_input();
                system("cls");
                printf("�����Զ�ģʽ\n");
            }else{
                system("cls");
                print_world(world,rows,cols);
                printf("��Ч����!\n");
                clear_all_input();

        }


        } 
    }    
}

//�������ļ�����
void import_file(){
    char filename[20];
    char input_4[10];
            printf("����������Ҫ������ļ���,ע���׺:");
            scanf("%s",filename);
            free_world(world,rows);
            rows=0,cols=0;           
            read_file(filename);
            system("cls");
            print_world(world,rows,cols);
            printf("�ļ�%s������\n",filename);
            printf("���س����ز˵�\n");
            while(!kbhit()){}
            getch();
            system("cls");
            
}
//�����ͼ
void save_file(){
    char filename[20];
        printf("����������Ҫ�������ļ���(�Ḳ�����е�ͬ���ļ�),����\\q�˳�:");
        scanf("%s",filename);
        if(strcmp(filename,"\\q")!=0){
        write_file(filename);
        printf("�ɹ�����Ϊ%s\n",filename);
        }else{
            printf("���˳���δ����\n");
        }
}
//��ӡ���е�ͼ
void print_current_world(){
    if (rows>0&&cols>0){
        printf("��ǰ�ĵ�ͼ״̬����\n");
        print_world(world,rows,cols);
    }else{
        printf("��ǰû�к��ʵĵ�ͼ!\n");
    }
}
//ϸ��ͼ���ģʽ
void design_mode(char menu_command){//�����������������������,������ᵼ������ѭ��
    flag_set_world=0;
    char quit_command_1[10];
    printf("�����ͼ���ģʽ\n");
     while(flag_set_world==0)  {
            printf("�밴˳����������Ҫ�ĵ�ͼ�ĳ��Ϳ�,�м��ÿո����,��Ҫ����60*60.����\\q���˳���ģʽ\n");            
            if (scanf("%d %d",&rows,&cols)==2){
                 if (rows>0&&cols>0&&rows<81&&cols<81){//�ж��к��������Ƿ�Ϸ�
                     flag_set_world=1;}
            }else if(scanf("%s",quit_command_1)==1){
                if(strcmp(quit_command_1,"\\q")==0){
                    break;
                }                
            }else {
                printf("���Ϸ�������\n");
        }
            if (flag_set_world==1){
            world=decide_size_of_world(rows,cols);//����һ��ָ����С������
            for (int i=0;i<rows;i++){
                for (int j=0;j<cols;j++){
                    world[i][j]=0;
            }
            }//���ڳ�ʼ������
            system("cls");                   
            int temp_rows,temp_cols;//����ı���к���
            char input_1[100];//�ж����������
            int flag_change=0;//�ж��Ƿ��˳����ϸ��ͼ��ѭ��
            int count_change=-1;//ͳ�Ʋ����Ĵ�������ֹ�տ�ʼִ��else if
            int random_generator;//�������������ϸ��
            while(flag_change==0){
                int leap=0;
                input_1[0] = '\0';
                fgets(input_1,100,stdin);
                char *input_1_trace=input_1;
                system("cls"); 
                if (sscanf(input_1,"%d %d",&temp_rows,&temp_cols)==2){
                    while (sscanf(input_1_trace, "%d %d%n", &temp_rows, &temp_cols,&leap) == 2) { // �����ǿ��Զ�ȡ��������ʱ����%n��ȡ�Լ���ȡ���ַ���������һ������
                        if (0 < temp_rows && temp_rows <= rows && 0 < temp_cols && temp_cols <= cols) {
                            world[temp_rows - 1][temp_cols - 1] = 1; // ��ָ��λ������Ϊ��ϸ��
                            printf("��%d��%d�б���˻�ϸ��!\n", temp_rows, temp_cols);
                        }else if(temp_rows<0&&temp_cols<0&&abs(temp_rows)<=rows&&abs(temp_cols)<=cols&&world[abs(temp_rows)-1][abs(temp_cols)-1]==1){
                            world[abs(temp_rows)-1][abs(temp_cols)-1]=0;
                            printf("��%d��%d�б����ϸ��\n",abs(temp_rows),abs(temp_cols));

                        }else {
                            printf("����(%d,%d)������Χ������\n", temp_rows, temp_cols);
                        }
                        input_1_trace+=leap;
                         while (*input_1_trace && (*input_1_trace == ' ')){//ָ��ָ����\0��ո�
                             input_1_trace+=1;
                         }           
                    }                     
                    print_world(world,rows,cols);
                    count_change+=1; 
                    printf("����������Ҫ�Ļ�ϸ��λ�ã��ÿո���������������룬ͬ���ÿո������������Χ����ʾ����:\n");
                    printf("����-x -y (x  y���л�ϸ����λ��)����ɾ����ϸ��\n");
                    printf("����random ���֣�ע��ո񣩿���������ɻ�ϸ��,�Ƽ���������10%%�ĸ���\n");
                    printf("��������������,������\\q\n");                   
                    continue;
                }else if(strncmp(input_1,"\\q",2)==0){//�����ĵ�
                    printf("�Ƿ���Ҫ�����ͼ���ļ�,�����������\\s,���Ҫ������һ��ҳ��������\\b����\n���������ַ������������˳�\n");
                    char input_2[20];
                    scanf("%s",input_2);
                    if(strcmp(input_2,"\\s")==0){
                        save_file();
                    }else if (strcmp(input_2,"\\b")==0){
                        continue;
                    }else{
                    flag_set_world=1;
                    flag_change=1;
                    break; //�˳����ѭ��  
                    }               
                }else if(sscanf(input_1,"random %d",&random_generator)==1){
                    srand(time(NULL));
                    if (random_generator<=(rows*cols)){
                        for(int i=0;i<random_generator;){
                            int random_rows=rand()%rows;
                            int random_cols=rand()%cols;
                            if (world[random_rows][random_cols]==0){
                                world[random_rows][random_cols]=1;
                                i+=1;
                            }
                        }
                    }else{
                        printf("��������ʵ�����");
                        continue;
                    }
                       
                }else if(strcmp(input_1,"\\h")==0){
                    print_help();
                }else if(sscanf(input_1,"%d %d",&temp_rows,&temp_cols)!=2||temp_rows>=rows||temp_cols>=cols){
                    if(count_change>=0){
                    printf("��Ч������!��������!\n");//�Ը��ִ�������
                    }
                } 
            print_world(world,rows,cols);
            printf("����������Ҫ�Ļ�ϸ��λ�ã��ÿո���������������룬ͬ���ÿո������������Χ����ʾ����:\n");
            printf("����-x -y (x  y���л�ϸ����λ��)����ɾ����ϸ��\n");
            printf("����random ���֣�ע��ո񣩿���������ɻ�ϸ��,�Ƽ���������10%%�ĸ���\n");
            printf("��������������,������\\q\n");
            count_change+=1;//��ֹ���õ�ͼ��Сʱ���ִ�����ʾ
            }
        } 

}
}





//��ʽ��ʼ
int main(){
    rows=0,cols=0;
    char menu_command[20]; 
    system("cls");  
    while(flag_main_menu==0){
        welcome_to_game();//��ӭ����
        print_help();
        printf("\n");
        printf("�������ڲ˵���\n");
        printf("������һ��ָ��,Ȼ�󰴻س�\n");
        scanf("%s",menu_command);
        if (strcmp(menu_command,"\\d")==0){
            system("cls");
            design_mode(menu_command);           
        }else if(strcmp(menu_command,"\\r")==0){//��ʽ��ʼ��Ϸ
            if(rows>0&&cols>0){
                system("cls");
                main_game();
            }else{
                system("cls");
                printf("û�г�ʼ����ͼ\n");
                continue;
            }
        }else if(strcmp(menu_command,"\\h")==0){//��ӡ�����ĵ�
            system("cls");
            
            continue;
        }else if(strcmp(menu_command,"\\l")==0){//�����ĵ�
            import_file();
            continue;
        }else if(strcmp(menu_command,"\\s")==0){//���������
            save_file();
        }else if(strcmp(menu_command,"\\p")==0){//��ӡ��ǰ����
            if (rows<=0||cols<=0){
                printf("û�к��ʵ�ͼ\n");
                system("cls");
                continue;
            }else{ 
                system("cls");          
                print_current_world(); 
                char input_3[10];
                printf("���س����ز˵�");
                while(!kbhit()){}
                getch();
                system("cls");
            }                                
        }else if(strcmp(menu_command,"end")==0){//������Ϸ
            break;
        }
        system("cls");
    }
    return 0;
}
//�������ж������⣬�ƺ��͵��Էֱ����й�

