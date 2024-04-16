//参考URL
// http://www.musashinodenpa.com/arduino/ref/index.php 
  
  
/*  int SEL = 0;
  int WR = 1;

  // VFD_Input PIN Value
  int value_SEL = 0;
  int value_WR = 0;
  int value_D = 0;
  int num_D[8] = {0,0,0,0,0,0,0,0};
  int value_mask = 49407;
  bool bit_SEL = 0;
  // VFD_Output PIN No Setting
  int DO_SEL = 10;
  int DO_WR = 11;
*/
#define   BIT0    0x01
#define   BIT1    0x02
#define   BIT2    0x04
#define   BIT3    0x08
#define   BIT4    0x10
#define   BIT5    0x20
#define   BIT6    0x40
#define   BIT7    0x80

//Intterrupt
unsigned short int_SEL = 0;

unsigned long Loopcount; //メインループカウント
unsigned short Blink =1;

unsigned char  VFD_val[50] ={
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39  };

  
/**************************************************/
/*0 関数名   : setup                               */
/*0                                               */
/*1 概要     : 変数やピンモードの初期化                */
/**************************************************/
void setup() {
  // put your setup code here, to run once:
  int i; //ピンセッティング用

  //Input Pin Setting
  for(i=0;i<=9;i++)
    pinMode(i, INPUT);
/*  GP0 = SEL
//  GP1 = WR
//  GP2 = D0
//  GP3 = D1
//  GP4 = D2
//  GP5 = D3
//  GP6 = D4
//  GP7 = D5
//  GP8 = D6
//  GP9 = D7 */
    
  //Output Pin Setting
  for(i=10;i<=19;i++)
    pinMode(i, OUTPUT);
/*  GP10 = DoSEL
//  GP11 = DoWR
//  GP12 = Do_D0
//  GP13 = DO_D1
//  GP14 = DO_D2
//  GP15 = DO_D3
//  GP16 = DO_D4
//  GP17 = DO_D5
//  GP18 = DO_D6
//  GP19 = DO_D7 */
  
  Serial.begin(9600);  //Set Baud Rate

  //USER_LED
  pinMode(25,OUTPUT);

  //PB0で外部割込み
  attachInterrupt(0,SEL_edge,FALLING);
}

/**************************************************/
/*0 関数名   : loop                                */
/*0                                               */
/*1 概要     : メイン関数                           */
/**************************************************/
void loop() {
  //LED点滅!!
  Loopcount++;
  if(Loopcount > 0x3FFFF){
    Loopcount = 0;
    Blink = !Blink; 
  }
  digitalWrite(25, Blink);

  //デバッグ用シリアル
  Serial_COM();
}

/**************************************************/
/*0 関数名   : SEL_edge                            */
/*0                                               */
/*1 概要     : SEL信号外部端子割り込み関数　　　        */
/*static uint32_t gpio_get_all (void)             */
/**************************************************/
void SEL_edge(){
//  int_SEL = digitalRead(SEL);
//  int_SEL = gpio_get(SEL);

  uint32_t get_vfd; //raspi-picoの入力
  unsigned char vfd_dat; //VFDへの入力データ抽出

//  gpio_get_all();
  get_vfd = gpio_get_all();
  vfd_dat = (unsigned char)( get_vfd>>2 & 0x000000FF );
    
  VFD_val[int_SEL] = vfd_dat;

  int_SEL++;
  if(int_SEL > 51) int_SEL = 0;
}

/**************************************************/
/*0 関数名   : Serial_COM                          */
/*0                                               */
/*1 概要     : シリアルモニタコマンド関数             */
/**************************************************/
void Serial_COM(){

  if(Serial.available()){
    unsigned char val = Serial.read();
    unsigned char vfdcode;
    unsigned char i;

    if(val != 0xFF)
    {
      switch(val)
      {
        case 'c':
          int_SEL = 0;
          for(i=0;i<50;i++){
            vfdcode = 0;
            Serial.println(vfdcode);
          }
        break;
        case 'g':
          for(i=0;i<50;i++){
            vfdcode = Char2Char(VFD_val[i]);
            Serial.println(vfdcode);
          }
        break;
        case 'v':
          Serial.println("RasPico v240411");
        break;
        case 'x':
          Serial.println(int_SEL);
        break;
      }
    }
//    else stop();
  }
}

/**************************************************/
/*0 関数名   : Char2Char                            */
/*0                                               */
/*1 概要     : VFD_Code -> Char         　　        */
/**************************************************/
unsigned char Char2Char(unsigned char VFDCode){
  unsigned char CharCode = 0;
  
  switch(VFDCode){
    case 0x30:
       CharCode = 0x31;
      break;
    case 0x31:
       CharCode = 0x32;
      break;
    case 0x32:
       CharCode = 0x33;
      break;
    default:
       CharCode = VFDCode;
//       CharCode = (unsigned char)gpio_get_all();
      break;
  }
  
  return CharCode;
}
