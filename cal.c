#include <gtk/gtk.h>

typedef struct _Data Data;
struct _Data {
GtkWidget *Window;
GtkWidget *BtnAdd;
GtkWidget *BtnSub;
GtkWidget *BtnMul;
GtkWidget *BtnDiv;
GtkWidget *BtnResult;
GtkWidget *BtnReset;


GtkWidget *ResultEntry;

float num1; // 첫번쨰 숫자 엔트리가 저장됨
float num2; // 두번째 숫자 엔트리가 저장됨
const gchar *cleartext; // 초기화용 gchar 

int isAdd;
int isSub;
int isMul;
int isDiv;
};

G_MODULE_EXPORT void quit (GtkWidget *window,gpointer data){
	gtk_main_quit ();
}
/*시그널(이벤트 선언)*/

/*연산자 버튼 클릭 이벤트*/
G_MODULE_EXPORT void on_AddBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
      // 첫번째로 입력될때는 첫번째 숫자 변수가 비어있다. 따라서 첫번쨰 숫자변수에 엔트리의 값을
      // 저장한다
    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
     //엔트리 값 받기
    char* result_stringadd = malloc(sizeof(char) * 10);
     // 엔트리에 뿌려줄 값생성
    memset(result_stringadd, 0, 10);
     // 메모리 초기화
    data->num1=atof(entrytemp); // 현재 엔트리의 값을 num1에 저장

    sprintf(result_stringadd,"",entrytemp);
     // 엔트리에 뿌려줄 값(초기화)
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringadd); // 엔트리 초기화
    data->isAdd=1;
    data->isSub=0;
    data->isMul=0;
    data->isDiv=0;
  }
}

G_MODULE_EXPORT void on_SubBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
      // 첫번째로 입력될때는 첫번째 숫자 변수가 비어있다. 따라서 첫번쨰 숫자변수에 엔트리의 값을
      // 저장한다
    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
     //엔트리 값 받기
    char* result_stringsub = malloc(sizeof(char) * 10);
     // 엔트리에 뿌려줄 값생성
    memset(result_stringsub, 0, 10);
     // 메모리 초기화
    data->num1=atof(entrytemp); // 현재 엔트리의 값을 num1에 저장

    sprintf(result_stringsub,"",entrytemp);
     // 엔트리에 뿌려줄 값(초기화)
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringsub); // 엔트리 초기화
    data->isAdd=0;
    data->isSub=1;
    data->isMul=0;
    data->isDiv=0;
  }
}

G_MODULE_EXPORT void on_MulBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
       // 첫번째로 입력될때는 첫번째 숫자 변수가 비어있다. 따라서 첫번쨰 숫자변수에 엔트리의 값을
      // 저장한다
    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
     //엔트리 값 받기
    char* result_stringmul = malloc(sizeof(char) * 10);
     // 엔트리에 뿌려줄 값생성
    memset(result_stringmul, 0, 10);
     // 메모리 초기화
    data->num1=atof(entrytemp); // 현재 엔트리의 값을 num1에 저장

    sprintf(result_stringmul,"",entrytemp);
     // 엔트리에 뿌려줄 값(초기화)
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringmul); // 엔트리 초기화
    data->isAdd=0;
    data->isSub=0;
    data->isMul=1;
    data->isDiv=0;
  }
}

G_MODULE_EXPORT void on_DivBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
        // 첫번째로 입력될때는 첫번째 숫자 변수가 비어있다. 따라서 첫번쨰 숫자변수에 엔트리의 값을
      // 저장한다
    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
     //엔트리 값 받기
    char* result_stringdiv = malloc(sizeof(char) * 10);
     // 엔트리에 뿌려줄 값생성
    memset(result_stringdiv, 0, 10);
     // 메모리 초기화
    data->num1=atof(entrytemp); // 현재 엔트리의 값을 num1에 저장

    sprintf(result_stringdiv,"",entrytemp);
     // 엔트리에 뿌려줄 값(초기화)
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringdiv); // 엔트리 초기화
    data->isAdd=0;
    data->isSub=0;
    data->isMul=0;
    data->isDiv=1;
  }
}

/*계산기 초기화, 계산 버튼 클릭 이벤트*/
G_MODULE_EXPORT void on_ClearBtn_clicked(GtkButton *button, Data *data){
  initData(data);
}

G_MODULE_EXPORT void on_ResultBtn_clicked(GtkButton *button, Data *data){
    char* result_stringres = malloc(sizeof(char) * 10);
    memset(result_stringres, 0,10);

   if(data->num1 != -1){ 
      //앞서 입력된 숫자가 있어야한다
      const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
      //엔트리 값 받기
  
      data->num2=atof(entrytemp); 
      // 현재 엔트리의 값을 num2에 저장
           
      if(data->isAdd==1){ // 더하기일때
       sprintf(result_stringres,"%.1f+%.1f=%.1f",data->num1,data->num2,data->num1+data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres); // 엔트리 초기화
      }
      else if(data->isSub==1){ // 빼기일때
       sprintf(result_stringres,"%.1f-%.1f=%.1f",data->num1,data->num2,data->num1-data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres); // 엔트리 초기화
      }
      else if(data->isMul==1){ // 곱하기일때
       sprintf(result_stringres,"%.1f*%.1f=%.1f",data->num1,data->num2,data->num1*data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres); // 엔트리 초기화
      }
      else if(data->isDiv==1){ // 나누기일때
       sprintf(result_stringres,"%.1f/%.1f=%.1f",data->num1,data->num2,data->num1/data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres); // 엔트리 초기화
      }
      data->isAdd=0;
      data->isSub=0;
      data->isMul=0;
      data->isDiv=0;
  }

}

void initData(Data *data){
/* //엔트리 값 받기
 char* result_string = malloc(sizeof(char) * 10);
 //엔트리에 뿌려줄 값생성
 memset(result_string, 0, 10);
 result_string="";*/
 data->num1=-1;
 data->num2=-1;
 data->isAdd=0;
 data->isSub=0;
 data->isMul=0;
 data->isDiv=0;
 gtk_entry_set_text(GTK_ENTRY(data->ResultEntry)," ");  
}



int main (int argc, char *argv[]){
 GtkBuilder *builder;
 GError *error;
 Data *data;
 gtk_init (&argc, &argv);
 /* 빌더 생성 및 UI 파일 열기 */
 builder = gtk_builder_new ();
 gtk_builder_add_from_file (builder,"cal.glade", NULL);      
 /*데이타 구조체 메모리 초기화*/
 data = g_slice_new (Data);

 /*데이터 구조체 내에 위젯 담기 및 시그널 등록 */
 data->Window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
 data->BtnAdd = GTK_WIDGET(gtk_builder_get_object(builder, "AddBtn"));
 data->BtnSub = GTK_WIDGET(gtk_builder_get_object(builder, "SubBtn"));
 data->BtnMul = GTK_WIDGET(gtk_builder_get_object(builder, "MulBtn"));
 data->BtnDiv = GTK_WIDGET(gtk_builder_get_object(builder, "DivBtn"));


 data->BtnResult = GTK_WIDGET(gtk_builder_get_object(builder, "ResultBtn"));
 data->BtnReset = GTK_WIDGET(gtk_builder_get_object(builder, "ClearBtn"));

 data->ResultEntry = GTK_WIDGET(gtk_builder_get_object (builder, "ResultEntry"));

 gtk_builder_connect_signals (builder, data);
 
 g_object_unref (G_OBJECT (builder));
 
 gtk_widget_show_all (data->Window);

 initData(data);
 /*GTK메인 루프 실행*/
 gtk_main ();
 /*데이터 구조체 메모리 해제*/
 g_slice_free (Data, data);
return (0);
} 
