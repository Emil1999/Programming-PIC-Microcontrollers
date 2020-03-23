#line 1 "C:/Users/User/Downloads/A211_Groupe_07_TP3/A211_Groupe_07_TP3/A211_Groupe_07_TP4Bis/mikroC/TP4_Bis.c"







sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;



int val_1 = 0;
int val_2 = 0;
int index;

bit update_flag;
bit data_recieved_flag;

unsigned int pot_val;
unsigned int old_pot_val;

char content_line_1[16];
char buffer[ 40 ];
char recieved_data;
char command_line[ 40 ];


void send_values(){
 if (update_flag){
 UART1_Write( 2 );
 UART1_Write_Text(content_line_1);
 UART1_Write( 3 );
 UART1_Write( 10 );
 UART1_Write( 13 );
 update_flag = 0;
 }
}


void interrupt(){
 if(RC1IF_bit == 1){
 recieved_data = UART_Read();
 switch (recieved_data){
 case '[':
 index = 0;
 break;
 case ']':
 command_line[index+1] = '\0';
 data_recieved_flag = 1;
 break;
 default :
 command_line[index++] = recieved_data;
 }
 }
}


void update_values(){
 if (Button(&PORTC, 0, 1, 0) && val_1 + 10 <= 255) {
 val_1 += 10;
 update_flag = 1;
 delay_ms(300);
 }

 if (Button(&PORTC, 1, 1, 0) && val_1 - 10 >= 0) {
 val_1 -= 10;
 update_flag = 1;
 delay_ms(300);
 }

 if (Button(&PORTC, 2, 1, 0) && val_2 + 10 <= 255) {
 val_2 += 10;
 update_flag = 1;
 delay_ms(300);
 }

 if (Button(&PORTC, 3, 1, 0) && val_2 - 10 >= 0) {
 val_2 -= 10;
 update_flag = 1;
 delay_ms(300);
 }
}


void update_pot(){
 old_pot_val = pot_val;
 pot_val = ADC_Read(0);
 if (old_pot_val != pot_val){
 update_flag = 1;
 }
}


void update_LCD(){
 if (update_flag){
 sprintf(content_line_1, "%03u %03u %04u", val_1, val_2, pot_val);
 Lcd_Out(1, 1, content_line_1);
 }
 if (data_recieved_flag){
 Lcd_Out(2,3, strncpy(buffer,command_line+0,3));
 Lcd_Out(2,9, strncpy(buffer,command_line+4,3));
 data_recieved_flag = 0;
 }
}


void terminal_init_message(){
 UART1_Write_Text("Connect� au PIC18F45K22");
 UART1_Write( 10 );
 UART1_Write( 13 );
 UART1_Write_Text("Bienvenue � mon programme...");
 UART1_Write( 10 );
 UART1_Write( 13 );
 UART1_Write_Text("Format des donn�es � envoyer: [X;Y]");
 UART1_Write( 10 );
 UART1_Write( 13 );
 UART1_Write_Text("O� X et Y sont des entiers compris entre 0 et 999.");
 UART1_Write( 10 );
 UART1_Write( 13 );
}


void LCD_init_message(){
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Out(1,1,"Initialisation...");
 Delay_ms(1000);
 Lcd_Cmd(_LCD_CLEAR);
 sprintf(content_line_1, "%03u %03u %04u", val_1, val_2, pot_val);
 Lcd_Out(1, 1, content_line_1 );
 Lcd_Out(2,1,"A:");
 Lcd_Out(2,7,"B:");
}


void init(){
 ANSELA = 0b00000001;
 ANSELB = 0;
 ANSELD = 0;
 ANSELC = 0;

 update_flag = 0;
 data_recieved_flag = 0;


 C1ON_bit = 0;
 C2ON_bit = 0;


 TRISC = 0b10001111;
 TRISA = 0b00000001;
#line 167 "C:/Users/User/Downloads/A211_Groupe_07_TP3/A211_Groupe_07_TP3/A211_Groupe_07_TP4Bis/mikroC/TP4_Bis.c"
 ADC_Init();

 UART1_Init(115200);
 terminal_init_message();

 Lcd_Init();
 LCD_init_message();

 Delay_1sec();
 RC1IE_bit = 1;
 RC1IF_bit = 0;
 PEIE_bit = 1;
 GIE_bit = 1;
}


void main() {
 init();


 for(;;){
 update_pot();
 update_values();
 update_LCD();
 send_values();
 }
}
